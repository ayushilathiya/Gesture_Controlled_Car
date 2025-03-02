# 🚗 Gesture-Controlled Car Using ESP8266 & MPU6050

![Project Banner](https://yourimageurl.com/banner.png)  
*Seamlessly control your car with hand gestures using ESP-NOW!*

---

## 📌 Overview

This project demonstrates a **gesture-controlled car** that operates using **ESP8266 modules communicating via ESP-NOW**. Using an **MPU6050 accelerometer and gyroscope**, the system detects hand gestures and translates them into movements for the car.

🔹 **Transmitter Module:** ESP8266 + MPU6050 (detects hand gestures)  
🔹 **Receiver Module:** ESP8266 + L298N Motor Driver + DC Motors (executes movements)

📖 *Check out the full blog post:* [Gesture-Controlled Car Using ESP8266 & MPU6050]([https://ayushilathiya.hashnode.dev/gesture-controlled-car-using-esp8266-and-mpu6050-a-wireless-solution](https://ayushilathiya.hashnode.dev/gesture-controlled-car-using-esp8266-and-mpu6050-a-wireless-solution))

---

## ✨ Features

🚀 **Wireless Communication:** Utilizes ESP-NOW protocol for fast, reliable data transfer  
🎮 **Real-Time Gesture Control:** Detects hand movements via MPU6050  
⚡ **Smooth & Responsive Motion:** Low latency execution for precise control  
🔋 **Compact & Portable:** No need for additional routers or Bluetooth pairing  
🛠️ **DIY & Open Source:** Fully customizable and expandable  

---

## 🛠️ Components Required

| 🛒 Component         | 🔢 Quantity |
|------------------|----------|
| ESP8266 NodeMCU  | 2        |
| MPU6050 Sensor  | 1        |
| L298N Motor Driver | 1        |
| DC Motors (3.6V) | 2        |
| Wheels          | 2 Rear + 1 Front |
| 12V Battery      | 1        |
| Jumper Wires     | As needed |

---

## 🔧 How It Works

1️⃣ **Hand Gesture Detection:** The MPU6050 detects hand movements (tilt forward, backward, left, right).  
2️⃣ **Wireless Data Transmission:** The transmitter ESP8266 sends commands via ESP-NOW to the receiver ESP8266.  
3️⃣ **Motor Control:** The receiver interprets commands and controls the L298N motor driver to move the car accordingly.  

---

## 📸 Project Images & Demo

![Circuit Diagram](https://yourimageurl.com/circuit-diagram.png)  
📽️ **Watch the demo video:** [YouTube Link](https://yourvideourl.com)  

---

## 🚀 Installation & Setup

### 🔹 1️⃣ Clone This Repository
```bash
git clone https://github.com/yourusername/gesture-controlled-car.git
cd gesture-controlled-car
```

### 🔹 2️⃣ Install Required Libraries
Ensure you have the following libraries installed in the Arduino IDE:
- 📡 **ESP8266WiFi**
- 📡 **ESP-NOW**
- 🎛️ **Wire**
- 🎛️ **MPU6050**

### 🔹 3️⃣ Upload Code to ESP8266 Modules
- 📤 **Transmitter Code:** Upload `transmitter.ino` to the first ESP8266 connected to the MPU6050.
- 📥 **Receiver Code:** Upload `receiver.ino` to the second ESP8266 connected to the motor driver.

### 🔹 4️⃣ Power Up & Test
- ⚡ Turn on the **transmitter module** and hold it in your hand.
- 🚗 Power up the **receiver module** (car) and observe the movement based on your gestures.

---

## 📜 License

📄 This project is licensed under the **MIT License** – feel free to modify and enhance it!  

✨ **Contributions are welcome!** If you have improvements or suggestions, feel free to create a pull request. 

---

### 🎯 Stay Connected
📢 **Follow for More Projects:** [@AyushiLathiya](https://github.com/AyushiLathiya)  
✍️ **Read My Blogs:** [ayushilathiya.hashnode.dev](https://ayushilathiya.hashnode.dev)  
💬 **Got Questions?** Open an issue or reach out on [LinkedIn](https://www.linkedin.com/in/your-profile)

---

### **🎮 Happy Coding, Happy Driving! 🚀**
```
