# 🌬️ Windi-Bot — Smart Window Automation System

**Windi-Bot** is a smart, Wi-Fi connected window-opening robot that automatically regulates room temperature and airflow using real-time indoor and outdoor data.  
Built with an **ESP32**, it uses temperature sensors and smart logic to open or close your windows for optimal comfort and energy efficiency — no manual effort required.

---

## Features

-  **Automatic Climate Control** — Adjusts windows based on real-time indoor and outdoor temperature data.  
-  **Smart Sensor Integration** — Uses onboard and external sensors for precise environmental readings.  
-  **Wi-Fi Connectivity** — Fetches outdoor weather data via network API and syncs configuration remotely.  
-  **Manual Override** — Local button or app control for manual window operation.  
-  **Energy Efficient** — Designed for low-power standby operation.  
-  **Universal Mount Design** — Fits standard sliding or hinged windows with simple mechanical linkage.

---

## Hardware Overview

| Component | Description |
|------------|-------------|
| **MicroController** | ESP-32 (Wi-Fi, Bluetooth, and sensor control) |
| **Temperature Sensor** | BMP-180 |
| **Motor/Actuator** | NEMA 17 stepper motor|
| **Motor Driver** | TMC2209 |
| **Display** | 1.3 inch OLED Module |
| **Power Supply** | 12V 2200mAh 20C DC liopo with onboard regulator |

---

##  Software Overview

The ESP32 runs **custom firmware** that:
- Reads sensor data for temperature and humidity  
- Connects to Wi-Fi to retrieve outdoor weather data  
- Determines window position based on user-defined comfort thresholds  
- Controls motor movement with safe limit handling  

This firmware is written in **C++ (Arduino / ESP-IDF)** and can be updated over USB or OTA (Over-the-Air).

---

## System Architecture

