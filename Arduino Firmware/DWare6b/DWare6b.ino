#include <DmxSimple.h>
#include <string.h>
#include <ctype.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <MemoryFree.h>
#include <DallasTemperature.h>

#define DEBUG true
#define LED_PIN 13 // Arduino Built in LED
#define DMX_EN 10
#define DMX_PIN 1
#define ONE_WIRE_BUS 7
#define LED_STRINGS 8
#define DMX_CHANNELS 24 //number of channels
#define DMX_CHANNEL 1	//first channel number
#define DMX_TIME 100
#define LOOP_TIME 100

#define ANIN1 0   // Analog 1 is connected to Arduino Analog In 0
#define ANIN2 1   // Analog 2 is connected to Arduino Analog In 1
#define ANIN3 2   // Analog 3 is connected to Arduino Analog In 2
#define ANIN4 3   // Analog 4 is connected to Arduino Analog In 3
#define ANIN5 4   // Analog 5 is connected to Arduino Analog In 4
#define ANIN6 5   // Analog 6 is connected to Arduino Analog In 5
#define ANIN7 6   // Analog 7 is connected to Arduino Analog In 6
#define ANIN8 7   // Analog 8 is connected to Arduino Analog In 7

#define REL1 2  // Relay 1 is connected to Arduino Digital 2
#define REL2 3  // Relay 2 is connected to Arduino Digital 3 PWM
#define REL3 4  // Relay 3 is connected to Arduino Digital 4
#define REL4 5  // Relay 4 is connected to Arduino Digital 5 PWM
#define REL5 6  // Relay 5 is connected to Arduino Digital 6 PWM
#define REL6 7  // Relay 6 is connected to Arduino Digital 7
#define REL7 8  // Relay 7 is connected to Arduino Digital 8
#define REL8 9  // Relay 8 is connected to Arduino Digital 9 PWM

#define REL_SAVE 1
#define BAUD_SAVE 2
#define LOOP_TIME_SAVE 3
#define DMX_TIME_SAVE 5
#define ID_SAVE 7

int relayPin[8] = {REL1, REL2, REL3, REL4, REL5, REL6, REL7, REL8}; // pin addresses
int analogPin[8] = {ANIN1, ANIN2, ANIN3, ANIN4, ANIN5,ANIN6, ANIN7, ANIN8};
boolean relay[8] = {false, false, false, false, false, false, false, false}; // initial states
DeviceAddress therm[5] = { {0x28, 0x65, 0x7B, 0xCF, 0x05, 0x00, 0x00, 0xBD },
                          { 0x28, 0x68, 0xD6, 0xCF, 0x05, 0x00, 0x00, 0x28 },
                          { 0x28, 0x7C, 0xAF, 0xCF, 0x05, 0x00, 0x00, 0x31 },
                          { 0x28, 0xC6, 0x66, 0xCF, 0x05, 0x00, 0x00, 0x28 },
                          { 0x28, 0x91, 0x5F, 0xCF, 0x05, 0x00, 0x00, 0xF4 } };
                          
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int lights[LED_STRINGS][7]; 
String msg = "";
char Unitbaud;
unsigned int loopTime = 100;
unsigned int DMXTime = 1000;



void setup() {
        
	pinMode(LED_PIN, OUTPUT);
	pinMode(DMX_EN, OUTPUT);
	digitalWrite(DMX_EN, LOW);
	Unitbaud = EEPROM.read(BAUD_SAVE);
	setBaud(Unitbaud);
        delay(100);
        debug("setup()");
	for (int i=0;i<8;i++)            // set up all relay pins
	{
		pinMode(relayPin[i], OUTPUT);  // declare the relay pin as an output
		analogWrite(relayPin[i], LOW);
	}  
  
	DmxSimple.usePin(DMX_PIN);   //set up DMX
	DmxSimple.maxChannel(DMX_CHANNELS);
        for (int i=0;i<5;i++){
          sensors.setResolution(therm[i], 10);
        }

	//relL();    //load saved relay positions 
	for (int i=0;i<LED_STRINGS;i++) { 
		for (int j=0;j<8;j++) {
			lights[i][j]=j/4;   // turn all the lights off. integer division sets the current 
							  // values to 1 so that they are reset to 0 on first DMX update.
							  // consider replacing this with DMXInit() function.
		}
	}
	delay(1000);
        debug(" done with setup()");
        debug(getFreeMemory());

}

void loop() {
  Serial.println("looping...");
  readSerial(); //get command data
  
  if (msg.length()>0) { //if there's command data, process it
    processCommand();
  }
  DMXUpdate(false); //update DMX
  delay(loopTime); //kill 100 milliseconds
}

