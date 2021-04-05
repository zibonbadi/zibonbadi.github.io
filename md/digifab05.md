% Microcontroller programming using Arduino

[^dlsrc]: [Morse code transmitter source code](https://github.com/zibonbadi/zibonbadi.github.io/blob/main/src/morsecode.c/morsecode.c.ino)
[^dlvid]: [Video of me showcasing the morse code emitter](download/transmitter.webm)

## Introduction

For this week, I decided to write a simple morse code emitter, as it is an easy test case for using the I/O pins, LED and serial bus. The intent is to write a message into the Arduino over Serial and to make the LED blink accordingly once the message has been confirmed using the button.

As usual, all software resources (as in the *one* source file) are available for download [^dlsrc] and a video of the slightly extended version can be found [in the next week's project documentation](html/digifab06.html).

![Video showing the morse code emitter over serial. If your browser does not support video playback, Download it instead.[^dlvid]](download/transmitter.webm)

## Setting up the Arduino

For this excercise I've been supplied with the following hardware:

|Hardware piece|Count|
|--|--|
|Arduino Uno A7| 1|
|Breadboard| 1|
|USB cable type A &rarr; type C| 1|
|Resistor 10k&omega;| 3|
|Ultrasonic sensor| 1|
|Humidity sensor| 1|
|Motion sensor| 1|
|Presence sensor| 1|
|Beeper| 1|
|Controllable LED light| 1|
|Potentiometer| 1|
|Cable; double male| several|
|5 cable band; double female| 2|
|2 cable band; double male| 1|
|2 cable band; male/female| 2|
|Button| 1|

For this project I wired up a button to pin D2 and ground to use as a transmisison button (as shown in the image below). While I was at it, I also used the opportunity to test out an older self-built Arduino I had lying around, just to test whether I'd be able to use it after returning the University one. Turns out it works just as a regular Arduino Uno:

![Wiring for the morse code emitter](img/lesson5/morsecode_wires.jpg)

## Programming

First step was to install the Arduino into your computer. This might range from self-explanatory to something you should already know how to do, depending on your choice of operating system and programming method.

For simplicity's sake I used Windows and the official Arduino IDE for programming, however since I'm a regular Arch Linux user ~~(alright Manjaro, I'll admit it ;P)~~ I will outline some common alternatives shortly:

Thankfully, as it is Java-based the official Arduino IDE is available in most repositories. However you might notice that upon startup your newly plugged-in Arduino might not get recognized. Usually this ends up being an issue of read/write permissions on behalf of the your kernel. The solution is to give yourself read/write permissions to the Arduino's serial device, either by using `ls -l /dev` and adding yourself to the appropriate user group or by using `chmod` to change file access permissions. The serial device to look for is usually either `/dev/ttyACM0` (official Arduino) or `/dev/ttyUSB0` (selfbuilt Arduino) 

If you feel like diving into advanced microcontroller programming or you just prefer Vim, you can also use the Arduino AVR tools with your trusty old GCC. I'm only gonna mention it here as I haven't yet found the time to configure it.

Lastly you might run into issues when accessing Serial on Arch-based distributions. After reading around in the forums, this currently seems to be a bug and it is suggested to do a workaround using regular terminal tools such as `screen` or `tail -f`.


As you will be able to see in the program's source code later on, I decided to add a buffer probing function using the letter '?' for Serial IO:

![Serial IO for the morse code emitter program](img/lesson5/morsecode_serial.png)

Below you can find the entire program's source code written in the Arduino implementation of the C programming language the entire code can be downloaded as well[^dlsrc].

```C
//Some constants for defining arbitrary things
const int MESSAGE_LENGTH = 64;
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin
const int pulseLen = 50;

//Message buffer. This will be treated as ring memory
char msg[MESSAGE_LENGTH];
byte msgIndex = 0;

// Probing the button
int buttonState = 0; 


void LEDOut(char mCode[4]){

  for(int i=0; (mCode[i] == '.' || mCode[i] == '-') && i<4 ;i++){
    digitalWrite(ledPin, HIGH);
    delay( (mCode[i] == '.')?pulseLen:3*pulseLen);
    digitalWrite(ledPin, LOW);
    delay(pulseLen);
  } 
}

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
    digitalWrite(ledPin, LOW);
    msgIndex = 0;
    msg[0] = '\0';
  }
  
}
```
