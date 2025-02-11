#include <ESP8266WiFi.h>
#include <espnow.h>

// These are needed for MPU
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// MPU control/status vars
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// RECEIVER MAC Address
// 2C:F4:32:63:35:7B
uint8_t receiverMacAddress[] = {0x2C, 0xF4, 0x32, 0x63, 0x35, 0x7B};

struct PacketData {
  byte xAxisValue;
  byte yAxisValue;
  byte zAxisValue;
};
PacketData data;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  // Serial.println(status == 0 ? "Message sent" : "Message failed");
}

void setupMPU() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  mpu.initialize();
  devStatus = mpu.dmpInitialize();

  if (devStatus == 0) {
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.setDMPEnabled(true);
      dmpReady = true;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Success: Initialized ESP-NOW");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // Required for ESP8266

  esp_now_register_send_cb(OnDataSent);

  // Register peer
  if (esp_now_add_peer(receiverMacAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Success: Added peer");
  }

  setupMPU();
}

void loop() {
  if (!dmpReady) return;

  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    int xAxisValue = constrain(ypr[2] * 180 / M_PI, -90, 90);
    int yAxisValue = constrain(ypr[1] * 180 / M_PI, -90, 90);
    int zAxisValue = constrain(ypr[0] * 180 / M_PI, -90, 90);

    data.xAxisValue = map(xAxisValue, -90, 90, 0, 254);
    data.yAxisValue = map(yAxisValue, -90, 90, 0, 254);
    data.zAxisValue = map(zAxisValue, -90, 90, 0, 254);

    int result = esp_now_send(receiverMacAddress, (uint8_t *)&data, sizeof(data));

    String inputData = String("values ") + xAxisValue + " " + yAxisValue + " " + zAxisValue;
    Serial.println(inputData);
    delay(50);
  }
}
