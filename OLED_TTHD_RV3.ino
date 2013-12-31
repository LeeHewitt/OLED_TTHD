/*OLED Temprature, Humid and time
 Created 14/10/2013
 Jed Hodson 
 */
#include <SPI.h>
#include <SD.h>
#include <FTOLED.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_36.h>
#include "DHT.h"
#include <Wire.h>
#include "RTClib.h"
#define DHTPIN 9
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 RTC;
const byte pin_cs = 7;
const byte pin_dc = 2;
const byte pin_reset = 3;

#define Green_LED 5
#define Blue1_LED 6
#define Blue2_LED 8


OLED oled(pin_cs, pin_dc, pin_reset);
OLED_TextBox temp(oled,20,7,30,17);
OLED_TextBox humid(oled,65,7,50,17);
OLED_TextBox date(oled,35,105,128,15);
OLED_TextBox time(oled,35,52,128,32);

#define WELCOME F("Temprature,\nHumidity \nand Time\nOn OLED \nDisplay\nJed Hodson \n2013")
#define ERRORI2C F("An ERROR has\noccured,\nplease I2C\nwiring and try\nagain later!")
#define ERRORDHT F("An ERROR has\noccured,\nplease DHT\nwiring and try\nagain later!")
void setup() {
  //Start everything up
  oled.begin();
  dht.begin();
  Wire.begin();
  RTC.begin();
  pinMode(Green_LED, OUTPUT);
  pinMode(Blue1_LED, OUTPUT);
  pinMode(Blue2_LED, OUTPUT);
  
  //Preform Blink of the LEDs
Blink_Green();
Blink_Blue1();
Blink_Blue2();

  //Display start up message
  oled.selectFont(Arial14);
  oled.drawString(10,101,WELCOME,GREEN,BLACK);
  Blink_Green();
  oled.clearScreen();
}

void loop() {
  DateTime now = RTC.now();
  //Always Check the DHT22 for unplug
  byte h = dht.readHumidity();
  byte t = dht.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    oled.drawString(10,101,ERRORDHT,RED,BLACK);
    delay(2500);
    oled.drawString(10,101,"",RED,BLACK);
    Blink_Blue1();
  }
  //And the RTC just in case
  if (! RTC.isrunning()) {
    oled.selectFont(Arial14);
    //DISPLAY ERROR MESSAGE
    oled.drawString(10,101,ERRORI2C,RED,BLACK);
    delay(2500);
    oled.drawString(10,101,"",RED,BLACK);
    Blink_Blue2();
  }

  String writeString;
  time.setForegroundColour(GREEN);
  writeString = "";
  writeString += String(now.hour());
  writeString += ":";
  writeString += String(now.minute());
  oled.selectFont(Droid_Sans_36);
  time.print(writeString);
  time.reset();

  writeString = "";
  writeString += String(now.day());
  writeString += "/";
  writeString += String(now.month());
  writeString += "/";
  writeString += String(now.year());
  date.setForegroundColour(WHITE);
  oled.selectFont(SystemFont5x7);
  date.print(writeString);
  date.reset();

  temp.setForegroundColour(RED);
  int tempS = t;
  writeString = "";
  writeString += String(tempS);
  writeString += "*C";
  oled.selectFont(Arial_Black_16);
  temp.print(writeString);
  temp.reset();

  humid.setForegroundColour(ROYALBLUE);
  byte humidS = h;
  writeString = "";
  writeString += ("RH ");
  writeString += String(humidS);
  oled.selectFont(Arial_Black_16);
  humid.print(writeString);
  humid.reset();
  delay(500);
}

void Blink_Green() {
  digitalWrite(Green_LED, HIGH);
  delay(500);
  digitalWrite(Green_LED, LOW);
  delay(500);
  digitalWrite(Green_LED, HIGH);
  delay(500);
  digitalWrite(Green_LED, LOW);
  delay(500);
}

void Blink_Blue1() {
  digitalWrite(Blue1_LED, HIGH);
  delay(500);
  digitalWrite(Blue1_LED, LOW);
  delay(500);
  digitalWrite(Blue1_LED, HIGH);
  delay(500);
  digitalWrite(Blue1_LED, LOW);
  delay(500);
}

void Blink_Blue2() {
  digitalWrite(Blue2_LED, HIGH);
  delay(500);
  digitalWrite(Blue2_LED, LOW);
  delay(500);
  digitalWrite(Blue2_LED, HIGH);
  delay(500);
  digitalWrite(Blue2_LED, LOW);
  delay(500);
}
  
 
  

