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
