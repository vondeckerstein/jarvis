//  This Arduino sketch reads DS18B20 "1-Wire" digital
//  temperature sensors.
//  Copyright (c) 2010 Mark McComb, hacktronics LLC
//  License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
//  Tutorial:
//  http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 7

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress therm1 = { 0x28, 0x65, 0x7B, 0xCF, 0x05, 0x00, 0x00, 0xBD };
DeviceAddress therm2 = { 0x28, 0x68, 0xD6, 0xCF, 0x05, 0x00, 0x00, 0x28 };
DeviceAddress therm3 = { 0x28, 0x7C, 0xAF, 0xCF, 0x05, 0x00, 0x00, 0x31 };
DeviceAddress therm4 = { 0x28, 0xC6, 0x66, 0xCF, 0x05, 0x00, 0x00, 0x28 };
DeviceAddress therm5 = { 0x28, 0x91, 0x5F, 0xCF, 0x05, 0x00, 0x00, 0xF4 };

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(therm1, 10);
  sensors.setResolution(therm2, 10);
  sensors.setResolution(therm3, 10);
  sensors.setResolution(therm4, 10);
  sensors.setResolution(therm5, 10);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.println(DallasTemperature::toFahrenheit(tempC));
  }
}

void loop(void)
{ 
  delay(2000);
  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();
  
  Serial.print("Rachael's temperature is: ");
  printTemperature(therm1);
  printTemperature(therm2);
  printTemperature(therm3);
  printTemperature(therm4);
  printTemperature(therm5);
  Serial.print("\n\r");
}
