//Some constants for defining arbitrary things
const int MESSAGE_LENGTH = 64;
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin

//Message buffer. This will be treated as ring memory
char msg[MESSAGE_LENGTH];
byte msgIndex = 0;

// Probing the button
int buttonState = 0; 

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // Pullup for actuation precision
  pinMode(buttonPin, INPUT_PULLUP);
  //Initialise serial. 9600 Baud just to be safe
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  while(Serial.available() > 0){
    char c_tmp =Serial.read();

    //Discard anything that isn't A-Z
    if(c_tmp >='A' && c_tmp <= 'Z'){
      msg[msgIndex] = c_tmp;
      msgIndex = (msgIndex+1)%MESSAGE_LENGTH;
    }
    msg[msgIndex] = '\0';
  }

  /*
  for(int i=0; msg[i] =! '\0' || i<MESSAGE_LENGTH; i++){
    Serial.print(
  }
  */
  
  // Button LED switch. Thus far a stub
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }

  //Stubby debug output
  Serial.print("Echo: ");
  Serial.println(msg[0]);
  
}