void readSerial() {
  byte ch; // the character that you use to construct the Message 


  if(Serial.available())// checks if there is a new message;
  {
    while(Serial.available())// while the message did not finish
    {
      ch=Serial.read();// get the character
      msg+=(char)ch;//add the character to the message
      delay(1);//wait for the next character
    }
  }
  else
  {
    debug("serial data not available.");
  }
}

void processCommand() {
	char current = peelChar();
	if (current == '#') {
                Serial.println("found '#'");
		current = peelChar();
		switch (current) {
			case 'D':  // #D - DMX Functions
				current = peelChar();
				switch (current) {
					case 'E': // #DE - read single value
						DMXE();
						break;
					case 'W': // #DW - write single value
						DMXW();
						break;
					case 'A': // #DA 
						DMXA();
						break;
					case 'R': // #DR
						DMXR();
						break;
					case 'U': // #DU
						DMXUpdate(true);
						break;
					case 'S': // #DS
						DMXPrint(peelInt());
						break;
				}
				break;
			case 'R':  // #R - Relay Functions
				current = peelChar();
				switch (current) { 
					case 'E': // #RE
						relE();
						break;
					case 'W': // #RW
						relW();
						break;
					case 'R': // #RR
						relR();
						break;
					case 'L': // #RL
						relL();
						break;
					case 'S': // #RS
						relS();
						break;
					}
				break;
			case 'A':
				current = peelChar();
				switch (current)
					case 'R': // #AR
						Serial.println(String(analogRead(analogPin[peelInt()])));
				break;
			case 'U':
				current = peelChar();
				switch (current) { //break out to respective command type functions
					case 'B': // #UB
						saveBaud(peelChar());
						break;
					case 'A': // #UA
						Blink();
						break;
					case 'T': // #UT
						loopTime=peelInt();
						DMXTime=peelInt();
						break;
					case 'S': // #US
						saveTimes();
						break;
					case 'L': // #UL
						loadTimes();
						break;
					case 'I': // #UI
						EEPROM.write(ID_SAVE, peelChar());
						break;
					case 'C': // #UC
						Serial.println(char(EEPROM.read(ID_SAVE)));
						break;
					case 'M': // #UM
						Serial.println(getFreeMemory());
						break;
				}
				break;
                        case 'T':  // #T - Temperature Functions
				current = peelChar();
                                switch (current) { //break out to respective command type functions
					case 'R': // #TR 
						Serial.println(readThermometer(peelChar()));
						break;
                                }
                                break;
			default:
				Serial.println("Invalid Command.");
			}
		} 
	else {
		Serial.println("No '#', not processing as command");
		Serial.println(msg);
	}

}

char peelChar() {
  char current = msg.charAt(0);
  msg = msg.substring(1);
  return current;
}

int peelInt() {
  int peeled = msg.toInt();
  msg = msg.substring(msg.indexOf('.') + 1);
  return peeled;
}

