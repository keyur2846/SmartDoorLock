# SmartDoorLock
Smart RFID + WiFi-based door lock using Arduino, NodeMCU, and Blynk
# 🔐 Smart WiFi + RFID Door Lock

This is a smart and secure door lock system that uses **RFID authentication** and **WiFi-based remote control** via the **Blynk app**. The system combines an **Arduino UNO** (or Nano) and a **NodeMCU (ESP8266)** to control a **12V solenoid lock** through a **relay**.

This is ideal for home automation, secure lockers, or IoT-based access systems.

## 🔧 Features

- 🛡️ RFID-based access control
- 🌐 Remote lock/unlock via Blynk app (WiFi)
- 🔐 12V solenoid lock controlled by relay
- 🔄 Combination of Arduino + NodeMCU
- 🧠 Local + cloud control (offline RFID, online Blynk)
- 🔋 Powered by 12V supply (can be battery or adapter)

## 🧪 Technologies Used

- **Arduino UNO/Nano** – core logic and relay control
- **NodeMCU ESP8266** – handles WiFi & Blynk communication
- **Blynk IoT App** – smartphone control interface
- **RFID Module (RC522)** – user authentication
- **Relay Module** – electrical switch for solenoid
- **12V Solenoid Lock** – actual locking mechanism

## 🧰 Hardware Components

Here are the components required for building this project:

| Component              | Quantity | Description                          |
|------------------------|----------|--------------------------------------|
| Arduino UNO / Nano     | 1        | Main microcontroller for logic       |
| NodeMCU (ESP8266)      | 1        | WiFi communication + Blynk control   |
| RFID Reader (RC522)    | 1        | For scanning RFID cards/tags         |
| RFID Tags              | 2+       | Keychain or card format              |
| 12V Solenoid Lock      | 1        | Electromechanical locking device     |
| Relay Module           | 1        | To switch solenoid on/off            |
| 12V Adapter / Battery  | 1        | Power source for solenoid            |
| Jumper Wires           | Many     | For all connections                  |
| Breadboard / PCB       | 1        | For mounting and testing             |
| Optional: LED & Buzzer | 1 each   | For feedback (access granted/denied) |
