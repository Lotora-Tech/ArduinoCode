
#include <math.h>
#include <Wire.h>
#include "PCF85063TP.h"

PCD85063TP clock;//define a object of PCD85063TP class

int alarmHour = 16;
int alarmMinute = 0;
int tempTime = -1;
int tempSec = -1;

int waitUntilInterupt(int pin, int waitTime) {
  for (int i = 0; i <= waitTime; i += 10) {
    Serial.println(i);
    if (digitalRead(pin) == LOW) {
      Serial.println("Alarm is turned off");
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  delay(100);
      return 1;
    }
    delay(10);
  }
  return 0;
}

char getDat(char timeUnit[], int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    while (!Serial.available()) {
      delay(10);
    }
    timeUnit[i] = Serial.read();
  }

}

void beep(int seconds, int timeBetweenBeeps) {
  int alarmButton = 0;
  Serial.println("Alarm!");
  Serial.println(digitalRead(8));
  for (int i = 0; i < seconds && alarmButton == 0; i += 2*timeBetweenBeeps) {
    digitalWrite(7, HIGH);
    alarmButton = waitUntilInterupt(8, timeBetweenBeeps);
    digitalWrite(7, LOW);
    alarmButton = waitUntilInterupt(8, timeBetweenBeeps);
  }
  return;
}

int changeTypeToInt(char timeUnit[], int arrayLength) {

  double temp = 0.0;
  int j = 0;

  Serial.print("length: ");
  Serial.println(arrayLength);

  for (int i = arrayLength - 1; i >= 0; i--) {
    int parsed = (int) timeUnit[i];
    temp = pow(10, j) * (parsed - 48) + temp;
    j++;
  }
  return round(temp);
}

void printTime()
{
  clock.getTime();
  Serial.print(clock.hour, DEC);
  Serial.print(":");
  Serial.print(clock.minute, DEC);
  Serial.print(":");
  Serial.print(clock.second, DEC);
  Serial.print("  ");
  Serial.print(clock.month, DEC);
  Serial.print("/");
  Serial.print(clock.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(clock.year + 2000, DEC);
  Serial.print(" ");
  Serial.print(clock.dayOfMonth);
  Serial.print("*");
  switch (clock.dayOfWeek)// Friendly printout the weekday
  {
    case MON:
      Serial.print("MON");
      break;
    case TUE:
      Serial.print("TUE");
      break;
    case WED:
      Serial.print("WED");
      break;
    case THU:
      Serial.print("THU");
      break;
    case FRI:
      Serial.print("FRI");
      break;
    case SAT:
      Serial.print("SAT");
      break;
    case SUN:
      Serial.print("SUN");
      break;
  }
  Serial.println(" ");
}

void setup() {

  pinMode(7, OUTPUT);
  pinMode(8, INPUT_PULLUP);

  Serial.begin(9600);
  clock.begin();

  /*
    char year[4];
    char month[2];
    char day[2];
    char hour[2];
    char minute[2];
    char sec[2];

    getDat(year, 4);
    getDat(month, 2);
    getDat(day, 2);
    getDat(hour, 2);
    getDat(minute, 2);
    getDat(sec, 2);


    clock.stopClock();
    clock.fillByYMD(changeTypeToInt(year, 4), changeTypeToInt(month, 2), changeTypeToInt(day, 2)); //Jan 19,2013
    clock.fillByHMS(changeTypeToInt(hour, 2), changeTypeToInt(minute, 2), changeTypeToInt(sec, 2)); //15:28 30"
    clock.fillDayOfWeek(TUE);//Saturday
    clock.setTime();//write time to the RTC chip
    clock.startClock();
  */

  //clock.setcalibration(1, 32767.2);  // Setting offset by clock frequency
  uint8_t ret = clock.calibratBySeconds(0, -0.000041);
  Serial.print("offset value: ");
  Serial.print("0x");
  Serial.println(ret, HEX);

}

void loop() {
  // put your main code here, to run repeatedly:
  printTime();
  Serial.println(digitalRead(8));
  if (clock.hour == alarmHour && clock.minute == alarmMinute && tempTime != clock.minute) {
    beep(30000, 500);
    tempTime = clock.minute;
  }
  if(clock.minute == 0 && tempTime != clock.minute){
  beep(120, 30);
  tempTime = clock.minute;
  }
  if(clock.minute == 30 && tempTime != clock.minute){
  beep(60, 30);
  tempTime = clock.minute;
  }
  
  tempSec = clock.second;
  while(tempSec == clock.second){
    clock.getTime();
    delay(10);
  }
  if (tempTime != clock.minute) {
    tempTime = -1;
  }
  
}


