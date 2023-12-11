#include "bsec.h"
#include <Arduino_MKRIoTCarrier.h>

/*** Declare Carrier object
MKRIoTCarrier carrier; 
*/

// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

// Create an object of the class Bsec
Bsec iaqSensor;

String output;

/****Declare the moisture and rgb sensor variables
int moisture = A6;
int moist;
bool trigger = false;

int r = 0, g = 0, b = 0;
unsigned long lastUpdate = 0;
*/

// Entry point for the example
void setup(void)
{
  /* Initializes the Serial communication */
  Serial.begin(115200); //115200
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  iaqSensor.begin(BME68X_I2C_ADDR_LOW, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

/******
  while(!Serial);
 // carrier.noCase();
  carrier.begin();
*/
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

  // Print the header
  output = "Timestamp [ms], IAQ, IAQ accuracy, Static IAQ, CO2 equivalent, breath VOC equivalent, raw temp[°C], pressure [hPa], raw relative humidity [%], gas [Ohm], Stab Status, run in status, comp temp[°C], comp humidity [%], gas percentage";
  Serial.println(output);

    /******moisture sensor
  pinMode(moisture,INPUT);
  */
}

// Function that is looped forever
void loop(void)
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run()) { // If new data is available
    digitalWrite(LED_BUILTIN, LOW);
    output = String(time_trigger);
    output += ", IAQ: " + String(iaqSensor.iaq);
    output += ", IAQ Acc: " + String(iaqSensor.iaqAccuracy);
    output += ", staticIAQ: " + String(iaqSensor.staticIaq);
    output += ", CO2: " + String(iaqSensor.co2Equivalent);
    output += ", VOC: " + String(iaqSensor.breathVocEquivalent);
    output += ", raw Temp: " + String(iaqSensor.rawTemperature);
    output += ", pressure: " + String(iaqSensor.pressure);
    output += ", rawHumidity: " + String(iaqSensor.rawHumidity);
    output += ", gasRes: " + String(iaqSensor.gasResistance);
    output += ", stab: " + String(iaqSensor.stabStatus);
    output += ", sts: " + String(iaqSensor.runInStatus);
    output += ", temp: " + String(iaqSensor.temperature);
    output += ", hum: " + String(iaqSensor.humidity);
    output += ", gas%:" + String(iaqSensor.gasPercentage);
    Serial.println(output);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    checkIaqSensorStatus();
  }
}

// Helper function definitions
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

/**** print moisture and RGB, 
  moist = analogRead(moisture);

  Serial.println("Reading grove inputs ");
  Serial.print("Moist: ");
  Serial.print(moist);

  Serial.println();

  if (carrier.Light.colorAvailable()) {
    carrier.Light.readColor(r, g, b);
  }
// Print updates every 100ms
  if (millis() - lastUpdate > 100) {
    lastUpdate = millis();

    Serial.print(" rgb=");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.println(b);
  }

  //***
   delay(1000);
   */
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}