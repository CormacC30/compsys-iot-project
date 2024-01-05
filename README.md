# compsys-iot-project Cormac Costello

# MOULD-O-MATIC home environmental monitoring system.

# Background and description:
In Ireland, Black Mould is very common and poses a major health problem: some black moulds have been shown to be linked to serious health issues such as immune system disorders and asthma attacks. Black mould is caused by dampness and humidity, along with warm temperatures. Ireland has prime conditions for black mould growth. Black mould thrives on damp, dark, warm areas. During winter months condensation can form in poorly ventilated areas where people sleep, shower, or cook. It is extremely stubborn and difficult to remove. Many supermarkets bleach-based mould killers are simply not up to the task. Once it grows, it tends to keep recurring again and again. The best way in any case is to prevent the problem before it starts:
This project presents a novel IoT-based solution for early detection and prevention of the growth of black mould in the home.

# Setup Instructions:
Can be set up by downloading github repo, install dependencies with npm, connecting Arduino, carrier, and grove moisture sensor to grove input A6 on the carrier and away you go.
Also, configure Blynk account, (download template or contact me for the template) (add token to .env, and arduino-secrets.h), firebase credentials (add to .env). Home network SSID and password go into the arduino-secrets.h file.

# Intended use:
Moisture sensor to be mounted up in the corner of the room, on the wall or on the window.

Other sensors will just take data from surrounding environment.