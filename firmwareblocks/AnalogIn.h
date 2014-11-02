// AnalogIn template v0.1
//{{{inclusions}}}
//{{{definitions}}}
#define ANIN1 0   // Analog 1 is connected to Arduino Analog In 0
#define ANIN2 1   // Analog 2 is connected to Arduino Analog In 1
#define ANIN3 2   // Analog 3 is connected to Arduino Analog In 2
#define ANIN4 3   // Analog 4 is connected to Arduino Analog In 3
#define ANIN5 4   // Analog 5 is connected to Arduino Analog In 4
#define ANIN6 5   // Analog 6 is connected to Arduino Analog In 5
#define ANIN7 6   // Analog 7 is connected to Arduino Analog In 6
#define ANIN8 7   // Analog 8 is connected to Arduino Analog In 7
//{{{declarations}}}
int analogPin[8] = {ANIN1, ANIN2, ANIN3, ANIN4, ANIN5,ANIN6, ANIN7, ANIN8};
//{{{setup}}}
//{{{loop}}}
//{{{commands}}}
			case 'A':
				current = peelChar();
				switch (current) 
				{
					case 'R': // #AR - Read Single Analog Channel
						Serial.println(String(analogRead(analogPin[peelInt()])));
				}
				break;
//{{{functions}}}