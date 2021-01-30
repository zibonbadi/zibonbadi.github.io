% Final Project: Creating a Mini PC case

[^dlcad]: [FreeCAD project](../src/lcddisplay/lcddisplay.ino)
[^dldxf]: [DXF sketch of the case walls](../src/lcddisplay/lcddisplay.ino)
[^dlgcode]: [GCode export of the corner pieces](../src/lcddisplay/lcddisplay.ino)
[^dlcode]: [Arduino project](../src/lcddisplay/lcddisplay.ino)
[^dlsrc]: [LCD test source code](../src/lcddisplay/lcddisplay.ino)

This document will explain the creation of my final FabLab course project, a mini PC case featuring an old laptop motherboard with integrated electronics for measuring and displaying system temperature.

![Showcase of the final PC case](../download/pccasedemo.webm)

All designs and components needed will be linked for download in their appropriate sections. For convenience, you can also get the direct references at the bottom of the document.

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
		  - ~~Activity light PCB~~ The activity light is integrated into the trackpad PCB and thus not usable in this case, hence it won't be used.
- The optical drive is optional but "nice to have"
- A custom backplate suited towards the laptop's I/O
- A feet-based "floating" design to increase airflow at the bottom and reduce overheating
- Strict separation between the case's measurement electronics and the laptop's components
- An easy way to remove the laptop's PCB and service it without taking apart the entire case
- Ambient lighting at the bottom of the case to indicate activity in an aesthetically thought-out way.
- A heat sensor for measuring the CPU temperature
- A way to display the measured CPU temperature
- A microcontroller for interfacing between the above mentioned components

The final parts required to build this case ended up being the following:

- My Laptop. Duh.
- 8mm and 3mm wood for the laser-cut walls
- A roll of PLA for 3D-printing the stabilizing corner pieces
- M3x16 case screws and M2.5 screws for the motherboard (as I am taking my laptop apart, it will supply these already)
- M3-compatible feet
- Cables solder and a Luster terminal for connecting the electronics
- An Arduino Uno. I used a selfbuilt clone in the final case, but tested on an official one.
- An MLX90614ESF infrared temperature sensor
- A WS2812B-based programmable LED strip
- A 1602A-compatible LCD character display
- A Potentiometer and Button for controlling the LCD display's properties (contrast, backlight)

## Designing the case

For stability I created the walls out of sketches intended for 8mm laser cutting, meaning I set the notch width for all joints to be 8mm as well. These 8mm walls provide a basic framework onto which I overlaid another sketch for 3mm wood cuts. This allowed me to facilitate airflow more easily as well as giving me the ability to repurpose the now hollow walls as cable channels for the microcontroller cables.

!(3mm case plate sketch)[../img/final/3mmsketch.png)

As you can see in the 8mm sketch, the case ended up consisting of 3 levels. At the top you have the motherboard below that is a compartment for routing the slimline SATA drive bays into and at the bottom you have a small channel for routing the LED strip into. For routing purposes I left a cable hole at the bottom of the bottom plates of each layer.

!(8mm frame sketch)[../img/final/8mmsketch.png)

Below you can also see the joints used for keeping the case together using the wooden plates alone. As I was forced to work two-dimensionallym I used mostly box joints for 8mm to 8mm corners/ridges, with the occasional through dovetail joint, usually for integrating 3mm plates into the design (for example one side of the backplate, the other uses a half-lap joint).

!(Regular joints in action)[../img/final/joints.png)

There is also what I call the "sandwhich joint" which technically isn't a joint in itself (as it doesn't serve stability on it's own) but serves as an easy to take apart version of a dovetail joint, mostly being held together using extension screw mounts.

!("Sandwhich" joint)[../img/final/sandwhichjoint.png)

For adding a bit of extra stability I decided to fixate all wooden plates using 3D printed corner pieces which are able to hold the pieces together using M3x16 screws. These I created from a base sketch which I applied multiple extrusions of differing height to.

!(Base sketches for the 3D printed pieces)[../img/final/plasticsketches.png)

!(Models for the 3D printed pieces)[../img/final/plasticmodels.png)


## Laser cutting

After exporting the case walls' sketch into DXF format, I prepared it in Rhinoceros using the following settings:

## 3D printing

I prepared the corner piece models in Cura using the following settings:

## Electronics wiring

As almost everything electronic is gonna be driven by the Arduino Uno, here's a table with how to wire it up. Additional wirings are written below in text form:

| Arduino pin | Component pin |
|---|---|
| GND | 1602A VDD; 1602A RW; 1602A K; Potentiometer; MLX90614ESF GND |
| +5V | 1602A VSS; 1602A A (over resistor); Potentiometer <++> |
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
