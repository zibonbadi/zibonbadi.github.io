% Analog input on the Aruino

[^dlsrc]: [Updated morse code transmitter source code](../src/morsecode_analog.c/morsecode_analog.c.ino)
[^dlvid]: [Video of me showcasing the morse code emitter](../download/transmitter.webm)

## Updating the morse code emitter

This week I simply decided to test out some analog input using my morse code emitter code and a potentiometer.

The change was fairly simple: I added 2 more constants for defining a delay window and used the `map()` function to map the analog inputs default range of 0 to 1023 onto it. Afterwards I added a delay update at the beginning of the event loop. This way the potentiometer can be used to control the emitter's baud rate.

As a refactoring measure to save on memory, I also decided to relabel all of my constants, except the message buffer size into `#define`s.

Last thing to mention was wiring up the potentiometer to the board. This was fairly simple. The middle pin transmits the signal, which I wired to A0. On this model the right pin (pins facing you) was +5V and the left one was GND, however as long as the middle pin is wired to the Arduino's analog input and naturally voltage/currents are acceptable, it shouldn't matter too much which way you wire it.

Lastly, just like last week, I wired the Button to digital pin 2 accordingly to the ports above the analog inputs.

As always, the source code is available for download[^dlsrc] and below you can see a video of the working (updated) mose code emitter, which is availavle for download as well[^dlvid]:

![Video showing the morse code emitter over serial. If your browser does not support video playback, Download it instead.[^dlvid]](../download/transmitter.webm)

## Updated Morse code emitter source code.

```C
//Some constants for defining arbitrary things
#define PULSELEN_MIN 50
#define PULSELEN_MAX 200
#define PIN_BUTTON 2     // the number of the pushbutton pin
#define PIN_POT A0     // the number of the potentiometer input pin
#define PIN_LED LED_BUILTIN      // the number of the LED pin
const int MESSAGE_LENGTH = 64;

//Message buffer. This will be treated as ring memory
char msg[MESSAGE_LENGTH];
byte msgIndex = 0;
int pulseLen = PULSELEN_MIN;

// Probing the button
int buttonState = 0; 


void LEDOut(char mCode[4]){

	//Helper function to translalte morse code into blink patterns. '.' is a short and '-' a long pulse (3x length of short pulse)

  for(int i=0; (mCode[i] == '.' || mCode[i] == '-') && i<4 ;i++){
    digitalWrite(PIN_LED, HIGH);
    delay( (mCode[i] == '.')?pulseLen:3*pulseLen);
    digitalWrite(PIN_LED, LOW);
    delay(pulseLen);
  } 
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(PIN_LED, OUTPUT);
  // Pullup for actuation precision
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  //Initialise serial. 9600 Baud just to be safe
  Serial.begin(9600);
}

void loop() {
	//Probe button for message trigger
  buttonState = digitalRead(PIN_BUTTON);
	//Determine pulse length. This is basically the inverse of a baud speed and will be used in the output's delay() function
  pulseLen = map(analogRead(PIN_POT),0,1023,PULSELEN_MIN,PULSELEN_MAX);

	//Check for available characters over serial
  while(Serial.available() > 0){
    char c_tmp =Serial.read();

    //Probe current message state by entering '?' into serial
    if(c_tmp == '?'){
      Serial.print("Echo: ");
		//When a control character is recognized, text entry is aborted. NUL/'\0' is used for marking a premature end of input
      for(int i=0; msg[i] >= ' ' && i<MESSAGE_LENGTH; i++){
        Serial.print(msg[i]);
      }
      Serial.print('\n');
    }
    
    //Discard anything that isn't A-Z or SPACE
    if( (c_tmp >= 'A' && c_tmp <= 'Z') || c_tmp == ' '){
      msg[msgIndex] = c_tmp;
      msgIndex = (msgIndex+1)%MESSAGE_LENGTH;
    }
		  //Mark new end of line
    msg[msgIndex] = '\0';
  }

  
  
  // Button LED switch. As the button is LOW-Active, it is probed against LOW
  if (buttonState == LOW) {

    /*
		  LED Morse loop. Basically parses the characters into blink patterns using the LEDOut() function
		  Flushes the entire message buffer into output at a constant baud rate
	*/
    for(int i=0; msg[i] >= 'A' && i<MESSAGE_LENGTH; i++){
        switch(msg[i]){
          case 'A':{
            LEDOut(".-");
            break;
          }
          case 'B':{
            LEDOut("-...");
            break;
          }
          case 'C':{
            LEDOut("-.-.");
            break;
          }
          case 'D':{
            LEDOut("-..");
            break;
          }
          case 'E':{
            LEDOut(".");
            break;
          }
          case 'F':{
            LEDOut("..-.");
            break;
          }
          case 'G':{
            LEDOut("--.");
            break;
          }
          case 'H':{
            LEDOut("....");
            break;
          }
          case 'I':{
            LEDOut("..");
            break;
          }
          case 'J':{
            LEDOut(".---");
            break;
          }
          case 'K':{
            LEDOut("-.-");
            break;
          }
          case 'L':{
            LEDOut(".-..");
            break;
          }
          case 'M':{
            LEDOut("--");
            break;
          }
          case 'N':{
            LEDOut("-.");
            break;
          }
          case 'O':{
            LEDOut("---");
            break;
          }
          case 'P':{
            LEDOut(".--.");
            break;
          }
          case 'Q':{
            LEDOut("--.-");
            break;
          }
          case 'R':{
            LEDOut(".-.");
            break;
          }
          case 'S':{
            LEDOut("...");
            break;
          }
          case 'T':{
            LEDOut("-");
            break;
          }
          case 'U':{
            LEDOut("..-");
            break;
          }
          case 'V':{
            LEDOut("...-");
            break;
          }
          case 'W':{
            LEDOut(".--");
            break;
          }
          case 'X':{
            LEDOut("-..-");
            break;
          }
          case 'Y':{
            LEDOut("-.--");
            break;
          }
          case 'Z':{
            LEDOut("--..");
            break;
          }
          default: break;
        }
    }
    
    // Reset LED and Buffer
    digitalWrite(PIN_LED, LOW);
    msgIndex = 0;
    msg[0] = '\0';
  }
  
}
```
