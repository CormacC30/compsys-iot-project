#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier; //Constructor of the carrier maybe we can include it on the library itself

int moisture = A6;
int moist;
bool trigger = false;

int r = 0, g = 0, b = 0;
unsigned long lastUpdate = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  carrier.noCase();
  carrier.begin();

  //moisture sensor
  pinMode(moisture,INPUT);
}

void loop() {
  // temp and humidity
  float temperature = carrier.Env.readTemperature();
  float humidity    = carrier.Env.readHumidity();

    // read the sensor value
  float pressure = carrier.Pressure.readPressure();

  // print the sensor value
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

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


// read all the sensor values
  float airQualityStatic = carrier.AirQuality.readStaticIAQ();
  float airQualityAccuracy = carrier.AirQuality.readIAQAccuracy();
  float airQualityIndex = carrier.AirQuality.readIAQ();

  // print each of the sensor values
  Serial.print("Static Air Quality Index Value = ");
  Serial.println(airQualityStatic);

  Serial.print("Air Quality Accuracy = ");
  Serial.println(airQualityAccuracy);

  Serial.print("Air Quality Index = ");
  Serial.println(airQualityIndex);

 delay(1000);


}
