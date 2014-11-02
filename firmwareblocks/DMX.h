// DMX for Relayduino template v0.1
//{{{inclusions}}}
#include <DmxSimple.h>
//{{{definitions}}}
#define DMX_EN 10
#define DMX_PIN 1
#define LED_STRINGS 8
#define DMX_CHANNELS 24
#define DMX_FIRST_CHANNEL 1
//{{{declarations}}}
int lights[LED_STRINGS][7]; 
unsigned int DMXTime = 100;
//{{{setup}}}
	pinMode(DMX_EN, OUTPUT);
	digitalWrite(DMX_EN, LOW);
	DmxSimple.usePin(DMX_PIN);   //set up DMX
	DmxSimple.maxChannel(DMX_CHANNELS);
	for (int i=0;i<LED_STRINGS;i++) { 
		for (int j=0;j<8;j++) {
			lights[i][j]=j/4;   // turn all the lights off. integer division sets the current 
								// values to 1 so that they are reset to 0 on first DMX update.
								// consider replacing this with DMXInit() function.
		}
	}
//{{{loop}}}
	DMXUpdate(false); //update DMX
//{{{commands}}}
			case 'D':  // #D - DMX Functions
				current = peelChar();
				switch (current) 
				{
					case 'E': // #DE - read single value
						DMXE();
						break;
					case 'W': // #DW - write single value
						DMXW();
						break;
					case 'R': // #DR - read all DMX Values
						DMXR();
						break;
					case 'U': // #DU - Force Update all DMX Channels
						DMXUpdate(true);
						break;
					case 'S': // #DS - Read Single DMX Channel
						DMXPrint(peelInt());
						break;
				}
				break;
//{{{functions}}}
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

void DMXUpdate(boolean requested) {
	boolean change = false;  //no changes yet
	boolean channelChange = false;
	int difference;
	for (int i=0;i<LED_STRINGS;i++) 
	{ // for each RGB led string 
		for (int j=0;j<3;j++) 
		{ // for each of the 3 colors
			if (lights[i][j] != lights[i][j+4])
			{ //see if a change is pending
				if (lights[i][3]==0) { //if it's an instant change
					lights[i][j+4] = lights[i][j]; //go ahead and make the instant change
				}
				else { //shit it's a fade... now what?
					difference = (lights[i][j+4]-lights[i][j])/lights[i][3]; //calculate how far to go
					lights[i][j+4]-=difference; // fade the light that far
				}
				DmxSimple.write(i*3+j+DMX_CHANNEL, lights[i][j+4]); //send it out over DMX
				change = true; //annotate that a change was made
				channelChange = true; 
			}
		}
		if (channelChange==true)
		{
			if (lights[i][3]!=0) 
			{
				lights[i][3]-=1; //decrement timer for this channel
			}
			//DMXPrint(i);
			channelChange=false;
		}
	}
	if (requested or change) 
	{ //if user requested or a change happened
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