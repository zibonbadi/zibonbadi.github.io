% Final Project: Creating a Mini PC case

[^dlcad]: [FreeCAD project](../src/lcddisplay/lcddisplay.ino)
[^dldxf]: [DXF sketch of the case walls](../src/lcddisplay/lcddisplay.ino)
[^dlgcode]: [GCode export of the corner pieces](../src/lcddisplay/lcddisplay.ino)
[^dlcode]: [Arduino project](../src/lcddisplay/lcddisplay.ino)
[^dlsrc]: [LCD test source code](../src/lcddisplay/lcddisplay.ino)

## Inspiration

Alright, this is it. The final project. Countless possibilities when you can control everything from physical construction over electronics to microcontroller programming. But what do?

When I started this course, I did so with the intention of passing it having gained not only knowledge, but also a holding a physical object in my hands which I could use on a day-to-day basis.

I previously mentioned being a proud Linux user, a system that to this day gained a dedicated, but niche following among desktop computer users, mainly through word of mouth. Because of this, there are many reasons someone might "get into" Linux in the first place.

For me this reason was preserving old technology. As me and my family never had a lot of spare cash, I've learned to cherish any piece of precious (expensive) technology I manage to get hold of and use it until it's absolutely not possible anymore, then fix it, then use it again, 20 GOTO 10. However, when I first installed Manjaro on my laptop I encountered two problems which Linux wasn't able to fix. Overheating (which can be mitigated, thanks to `thermald` and I haven't had a thermal shutdown since ;p) and a case which was slowly falling apart.

Thus I decided to finally address these problems myself by building a custom case which warned me of overheating during runtime and allowed me to easily work on my laptop's old mainboard if needed.

## Design goals and parts list

The case I am trying to build will have to meet certain criteria, both for passing the course as well as for my own personal use case:

- The case requires fitting the following components inside
		  - Mainboard
		  - Power button
		  - Audio/SD I/O PCB
		  - Activity light PCB
- The optical drive is optional but "nice to have"
- A custom backplate suited towards the laptop's I/O
- A feet-based "floating" design to increase airflow at the bottom and reduce overheating
- Strict separation between the case's electronics and the laptop's components
- An easy way to remove the laptop's PCB and service it without taking apart the entire case
- Ambient lighting at the bottom of the case to indicate activity in an aesthetically thought-out way.
- A heat sensor for measuring the CPU temperature
- A way to display the measured CPU temperature
- A microcontroller for interfacing between the above mentioned components

The final parts required to build this case ended up being the following:

- My Laptop. Duh.
- 3mm wood for the laser-cut 
- A roll of PLA for 3D-printing the stabilizing corner pieces
- 2mm screws
- 2mm-compatible feet
- Cables and solder for connecting the electronics
- An Arduino Uno. I used a selfbuilt clone in the final case, but tested on an official one.
- An MLX90614ESF infrared temperature sensor
- A WS2812B-based programmable LED strip
- A 1602A-compatible LCD character display
- A Potentiometer and Button for controlling the LCD display's properties (contrast, backlight)
- An LCD display for displaying the measured CPU temperature

## Designing the case

## Laser cutting

After exporting the case walls' sketch into DXF format, I prepared it in Rhinoceros using the following settings:

## 3D printing

I prepared the corner piece models in Cura using the following settings:

## Electronics wiring

As almost everything electronic is gonna be driven by the Arduino Uno, here's a table with how to wire it up. Additional wirings are written below in text form:

| Arduino pin | Component pin |
|---|---|
| +5V | 1602A VSS; 1602A A (over resistor); Potentiometer <++> |
| GND | 1602A VDD; 1602A RW; 1602A K; Potentiometer; MLX90614ESF GND |
| +3.3V | MLX90614ESF VIN |
| A4 | MLX90614ESF SDA |
| A5 | MLX90614ESF SCL |
| Digital 2 | 1602A D7 |
| Digital 3 | 1602A D6 |
| Digital 4 | 1602A D5 |
| Digital 5 | 1602A D4 |
| Digital 7 | WS2812B (over resistor) |
| Digital 11 | 1602A E |
| Digital 12 | 1602A RS |

Additionally I wired the potentiometer's output pin to the 1602A's VO (contrast) pin.

## Microcontroller programming

As the parts used are fairly common among Arduino hobbyists, the source code ended up rather simple, given you install the `LiquidCrystal`, `FastLED` and `Adafruit MLX90614` libraries:

```C
// include the library code:
#include <LiquidCrystal.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <FastLED.h>

#define LED_PIN 7
#define LED_COUNT 10

//Define custom characters
byte temp[8] = {
0b00100,
0b01100,
0b01100,
0b01100,
0b01100,
0b10110,
0b11110,
0b01100
};
byte fan[8] = {
0b00000,
0b00110,
0b01001,
0b00110,
0b01001,
0b00001,
0b11110,
0b00000
};

byte ambient[8] = {
0b00000,
0b00000,
0b01110,
0b10001,
0b10101,
0b10001,
0b01110,
0b00000
};

byte fire[8] = {
0b00001,
0b10100,
0b01100,
0b01110,
0b11011,
0b10001,
0b11001,
0b01111
};


// Init LCD display. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Init MLX temp sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//Init LED strip
CRGB led[LED_COUNT];

unsigned int milicounter = 0;

void setup() 
{
  // display dimensions (16x2)
  lcd.begin(16, 2);
  mlx.begin();
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, LED_COUNT);
  
  // dejunkification
  lcd.clear();

  //load custom characters into the LCD's RAM
  lcd.createChar(0, temp);
  lcd.createChar(1, ambient);
  lcd.createChar(2, fire);
  lcd.createChar(3, fan);
}

void loop() 
{
  milicounter = (milicounter+1)%1000;
  float objTemp = mlx.readObjectTempC();
  float ambientTemp = mlx.readAmbientTempC();
  
  byte r=255,g=255,b=255;

  //Color calc
  if(objTemp >= 40.0f){ r=0; g=255; b=255; }
  if(objTemp >= 50.0f){ r=0; g=127; b=255; }
  if(objTemp >= 60.0f){ r=255; g=127; b=0; }
  if(objTemp >= 70.0f){ r=255; g=63; b=0; }
  if(objTemp >= 80.0f){ r=255; g=0; b=0; }

  /*
   *  Output
   */

  //Buffer clear
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  //Icons
  //temp
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(objTemp);
  //ambient
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.print(ambientTemp);
  //fire
  lcd.setCursor(14-4, 0);
  lcd.print("1000 ");
  lcd.write(byte(3));

  //Display LED colors
  for(byte i=0; i<LED_COUNT;i++){
      led[i]=CRGB(r,g,b);
  }
  FastLED.show();
}
```
