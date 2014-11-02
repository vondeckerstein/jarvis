// Dallas Temperature Template v0.1
//{{{inclusions}}}
#include <OneWire.h>
#include <DallasTemperature.h>
//{{{definitions}}}
#define ONE_WIRE_BUS 7
//{{{declarations}}}
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
DeviceAddress therm[5] = { {0x28, 0x65, 0x7B, 0xCF, 0x05, 0x00, 0x00, 0xBD },
                          { 0x28, 0x68, 0xD6, 0xCF, 0x05, 0x00, 0x00, 0x28 },
                          { 0x28, 0x7C, 0xAF, 0xCF, 0x05, 0x00, 0x00, 0x31 },
                          { 0x28, 0xC6, 0x66, 0xCF, 0x05, 0x00, 0x00, 0x28 },
                          { 0x28, 0x91, 0x5F, 0xCF, 0x05, 0x00, 0x00, 0xF4 } };
//{{{setup}}}
	sensors.begin();
	for (int i=0;i<sizeof(therm)/8;i++){
		sensors.setResolution(therm[i], 10);
		debug("setting resolution for sensor:");
		debug(i);
	}
	sensors.requestTemperatures();
	delay(50);
	sensors.requestTemperatures();
	delay(50);
	for (int i=0;i<sizeof(therm)/8;i++){
		debug(sensors.getTempC(therm[i]));
	}
//{{{loop}}}
//{{{commands}}}
			case 'T':  // #T - Temperature Functions
				current = peelChar();
				switch (current) { //break out to respective command type functions
					case 'R': // #TR - Read Single Temperature Sensor
						Serial.println(readThermometer(peelInt()));
						break;
				}
				break;
//{{{functions}}}
float readThermometer(int devId)
{
	debug(devId);
	debug(therm[devId]);
	float tempC = sensors.getTempC(therm[devId]);
	debug("temperature:");
	debug(tempC);
	return tempC;
}
void debug(DeviceAddress d)
{
	Serial.print("R=");
	for( int i = 0; i < 8; i++) {
		Serial.print(d[i], HEX);
		Serial.print(" ");
	}
}