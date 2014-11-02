// master template v0.1
//{{{inclusions}}}
#include <EEPROM.h>
#include <string.h>
#include <ctype.h>
#include <MemoryFree.h>
//{{{definitions}}}
#define DEBUG false
#define BAUD_SAVE 2
#define LOOP_TIME_SAVE
#define ID_SAVE 7
//{{{declarations}}}
char Unitbaud;
unsigned int loopTime = 1000;
//{{{setup header}}}
String msg = "";
void setup() {
	Unitbaud = EEPROM.read(BAUD_SAVE);
	setBaud(Unitbaud);
	delay(100);
	debug("setup()");
//{{{setup}}}
//{{{setup footer}}}
	delay(1000);
	debug(" done with setup(), device free memory:");
	debug(getFreeMemory());
}
//{{{loop header}}}
void loop() {
	debug("looping...");
	debug("free memory:");
	debug(getFreeMemory());
	readSerial(); 
	if (msg.length()>0) { 
		debug("processing command");
		processCommand();
	}
//{{{loop}}}
//{{{loop footer}}}
}
//{{{commands header}}}
void processCommand() 
	{
	char current = peelChar();
	if (current == '#') 
		{
		debug("found '#'");
		current = peelChar();
		switch (current) 
			{
//{{{commands}}}
			case 'U':
				current = peelChar();
				switch (current) 
				{ //break out to respective command type functions
					case 'B': // #UB - Save new baud rate
						saveBaud(peelChar());
						break;
					case 'T': // #UT - Write Loop Time
						loopTime=peelInt();
						break;
					case 'S': // #US - Save Current Time to EEPROM
						saveTime();
						break;
					case 'L': // #UL - Load Loop Time from EEPROM
						loadTime();
						break;
					case 'I': // #UI - Set Device ID
						EEPROM.write(ID_SAVE, peelChar());
						break;
					case 'C': // #UC - Read Device ID
						Serial.println(char(EEPROM.read(ID_SAVE)));
						break;
					case 'M': // #UM - Check available memory
						Serial.println(getFreeMemory());
						break;
					case 'Q': // #UQ - Communications check
						Serial.println("Test Message");
						break;
					case 'N': // #UN - Get Device Name
						Serial.println(UNIT_NAME);
						break;
				}
				break;
//{{{commands footer}}}
			default:
				Serial.println("Invalid Command.");
			}
		} 
	else {
		Serial.println(current);
		Serial.println("No '#', not processing as command");
		Serial.println(msg);
	}
}
//{{{functions}}}
void readSerial() {
	byte ch; // the character that you use to construct the Message 
	debug("reading serial...");
	if(Serial.available())// checks if there is a new message;
	{
		while(Serial.available())// while the message did not finish
		{
			ch=Serial.read();// get the character
			debug((char)ch);
			msg = msg + (char)ch;//add the ch aracter to the message
			debug(msg);
			delay(1);//wait for the next character
		}
		debug("done reading message");
	}
	else
	{
		debug("serial data not available.");
	}
}
char peelChar() {
  debug("peeling char from string:");
  debug(msg);
  char current = msg.charAt(0);
  msg = msg.substring(1);
  debug("found char:");
  debug(current);
  return current;
}
int peelInt() {
  int peeled = msg.toInt();
  msg = msg.substring(msg.indexOf('.') + 1);
  return peeled;
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
void saveTime() {
	byte a;
	byte b;
	a = *((byte*)(&loopTime)); // Grab char from first byte of the pointer to the int
	b = *((byte*)(&loopTime) + 1); // Grab char from next byte of the pointer to the int
	EEPROM.write(LOOP_TIME_SAVE, a);
	EEPROM.write(LOOP_TIME_SAVE + 1, b);
}

void loadTime() {
	unsigned int time1;
	byte a;
	byte b;
	a = EEPROM.read(LOOP_TIME_SAVE);
	b = EEPROM.read(LOOP_TIME_SAVE + 1);
	time1= a*256 + b;
	Serial.println(time1);
}
void debug(String s)
{
	if (DEBUG) {
		Serial.println(s);
	}
}
void debug(float s)
{
	if (DEBUG) {
		Serial.println(s);
	}
}
void debug(int s)
{
	if (DEBUG) {
		Serial.println(s);
	}
}
void debug(char s)
{
	if (DEBUG) {
		Serial.println(s);
	}
}