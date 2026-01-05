🌱 Smart Plant Watering & Monitoring System (IoT)

An IoT-based Smart Plant Watering and Monitoring System built using ESP32 and Blynk, designed to monitor soil moisture, temperature, and humidity, and automatically control irrigation for efficient water usage.

🚀 Features

🌱 Real-time soil moisture monitoring

💧 Automatic irrigation control based on soil moisture levels

📱 Manual pump control via Blynk mobile app

🔁 Auto / Manual mode switching (App + Physical Buttons)

🌡️ Temperature & Humidity monitoring using DHT11

🖥️ Live data display on OLED screen

☁️ Remote monitoring using Blynk IoT platform

🔌 Relay-based pump control (Active LOW)

🧠 Tech Stack

ESP32

Soil Moisture Sensor

DHT11 Temperature & Humidity Sensor

Relay Module & Water Pump

OLED Display (128×32, I2C)

Blynk IoT (Cloud + Mobile App)

AceButton Library

🔧 Hardware Connections
Component	ESP32 Pin
Soil Moisture Sensor	GPIO 34
Relay (Pump Control)	GPIO 25
Pump Button	GPIO 32
Mode Button	GPIO 33
DHT11 Sensor	GPIO 14
Pump LED	GPIO 2
Mode LED	GPIO 15
OLED SDA	GPIO 21
OLED SCL	GPIO 22
📲 Blynk Virtual Pins
Function	Virtual Pin
Pump Control	V5
Auto / Manual Mode	V6
Temperature	V11
Humidity	V8
Soil Moisture (%)	V12
⚙️ Working Logic
🔄 Auto Mode

Pump turns ON when soil moisture < 20%

Pump turns OFF when soil moisture > 80%

Fully autonomous operation

✋ Manual Mode

Pump controlled via:

Blynk mobile app

Physical push button

🖥️ OLED Display Data

Soil Moisture (%)

Temperature (°C)

Humidity (%)

Current Mode (AUTO / MANUAL)

📚 Libraries Used
WiFi.h
BlynkSimpleEsp32.h
DHT.h
AceButton.h
Adafruit_SSD1306.h
Wire.h


Install all libraries via Arduino Library Manager.

🛠️ Setup Instructions

Create a project in Blynk IoT

Copy:

Template ID

Template Name

Auth Token

Update WiFi credentials in code

Select ESP32 Dev Module in Arduino IDE

Upload the code

Power the system and monitor via Blynk app

🔮 Future Enhancements

📊 Data analytics dashboard

🚨 Push notifications & alerts

🤖 AI-based irrigation decisions

☀️ Solar-powered operation

📡 Cloud data logging

👨‍💻 Author

Sumukh Bharadwaj K S
IoT & Embedded Systems Enthusiast

📌 License

This project is open-source and free to use for learning and development purposes.
