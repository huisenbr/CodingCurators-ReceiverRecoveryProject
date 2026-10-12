#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time");
  }
  // Sets the clock to the time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year());   Serial.print("-");
  Serial.print(now.month());  Serial.print("-");
  Serial.print(now.day());    Serial.print(" ");
  Serial.print(now.hour());   Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.println(now.second());
  delay(1000);
}
