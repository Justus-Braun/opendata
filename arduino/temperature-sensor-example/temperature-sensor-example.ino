#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>


// DFRobot_SHT3x sht3x(&Wire,/*address=*/0x76,/*RST=*/4);
Adafruit_SHT31 sht31 = Adafruit_SHT31();


void setup() {
  Serial.begin(9600);

  Serial.println("SHT31 test");
  if (!sht31.begin(0x44)) {  // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void loop() {
  // read_sensor();

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = ");
    Serial.print(t);
    Serial.print("\t\t");
  } else {
    Serial.println("Failed to read temperature");
  }

  if (!isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = ");
    Serial.println(h);
  } else {
    Serial.println("Failed to read humidity");
  }

  delay(1000);
}
