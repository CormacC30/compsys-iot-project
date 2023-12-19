#include <Wire.h>
#include "bsec.h"
#include <Arduino_APDS9960.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include <ThingSpeak.h>
#include <ArduinoJson.h>
#include <BlynkSimpleWifi.h>
#include <ArduinoHttpClient.h>
// #include <Firebase_Arduino_WiFiNINA.h>

#define OFF false
#define ON true

const char* iftttKey = "dhUEVYI8vPSvFOdJBIxPJh_IIeYmkDLINMy7egFHl0J"; 
const char* iftttEventOn = "dehumidify";
const char* iftttEventOff = "turn-off";
const char* iftttHost = "maker.ifttt.com";
const int iftttPort = 80;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASSWORD;
char auth[] = BLYNK_AUTH_TOKEN;
//char dburl[] = DATABASE_URL;
//char dbsecret[] =  DATABASE_SECRET;
int status = WL_IDLE_STATUS;
const char *mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;
bool deviceState = OFF;

String webhookURL;

WiFiClient wifiClient;

HttpClient client(wifiClient, iftttHost, iftttPort);

Bsec iaqSensor; // Bsec sensor for air quality

int moisturePin = A6; //moisture sensor on grove input A6
int moist;
bool trigger = false;

unsigned long previousMillisSerial = 0;
unsigned long previousMillisUpdate = 0;
const long serialInterval = 1000;   // Print to serial monitor every second
const long updateInterval = 15000;  // Update ThingSpeak every 15 seconds

String output;

BlynkTimer timer;

void checkIaqSensorStatus(void);
void errLeds(void);
void updateThingSpeak(void);

// FirebaseData firebaseData;

