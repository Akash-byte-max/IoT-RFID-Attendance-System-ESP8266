# IoT-RFID-Attendance-System-ESP8266
RFID-Attendance-Using-ESP8266 Smart-RFID-Attendance-System RFID-Attendance-Google-Sheets-ESP8266

📌 IoT RFID Attendance Management System using ESP8266
📖 Project Overview

This project implements an IoT-based RFID Attendance Management System using NodeMCU ESP8266, RFID RC522, and Google Sheets.

The system automatically records attendance when an RFID card is scanned and uploads the data to Google Sheets in real time using a secure HTTPS connection. Duplicate entries are detected and prevented.

This project is suitable for schools, colleges, offices, and academic demonstrations.

⚙️ Features

📶 WiFi-enabled attendance system using ESP8266

🆔 RFID-based user identification

🔔 Buzzer alert for scan confirmation and duplicate detection

📟 16×2 I2C LCD display for system status

☁️ Real-time data upload to Google Sheets

❌ Duplicate attendance prevention

📄 Paperless and automated attendance system

🧰 Hardware Components

NodeMCU ESP8266

RFID RC522 Module

RFID Cards / Tags

16×2 I2C LCD Display

Buzzer

Jumper Wires

Breadboard

🧪 Software & Tools

Arduino IDE

ESP8266 Board Package

Google Apps Script

GitHub

🔌 Circuit Connections (Important Pins)
Component	ESP8266 Pin
RFID SDA	D4
RFID RST	D3
Buzzer	D8
LCD SDA	D2
LCD SCL	D1
🔄 Working Principle

ESP8266 connects to the WiFi network.

System waits for an RFID card scan.

RFID UID is read and matched with stored user data.

If the card is new:

Welcome message is shown on LCD

Attendance is sent to Google Sheets

If the card is already scanned:

Duplicate entry is detected

LCD displays "Already Recorded"

📊 Google Sheets Integration

Attendance data is sent to Google Sheets using Google Apps Script via an HTTPS GET request.
The data includes:

Student Name

USN

Timestamp

💻 Source Code

Main Arduino code is provided in this repository.

Update WiFi SSID, Password, and Google Script URL before uploading.
