#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
SoftwareSerial s(2,3); // RX | TX

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int pirPin = 4;
int prevStat = 0;
int pirStat = 0;     
long lastDate = 0;


void setup() {
  pinMode(pirPin, INPUT);  
  Serial.begin(9600);
  Serial.println("Starting...");
  s.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }else{
    Serial.println("RTC ready..");
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }else{
    Serial.println("RTC running..");
  }
  delay(5000);
}

bool timer(){
  DateTime now = rtc.now();

  int delta = now.unixtime()-lastDate;
  lastDate = now.unixtime();
  return delta>10;
}

void printTime(){
  DateTime now = rtc.now();
  String string = "#EVENT : " + String(now.day()) + "/" + now.month() + "/" + now.year() + " " + now.hour() + ":" + now.minute() + ":" + now.second();
  Serial.println(string);
  s.write(string.c_str());
  /*
  s.print("#EVENT : ");
  s.write(now.year(), DEC);
  s.write('/');
  s.write(now.month(), DEC);
  s.write('/');
  s.write(now.day(), DEC);
  s.write(" ");
  s.write(now.hour(), DEC);
  s.write(':');
  s.write(now.minute()+11, DEC);
  s.write(':');
  s.write(now.second()+10, DEC);*/
}

bool pir(){
 pirStat = digitalRead(pirPin); 
 if (pirStat == HIGH && pirStat != prevStat) {
   prevStat = pirStat;
   return true;
 }
 prevStat = pirStat;
 return false;
}

void loop() {
  /*
   * Serial.println(pir());
   * Serial.println(timer());
   */
  if(pir()&&timer()){
    printTime();
  }
  String incomingString = "";
  bool stringReady = false;

  if(s.available()){
    incomingString = s.readString(); 
    Serial.println(incomingString);
    if(incomingString.substring (0,5) == "color"){
      Serial.println("Color is now set to : "+incomingString.substring(8));
    }else if(incomingString.substring (0,5) == "theme"){
      Serial.println("Theme is now set to : "+incomingString.substring(8));
    }else if(incomingString.substring (0,4) == "song"){
      Serial.println("Song is now set to : "+incomingString.substring(7));
    }else if(incomingString.substring (0,5) == "start"){
      Serial.println("Start is now set to : "+incomingString.substring(8,13));
      Serial.println("Stop is now set to : "+incomingString.substring(22));
    }
  }
}