void Blink() {
  for (int i=0;i<5;i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void DMXR() {
  //Serial.println("DMX Read...");
  for (int i=0;i<LED_STRINGS;i++) {
    DMXPrint(i);
    //Serial.println("");
  }
}

void DMXE() {
  int channel=peelInt();
  for (int i=0;i<7;i++) {
    Serial.println(lights[channel][i]);
  }
}

void DMXW() {
  //Serial.println("DMX Write...");
  int channel= peelInt();
  if (0 <= channel < LED_STRINGS){
    for (int i=0;i<4;i++) {
      lights[channel][i] = peelInt();
    }
  }
  else Serial.println("Channel Out Of Range.");
}

void DMXA() {
}

void DMXUpdate(boolean requested) {
  boolean change = false;  //no changes yet
  boolean channelChange = false;
  int difference;
  for (int i=0;i<LED_STRINGS;i++) { // for each RGB led string 
    for (int j=0;j<3;j++) { // for each of the 3 colors
      if (lights[i][j] != lights[i][j+4]){ //see if a change is pending
        if (lights[i][3]==0) { //if it's an instant change
          lights[i][j+4] = lights[i][j]; //go ahead and make the instant change
        }
        else { //shit it's a fade... now what?
          difference = (lights[i][j+4]-lights[i][j])/lights[i][3]; //calculate how far to go
          lights[i][j+4]-=difference; // fade the light that far
        }
        DmxSimple.write(i*3+j+DMX_CHANNEL, lights[i][j+4]); //send it out over DMX
		//Serial.print("I1: ");
		//Serial.print(i*3+j);
		//Serial.print("I2: ");
		//Serial.println(lights[i][j+4]);
        change = true; //annotate that a change was made
		channelChange = true; 
      }
    }
    if (channelChange==true) {
      if (lights[i][3]!=0) {
        lights[i][3]-=1; //decrement timer for this channel
      }
      //DMXPrint(i);
      channelChange=false;
    }
  }
  if (requested or change) { //if user requested or a change happened
    Serial.end();            //switch from serial over to DMX for a while
    digitalWrite(DMX_EN, HIGH);
    delay(DMXTime);
    digitalWrite(DMX_EN, LOW);
    setBaud(Unitbaud); // restart the serial connection
	}
}

void DMXPrint(int chan) {
  Serial.print("Channel: ");
  Serial.print(chan);
  String location[] = { 
    "R1", "G1", "B1", "T", "R2", "G2", "B2"  };
  for (int i=0;i<7;i++) {
    Serial.print(" " + location[i] + ": ");
    Serial.print(lights[chan][i]);
  } 
  Serial.println();
}

void relR() {
  for (int i=0;i<8;i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(relay[i]);
    Serial.print(" | ");
  }
  Serial.println("");
}

void relE() {
  Serial.print(relay[peelInt()]);
}

void relW() {
  int address = peelInt();
  int action = peelInt();
  if (action==0) {
    relay[address] = false;
  }
  else if (action==1){
    relay[address] = true;
  }
  else {
    relay[address] = !relay[address];
  }
  if (relay[address]) {
		digitalWrite(relayPin[address], HIGH);
	}
	else {
		digitalWrite(relayPin[address], LOW);
	}
}

void relS() {
  byte val=0;
  for (int i=0;i<8;i++) {
    val *= 2;
    val += relay[i];
  }
  EEPROM.write(REL_SAVE, val);
  Serial.print("Saved.");
  Serial.println(val);
}

void relL() {
  byte val=EEPROM.read(REL_SAVE);
  boolean remainder;
  //Serial.print("reading  byte:");
  //Serial.println(val);
  
  for (int i=7;i>-1;i--) {
    remainder=val%2;
    val/=2;
    relay[i]= remainder;
    analogWrite(relayPin[i], remainder);
    
  }
  relR();
}

void setBaud(char myBaud) {
   switch (myBaud)
   {
    case 'A' : Serial.begin(1200);
      break;
    case 'B' : Serial.begin(2400);
      break;     
    case 'C' : Serial.begin(4800);
      break;
    case 'D' : Serial.begin(9600);
      break;
    case 'E' : Serial.begin(14400);
      break;
    case 'F' : Serial.begin(19200);
      break;
    case 'G' : Serial.begin(28800);
      break;
    case 'H' : Serial.begin(38400);
      break;
    case 'I' : Serial.begin(57600);
      break;
    case 'J' : Serial.begin(115200);
      break;
    default:  Serial.begin(9600);
      break;
   }
}

void saveBaud(char myBaud) {
	EEPROM.write(BAUD_SAVE, myBaud);
	Serial.print("Baud Saved to EEPROM:");
	Serial.println(myBaud);
}

void saveTimes() {
	byte a;
	byte b;
	a = *((byte*)(&loopTime)); // Grab char from first byte of the pointer to the int
	b = *((byte*)(&loopTime) + 1); // Grab char from next byte of the pointer to the int
	EEPROM.write(LOOP_TIME_SAVE, a);
	EEPROM.write(LOOP_TIME_SAVE + 1, b);
	a = *((byte*)(&DMXTime)); // Grab char from first byte of the pointer to the int
	b = *((byte*)(&DMXTime) + 1); // Grab char from next byte of the pointer to the int
	EEPROM.write(DMX_TIME_SAVE, a);
	EEPROM.write(DMX_TIME_SAVE + 1, b);
}

void loadTimes() {

	unsigned int time1;
	unsigned int time2;
	byte a;
	byte b;
	a = EEPROM.read(LOOP_TIME_SAVE);
	b = EEPROM.read(LOOP_TIME_SAVE + 1);
	time1= a*256 + b;
	a = EEPROM.read(DMX_TIME_SAVE);
	b = EEPROM.read(DMX_TIME_SAVE + 1);
	time1= a + b*256;
	Serial.println(time1);
	Serial.println(time2);
	
}

float readThermometer(int devId)
{
  float tempC = sensors.getTempC(therm[devId]);
  return tempC;
}

void debug(String s)
{
  if (DEBUG) {
    Serial.println(s);
  }
}
