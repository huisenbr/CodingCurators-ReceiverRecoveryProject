#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int LED_PIN = 9;
const int BUTTON_PIN = 2;

const bool SET_CLOCK = false;     // set true for ONE upload to set the time, then back to false
const long DELAY_SECONDS = 120;   // test alarm: 2 minutes after arming

bool armed = false;
bool fired = false;
DateTime alarmTime;
int lastSecond = -1;

void printTime(const DateTime& t) {
  Serial.print(t.year());   Serial.print("-");
  Serial.print(t.month());  Serial.print("-");
  Serial.print(t.day());    Serial.print(" ");
  Serial.print(t.hour());   Serial.print(":");
  Serial.print(t.minute()); Serial.print(":");
  Serial.println(t.second());
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (1);
  }
  if (SET_CLOCK || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Clock set");
  }
  Serial.println("Press button to arm");
}

void loop() {
  DateTime now = rtc.now();

  if (!armed && digitalRead(BUTTON_PIN) == LOW) {
    armed = true;
    alarmTime = now + TimeSpan(DELAY_SECONDS);
    Serial.print("Armed. Fires at ");
    printTime(alarmTime);
  }

  if (armed && !fired && now.unixtime() >= alarmTime.unixtime()) {
    digitalWrite(LED_PIN, HIGH);
    delay(3000);                      // stand-in for the 3 second burn
    digitalWrite(LED_PIN, LOW);
    fired = true;
    Serial.println("Fired. Idle now.");
  }

  if (now.second() != lastSecond) {   // print once per second
    lastSecond = now.second();
    printTime(now);
  }
  delay(100);
}
