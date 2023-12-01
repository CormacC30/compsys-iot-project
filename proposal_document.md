# Record Pal: Smart Record Shelf
#### Student Name: Cormac Costello  Student ID: 11399631   

# Project Background and description:
Own a disorganised record collection? Don't even know what's in there?
Look no further!
This is a Raspberry Pi / Arduino project which incorporates the use of a camera module with the Raspberry Pi.
You scan a record (Pi camera) and using and Google Cloud Vision image recognition API, the record details are fetched from web, added to your collection database, and added to a web dashboard. The Pi is connected to the home network via WiFi. The Pi camera will be triggered by a proximity/Gesture sensor (Arduino Carrier).
Your collection can be categorised alphabetically or by genre, and the records will be allocated a new spot on the record shelf. There will be a simple web interface built with JavaScript, and deployed on Glitch. 

Prototype will be built using single shelf/box.
There is potential to expand the project to read local temperature and humidity, with warning triggered when humidity exceeds 50% RH (not good for vinyl). Local conditions will be displayed on the local display and on web dashboard.

The web interface aims to feature a option to allow a user to select the record they wish to put back, and an LED will light up in the spot on the shelf allocated by the algorithm - using a HTTP GET request and a strip of individually addressable LEDs (Adafruit NeoPixel).

# Tools, Technologies and Equipment:
1.	Raspberry Pi (with camera module)
2.	Arduino MKR WiFi 1010 with IoT Carrier (optional)
3.	Addressable LED strip (Adafruit NeoPixel)
4.	Proximity sensor
5.	Python (for Raspberry Pi programming)
6.	Raspberry Pi
7.	Node.js / Express
8.	JavaScript
9.	Glitch
10.	Arduino IDE (for Arduino programming)
11.	Blynk
12.	Spotify API or YouTube API (for music playback)
13. Google Cloud vision API.

This is an ambitious project but many of the implementations described here are achievable with ample potential for future expansion/future releases with ideas for more advanced features (if time allows!).

# Project Repository
https://github.com/CormacC30/compsys-iot-project

