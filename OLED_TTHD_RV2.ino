/*OLED Temprature, Humid and time
 Created 19/09/2013
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

OLED oled(pin_cs, pin_dc, pin_reset);
OLED_TextBox temp(oled,20,7,30,17);
OLED_TextBox humid(oled,72,7,30,17);
OLED_TextBox date(oled,35,105,128,15);
OLED_TextBox time(oled,35,52,128,32);

const char *WELCOME = "Temprature,\nHumidity \nand Time\nOn OLED \nDisplay\nJed Hodson \n2013";
const char *ERRORHAP = "An ERROR has\noccured,\nplease check\nwiring and try\nagain later!";
void setup() {
  //Start everything up
  oled.begin();
  dht.begin();
  Wire.begin();
  RTC.begin();
  //Display start up message
  oled.selectFont(Arial14);
  oled.drawString(10,101,WELCOME,GREEN,BLACK);
  delay(5000);
  //Check everythings connected
  //No RTC!
  if (! RTC.isrunning()) {
    oled.selectFont(Arial14);
    //DISPLAY ERROR MESSAGE
    oled.drawString(10,101,ERRORHAP,RED,BLACK);
    delay(2500);
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  //And DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    oled.drawString(10,101,ERRORHAP,RED,BLACK);
    delay(2500);
  }
  oled.clearScreen();
}

void loop() {
  while(1){
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
    DateTime now = RTC.now();
    //Time to make some Strings
    String dateString = "";
    dateString += String(now.day());
    dateString += "/";
    dateString += String(now.month());
    dateString += "/";
    dateString += String(now.year());
    int tempS = t;
    String tempString = "";
    tempString += String(tempS);
    tempString += "*C";
    int humidS = h;
    String humidString = "";
    humidString += String(humidS);
    humidString += "RH";
    String timeString = "";
    timeString += String(now.hour());
    timeString += ":";
    timeString += String(now.minute());

    //Print Text
    //Set colours
    temp.setForegroundColour(RED);
    humid.setForegroundColour(ROYALBLUE);
    time.setForegroundColour(GREEN);
    date.setForegroundColour(WHITE);

oled.selectFont(Arial_Black_16);
    temp.print(tempString);
    
    oled.selectFont(SystemFont5x7);
    date.print(dateString);
 
 oled.selectFont(Droid_Sans_36);
    time.print(timeString);
    
    oled.selectFont(Arial_Black_16);
    humid.print(humidString);
    
    
    
    
   
    temp.reset();
  
    time.reset();
  
    date.reset();
  
    humid.reset();
    delay(100);

  }

  
}

