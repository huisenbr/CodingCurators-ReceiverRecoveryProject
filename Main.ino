#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int LED_PIN = 9;
const int BUTTON_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time");
  }
  // Sets the clock to the time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
rtc.adjust(DateTime(2026, 10, 1, 12, 0, 0));  // year, month, day, hour, minute, second
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year());   Serial.print("-");
  
  Serial.print(now.month());  Serial.print("-");
  Serial.print(now.day());    Serial.print(" ");
  Serial.print(now.hour());   Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.println(now.second());
   if (now.second() >10 && now.second()<12) {   // pressed
    digitalWrite(LED_PIN, HIGH);
    delay(3000);                          // stand-in for the 3 second burn
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
