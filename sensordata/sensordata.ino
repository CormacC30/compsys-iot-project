#include <Wire.h>
#include "bsec.h"
#include <Arduino_APDS9960.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include <ArduinoJson.h>

Bsec iaqSensor;

String output;

void checkIaqSensorStatus(void);
void errLeds(void);

int moisture = A6;
int moist;
bool trigger = false;

void setup(void)
{
  Serial.begin(115200); //115200

  while(!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);

  iaqSensor.begin(BME68X_I2C_ADDR_LOW, Wire);
 // output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
 // Serial.println(output);
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
    BSEC_OUTPUT_GAS_PERCENTAGE
  };

  iaqSensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

    pinMode(moisture,INPUT);

if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }

  // Print the header
 // output = "Timestamp [ms], IAQ, IAQ accuracy, Static IAQ, CO2 equivalent, breath VOC equivalent, raw temp[°C], pressure [hPa], raw relative humidity [%], gas [Ohm], Stab Status, run in status, comp temp[°C], comp humidity [%], gas percentage, Light R, Light G, Light B, Moisture";
 //  output = "IAQ, IAQ accuracy, Static IAQ, CO2 equivalent, breath VOC equivalent, raw temp[°C], pressure [hPa], raw relative humidity [%], gas [Ohm], Stab Status, run in status, comp temp[°C], comp humidity [%], gas percentage, Light R, Light G, Light B, Moisture";
//Serial.println(output);
}

void loop(void)
{
  output = "";
  int moisture = analogRead(A6); // Assuming the moisture sensor is connected to A6
 // unsigned long time_trigger = millis();


// check if a color reading is available
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;

  // read the color
  APDS.readColor(r, g, b);
  int ambientLight = r+g+b;

  if (iaqSensor.run()) { // If new data is available
    digitalWrite(LED_BUILTIN, LOW);
  //  output = String(time_trigger);
    output += "IAQ: " + String(iaqSensor.iaq);
    output += ", IAQ Acc: " + String(iaqSensor.iaqAccuracy);
    output += ", staticIAQ: " + String(iaqSensor.staticIaq);
    output += ", CO2: " + String(iaqSensor.co2Equivalent);
    output += ", VOC: " + String(iaqSensor.breathVocEquivalent);
    output += ", raw Temp: " + String(iaqSensor.rawTemperature);
    output += ", pressure: " + String(iaqSensor.pressure);
    output += ", rawHumidity: " + String(iaqSensor.rawHumidity);
    output += ", gas Resistance (Ohm): " + String(iaqSensor.gasResistance);
    output += ", stab status: " + String(iaqSensor.stabStatus);
    output += ", run in status: " + String(iaqSensor.runInStatus);
    output += ", heat compensated temp (°C): " + String(iaqSensor.temperature);
    output += ", humidity (%RH): " + String(iaqSensor.humidity);
    output += ", gas %: " + String(iaqSensor.gasPercentage);
    output += ", moisture: " + String(moisture);
    Serial.println();
    // print the values
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("g = ");
    Serial.println(g);
    Serial.print("b = ");
    Serial.println(b);
    Serial.println();
    Serial.print("Ambient Light: ");
    Serial.println(ambientLight);
    Serial.println();
//    Serial.println(output);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    checkIaqSensorStatus();
  }

  /*

  */
  Serial.println(output);  
  delay(1000);

}

void checkIaqSensorStatus(void)
{
  if (iaqSensor.bsecStatus != BSEC_OK) {
    if (iaqSensor.bsecStatus < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme68xStatus != BME68X_OK) {
    if (iaqSensor.bme68xStatus < BME68X_OK) {
      output = "BME68X error code : " + String(iaqSensor.bme68xStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
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