void setup(void)
{
  Serial.begin(115200); // 115200

  while (!Serial)
    ;
  pinMode(LED_BUILTIN, OUTPUT);

  iaqSensor.begin(BME68X_I2C_ADDR_LOW, Wire);

  setupWiFi();
  ThingSpeak.begin(wifiClient);

  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[13] = {
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_STABILIZATION_STATUS,
      BSEC_OUTPUT_RUN_IN_STATUS,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
      BSEC_OUTPUT_GAS_PERCENTAGE};

  iaqSensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  pinMode(moisturePin, INPUT);

  if (!APDS.begin())
  {
    Serial.println("Error initializing APDS-9960 sensor.");
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(2000L, writeTemperature);
  timer.setInterval(2000L, writeHumidity);
  timer.setInterval(2000L, writeIAQ);
  timer.setInterval(2000L, writeAmbientLight);
  timer.setInterval(2000L, writeMoisture);

//  Firebase.begin(dburl, dbsecret, ssid, pass);
//  Firebase.reconnectWiFi(true); 
}

int getAmbientLight() {
      while (!APDS.colorAvailable())
    {
      delay(5);
    }
    int r, g, b;
    APDS.readColor(r, g, b);
    int ambientLight = r + g + b;
    return ambientLight;
}


void loop(void)
{
  unsigned long currentMillis = millis();

  // Print to serial every 1 second
  if (currentMillis - previousMillisSerial >= serialInterval)
  {
    previousMillisSerial = currentMillis;
    output = "";
    int moisture = analogRead(moisturePin);

    int ambientLight = getAmbientLight();

    if (iaqSensor.run())
    {
      digitalWrite(LED_BUILTIN, LOW);
    output += "IAQ: " + String(iaqSensor.iaq);
    output += ", IAQ Acc: " + String(iaqSensor.iaqAccuracy);
    output += ", staticIAQ: " + String(iaqSensor.staticIaq);
    output += ", CO2: " + String(iaqSensor.co2Equivalent);
    output += ", VOC: " + String(iaqSensor.breathVocEquivalent);
    output += ", raw Temp: " + String(iaqSensor.rawTemperature);
    output += ", pressure (hPa): " + String(iaqSensor.pressure);
    output += ", rawHumidity: " + String(iaqSensor.rawHumidity);
    output += ", gas Resistance (Ohm): " + String(iaqSensor.gasResistance);
    output += ", stab status: " + String(iaqSensor.stabStatus);
    output += ", run in status: " + String(iaqSensor.runInStatus);
    output += ", heat compensated temp (°C): " + String(iaqSensor.temperature);
    output += ", humidity (%RH): " + String(iaqSensor.humidity);
    output += ", gas %: " + String(iaqSensor.gasPercentage);
    output += ", moisture: " + String(moisture);

      // Print the values
      Serial.print("Ambient Light: ");
      Serial.println(ambientLight);
      Serial.println();
      Serial.println(output);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      checkIaqSensorStatus();
    }
  }

  // Update ThingSpeak every 15 seconds
  
  if (currentMillis - previousMillisUpdate >= updateInterval)
  {
    previousMillisUpdate = currentMillis;
    updateThingSpeak();
  }
  
  // sendToDB();
  Blynk.run();
  timer.run();

}

void updateThingSpeak()
{
  float iaqAccuracy = iaqSensor.iaqAccuracy;
  float staticIAQ = iaqSensor.staticIaq;
  float humidity = iaqSensor.rawHumidity;
  float temperature = iaqSensor.temperature;
  int ambientLight = getAmbientLight();

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, A6);
  ThingSpeak.setField(4, staticIAQ);
  ThingSpeak.setField(5, ambientLight);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200)
  {
    Serial.println("Channel update successful.");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

/*
void sendToDB() {

  float iaqAccuracy = iaqSensor.iaqAccuracy;
  float staticIAQ = iaqSensor.staticIaq;
  float humidity = iaqSensor.rawHumidity;
  float temperature = iaqSensor.temperature;
  int ambientLight = getAmbientLight();
  int moisture = analogRead(moisturePin);

String path = "/";
String jsonString;

jsonString = "{\"temperature\":" + String(temperature) +
             ",\"humidity\":" + String(humidity) +
             ",\"air_quality\":" + String(staticIAQ) +
             ",\"ambient_light\":" + String(ambientLight) + 
             ",\"moisture\":" + String(moisture) + "}";

if(Firebase.pushJSON(firebaseData, path, jsonString)) {
  Serial.println("path: " + firebaseData.dataPath());
} else {
  Serial.println("error, " + firebaseData.errorReason());
}

  Serial.println("Pushing json... ");
}
*/

void checkIaqSensorStatus(void)
{
  if (iaqSensor.bsecStatus != BSEC_OK)
  {
    if (iaqSensor.bsecStatus < BSEC_OK)
    {
      output = "BSEC error code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    }
    else
    {
      output = "BSEC warning code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme68xStatus != BME68X_OK)
  {
    if (iaqSensor.bme68xStatus < BME68X_OK)
    {
      output = "BME68X error code : " + String(iaqSensor.bme68xStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    }
    else
    {
      output = "BME68X warning code : " + String(iaqSensor.bme68xStatus);
      Serial.println(output);
    }
  }
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void setupWiFi()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1000);
  }

  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true)
      ;
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
}
// function to construct the http request for ifttt trigger
void sendIftttRequest(const char* event) {
  String url = "/trigger/" + String(event) + "/json/with/key/" + iftttKey;

  if (client.connect(iftttHost, iftttPort)) {
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + iftttHost + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);
    client.stop();
  }
}

// Add these functions to represent turning on and off your device
void turnOnDevice() {
  Blynk.virtualWrite(V0, 1);
  Serial.println("Turning on the device");
  
  // Send request to IFTTT to trigger "dehumidify" event
  sendIftttRequest(iftttEventOn);
}

void turnOffDevice() {
  Blynk.virtualWrite(V0, 0);
  Serial.println("Turning off the device");
  
  // Send request to IFTTT to trigger "turn-off" event
  sendIftttRequest(iftttEventOff);
}

void writeTemperature()
{
  float temperature = iaqSensor.temperature;
  Blynk.virtualWrite(V1, temperature);
}

void writeHumidity()
{
  float humidity = iaqSensor.humidity;
  Blynk.virtualWrite(V2, humidity);
}

void writeIAQ()
{
  float airQuality = iaqSensor.staticIaq;
  Blynk.virtualWrite(V3, airQuality);
}

void writeMoisture()
{
  float moisture = analogRead(moisturePin);
  Blynk.virtualWrite(V4, moisture);
}

void writeAmbientLight()
{
  int ambientLight = getAmbientLight();
  Blynk.virtualWrite(V5, ambientLight);
}

BLYNK_WRITE(V0) {
  int buttonState = param.asInt();

  if (buttonState == 1) {
    // Button is pressed
    if (deviceState == OFF) {
      // Turn on the device
      turnOnDevice();

      deviceState = ON;
    } 
    }
    else if (buttonState == 0) {
      if (deviceState == ON) {
      // Turn off the device
      turnOffDevice();

      deviceState = OFF;  // Update deviceState only when turning off
      }
    }
}



