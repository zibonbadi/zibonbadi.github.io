//Some constants for defining arbitrary things
#define PULSELEN_MIN 50
#define PULSELEN_MAX 200
#define PIN_BUTTON 2     // the number of the pushbutton pin
#define PIN_POT A0     // the number of the pushbutton pin
#define PIN_LED LED_BUILTIN      // the number of the LED pin
const int MESSAGE_LENGTH = 64;

//Message buffer. This will be treated as ring memory
char msg[MESSAGE_LENGTH];
byte msgIndex = 0;
int pulseLen = PULSELEN_MIN;

// Probing the button
int buttonState = 0; 


void LEDOut(char mCode[4]){

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
  buttonState = digitalRead(PIN_BUTTON);
  pulseLen = map(analogRead(PIN_POT),0,1023,PULSELEN_MIN,PULSELEN_MAX);

  while(Serial.available() > 0){
    char c_tmp =Serial.read();

    //Probe current message state
    if(c_tmp == '?'){
      Serial.print("Echo: ");
      for(int i=0; msg[i] >= ' ' && i<MESSAGE_LENGTH; i++){
        Serial.print(msg[i]);
      }
      Serial.print('\n');
    }
    
    //Discard anything that isn't A-Z or SPACE
    if( (c_tmp >= 'A' && c_tmp <= 'Z') ||c_tmp == ' '){
      msg[msgIndex] = c_tmp;
      msgIndex = (msgIndex+1)%MESSAGE_LENGTH;
    }
    msg[msgIndex] = '\0';
  }

  
  
  // Button LED switch. As the button is LOW-Active, it is probed against LOW
  if (buttonState == LOW) {

    //LED Morse loop
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
