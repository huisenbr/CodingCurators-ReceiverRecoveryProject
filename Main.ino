#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int FIRE_PIN = 9;       // MOSFET gate, now driving the burn wire

// ===== EDIT THESE =====
DateTime LAUNCH_TIME(2026, 10, 15, 19, 30, 0);  // year, month, day, hour (24h), minute, second
const unsigned long BURN_MS = 1500;             // how long each burn lasts (start short, increase slowly)
const int BURN_COUNT = 1;                       // how many burns (2 = one backup burn)
const int GAP_SEC = 30;                         // pause between burns, lets the wire cool
const bool SET_CLOCK = false;                   // true for ONE upload to set the clock, then back to false
// ======================

const unsigned long MAX_BURN_MS = 8000;         // safety cap, no burn can ever exceed this

bool done = false;
int lastSecond = -1;

void printTime(const DateTime& t) {
  Serial.print(t.year());   Serial.print("-");
  Serial.print(t.month());  Serial.print("-");
  Serial.print(t.day());    Serial.print(" ");
  Serial.print(t.hour());   Serial.print(":");
  Serial.print(t.minute()); Serial.print(":");
  Serial.println(t.second());
}

void burn(unsigned long ms) {
  if (ms > MAX_BURN_MS) ms = MAX_BURN_MS;
  digitalWrite(FIRE_PIN, HIGH);
  delay(ms);
  digitalWrite(FIRE_PIN, LOW);
}

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (1);
  }
  if (SET_CLOCK || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Clock set");
  }

  DateTime now = rtc.now();
  Serial.print("Now: ");
  printTime(now);
  Serial.print("Launch at: ");
  printTime(LAUNCH_TIME);

  if (LAUNCH_TIME.unixtime() <= now.unixtime()) {
    Serial.println("Launch time already passed. Idle.");
    done = true;
  }
}

void loop() {
  DateTime now = rtc.now();

  if (!done && now.unixtime() >= LAUNCH_TIME.unixtime()) {
    done = true;                         // set first so a reset can never repeat the burn
    Serial.print("FIRE at ");
    printTime(now);
    for (int i = 1; i <= BURN_COUNT; i++) {
      Serial.print("Burn ");
      Serial.print(i);
      Serial.print(" of ");
      Serial.println(BURN_COUNT);
      burn(BURN_MS);
      if (i < BURN_COUNT) delay((unsigned long)GAP_SEC * 1000UL);
    }
    Serial.println("Done. Idle.");
  }

  if (now.second() != lastSecond) {
    lastSecond = now.second();
    printTime(now);
    if (!done) {
      Serial.print("  seconds until launch: ");
      Serial.println(LAUNCH_TIME.unixtime() - now.unixtime());
    }
  }
  delay(50);
}
