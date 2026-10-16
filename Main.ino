#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int FIRE_PIN = 9;       // MOSFET gate (or LED while testing)
const int BUTTON_PIN = 2;

// ===== EDIT THESE =====
DateTime LAUNCH_TIME(2026, 10, 15, 19, 30, 0);  // year, month, day, hour (24h), minute, second
const int BURN_MS = 4000;                       // how long the fire lasts
const bool SET_CLOCK = false;                   // true for ONE upload to set the clock, then back to false
const bool AUTO_ARM = false;                    // true = arms itself on power up, no button needed
// ======================

bool armed = false;
int lastSecond = -1;

void printTime(const DateTime& t) {
  Serial.print(t.year());   Serial.print("-");
  Serial.print(t.month());  Serial.print("-");
  Serial.print(t.day());    Serial.print(" ");
  Serial.print(t.hour());   Serial.print(":");
  Serial.print(t.minute()); Serial.print(":");
  Serial.println(t.second());
}

void arm(const DateTime& now) {
  if (LAUNCH_TIME.unixtime() <= now.unixtime()) {
    Serial.println("Launch time is in the past. Not armed.");
    return;
  }
  armed = true;
  Serial.print("ARMED. Launch at ");
  printTime(LAUNCH_TIME);
}

void setup() {
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
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

  Serial.print("Now: ");
  printTime(rtc.now());
  if (AUTO_ARM) {
    arm(rtc.now());
  } else {
    Serial.println("Press button to arm");
  }
}

void loop() {
  DateTime now = rtc.now();

  // Arm with the button
  if (!armed && digitalRead(BUTTON_PIN) == LOW) {
    arm(now);
    while (digitalRead(BUTTON_PIN) == LOW) { }   // wait for release
    delay(200);                                  // simple debounce
  }

  // Fire once when the launch time arrives
  if (armed && now.unixtime() >= LAUNCH_TIME.unixtime()) {
    Serial.print("FIRE at ");
    printTime(now);
    digitalWrite(FIRE_PIN, HIGH);
    delay(BURN_MS);
    digitalWrite(FIRE_PIN, LOW);
    armed = false;
    Serial.println("Done. Idle. Press button to arm again");
  }

  // Print status once per second
  if (now.second() != lastSecond) {
    lastSecond = now.second();
    printTime(now);
    if (armed) {
      Serial.print("  seconds until launch: ");
      Serial.println(LAUNCH_TIME.unixtime() - now.unixtime());
    }
  }
  delay(50);
}
