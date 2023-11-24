# Mould Detector
#### Student Name: Cormac Costello  Student ID: 11399631   

# Project Background and description:
This will be achieved by using an Arduino MKRWiFi1010 in the IoTCarrier board equipped with sensors for monitoring the local air temperature, humidity, air quality, light intensity, and moisture at the surface of the wall to allow early detection of conditions in your home for black mould growth. 
The sensor data from the Arduino can be sent to a ThingSpeak channel using MQTT. the data accessed and managed directly through this. When conditions become favourable for black mould growth (e.g. temperature > 18 oC, humidity > 70% RH, moist surface, dark), the user will receive a notification to open windows, switch on dehumidifiers via email/social media. This can be done using MQTT broker with a ThingSpeak React and IFTTT Webhooks. If this is up and running effectively, a further release will incorporate a Raspberry Pi connected to the Arduino via serial USB connection over which sensor data will be sent to the Pi. The Pi can be connected to the WiFi home router. If needs be, the raw data can be processed locally on the Pi e.g. using a Python script) and made available on a REST API. This data can be graphically displayed on a web dashboard, with member login (built using a JavaScript framework such as Express), deployed with Glitch. A user can activate fan/dehumidifier (simulated in Packet Tracer) using a HTTP request through the API. Using the Pi also enables the possibility of incorporating a camera, and image feed (a stretch goal for later releases).

# Tools, Technologies and Equipment:
1.	Arduino MKRWIFI1010 with IoT carrier
2.	Arduino IDE
3.	Onboard Environmental sensors: temperature, humidity, RGB light sensor, IAQ sensor.
4.	Capacitive moisture sensor
5.	C++
6.	Raspberry Pi
7.	Python
8.	Node.js / Express
9.	JavaScript
10.	Glitch
11.	ThingSpeak
12.	IFTTT/Webhooks
13.	Packet Tracer
This is an ambitious project but many of the implementations described here are achievable with ample potential for future expansion/future releases with ideas for more advanced features (if time allows!).

# Project Repository
https://github.com/CormacC30/compsys-iot-project

