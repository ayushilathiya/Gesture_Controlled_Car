#include <ESP8266WiFi.h>
#include <espnow.h>


#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define FORWARD_LEFT 5
#define FORWARD_RIGHT 6
#define BACKWARD_LEFT 7
#define BACKWARD_RIGHT 8
#define TURN_LEFT 9
#define TURN_RIGHT 10
#define STOP 0


#define BACK_RIGHT_MOTOR 0
#define BACK_LEFT_MOTOR 1
#define FRONT_RIGHT_MOTOR 2
#define FRONT_LEFT_MOTOR 3


struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
  int pinEn;
};


std::vector<MOTOR_PINS> motorPins = {
  {D0, D1, D2},  // BACK_RIGHT_MOTOR
  {D3, D4, D5},  // BACK_LEFT_MOTOR
  {D6, D7, D8},  // FRONT_RIGHT_MOTOR
  {D3, D4, D5}   // FRONT_LEFT_MOTOR (Shared EN pin with BACK_LEFT_MOTOR)
};


#define MAX_MOTOR_SPEED 200
#define SIGNAL_TIMEOUT 1000


unsigned long lastRecvTime = 0;


struct PacketData {
  byte xAxisValue;
  byte yAxisValue;
  byte zAxisValue;
};
PacketData receiverData;


void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len == 0) return;


  memcpy(&receiverData, incomingData, sizeof(receiverData));
  Serial.printf("Received: x=%d, y=%d, z=%d\n", receiverData.xAxisValue, receiverData.yAxisValue, receiverData.zAxisValue);


  if (receiverData.xAxisValue < 75 && receiverData.yAxisValue < 75)
    processCarMovement(FORWARD_LEFT);
  else if (receiverData.xAxisValue > 175 && receiverData.yAxisValue < 75)
    processCarMovement(FORWARD_RIGHT);
  else if (receiverData.xAxisValue < 75 && receiverData.yAxisValue > 175)
    processCarMovement(BACKWARD_LEFT);
  else if (receiverData.xAxisValue > 175 && receiverData.yAxisValue > 175)
    processCarMovement(BACKWARD_RIGHT);
  else if (receiverData.zAxisValue > 175)
    processCarMovement(TURN_RIGHT);
  else if (receiverData.zAxisValue < 75)
    processCarMovement(TURN_LEFT);
  else if (receiverData.yAxisValue < 75)
    processCarMovement(FORWARD);
  else if (receiverData.yAxisValue > 175)
    processCarMovement(BACKWARD);
  else if (receiverData.xAxisValue > 175)
    processCarMovement(RIGHT);
  else if (receiverData.xAxisValue < 75)
    processCarMovement(LEFT);
  else
    processCarMovement(STOP);


  lastRecvTime = millis();
}


void processCarMovement(int inputValue) {
  switch (inputValue) {
    case FORWARD:
      rotateMotor(FRONT_RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(BACK_RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(FRONT_LEFT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(BACK_LEFT_MOTOR, MAX_MOTOR_SPEED);
      break;
    case BACKWARD:
      rotateMotor(FRONT_RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(BACK_RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(FRONT_LEFT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(BACK_LEFT_MOTOR, -MAX_MOTOR_SPEED);
      break;
    case LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(BACK_RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(FRONT_LEFT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(BACK_LEFT_MOTOR, MAX_MOTOR_SPEED);
      break;
    case RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(BACK_RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(FRONT_LEFT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(BACK_LEFT_MOTOR, -MAX_MOTOR_SPEED);
      break;
    case STOP:
    default:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);
      break;
  }
}


void rotateMotor(int motorNumber, int motorSpeed) {
  if (motorSpeed < 0) {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  } else if (motorSpeed > 0) {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  } else {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
  analogWrite(motorPins[motorNumber].pinEn, abs(motorSpeed));
}


void setUpPinModes() {
  for (auto motor : motorPins) {
    pinMode(motor.pinIN1, OUTPUT);
    pinMode(motor.pinIN2, OUTPUT);
    pinMode(motor.pinEn, OUTPUT);
    rotateMotor(&motor - &motorPins[0], STOP);
  }
}


void setup() {
  setUpPinModes();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
  if (millis() - lastRecvTime > SIGNAL_TIMEOUT) {
    processCarMovement(STOP);
  }
}
