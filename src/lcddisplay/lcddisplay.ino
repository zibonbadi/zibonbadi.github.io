// include the library code:
#include <LiquidCrystal.h>


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


// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  // display dimensions (16x2)
  lcd.begin(16, 2);

  // dejunkification
  lcd.clear();

  //load custom characters into the LCD's RAM
  lcd.createChar(0, temp);
  lcd.createChar(1, fan);
  lcd.createChar(2, fire);
}

void loop() 
{
  //basics
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Fan speed");

  //custom characters
  lcd.setCursor(14, 0);
  lcd.write(byte(0));
  lcd.write(byte(2));
  lcd.setCursor(15, 1);
  lcd.write(byte(1));
}
