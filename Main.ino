#include <Wire.h>
#include "RTClib.h"
#include <TM1637Display.h>

RTC_DS3231 rtc;
const int FIRE_PIN = 9;       // MOSFET gate (or LED while testing)
const int CLK_PIN = 3;
const int DIO_PIN = 4;
TM1637Display display(CLK_PIN, DIO_PIN);

// ===== EDIT THESE =====
DateTime LAUNCH_TIME(2026, 10, 15, 19, 30, 0);  // year, month, day, hour (24h), minute, second
const int BURN_MS = 4000;                       // how long the fire lasts
const bool SET_CLOCK = false;                   // true for ONE upload to set the clock, then back to false
// ======================

const uint8_t SEG_DONE[] = { 0x5E, 0x5C, 0x54, 0x79 };   // spells "donE"

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

void showRemaining(long secs) {
  if (secs < 0) secs = 0;
  if (secs >= 360000L) {                 // 100+ hours: show whole days
    display.showNumberDec((int)(secs / 86400L), false);
  } else if (secs >= 3600) {             // 1 to 99 hours: HH:MM
    int h = secs / 3600;
    int m = (secs % 3600) / 60;
    display.showNumberDecEx(h * 100 + m, 0b01000000, true);
  } else {                               // under an hour: MM:SS
    int m = secs / 60;
    int s = secs % 60;
    display.showNumberDecEx(m * 100 + s, 0b01000000, true);
  }
}

void setup() {
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  Serial.begin(9600);
  display.setBrightness(7);
  display.clear();

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
    display.setSegments(SEG_DONE);
  }
}

void loop() {
  DateTime now = rtc.now();

  if (!done && now.unixtime() >= LAUNCH_TIME.unixtime()) {
    Serial.print("FIRE at ");
    printTime(now);
    digitalWrite(FIRE_PIN, HIGH);
    delay(BURN_MS);
    digitalWrite(FIRE_PIN, LOW);
    done = true;
    display.setSegments(SEG_DONE);
    Serial.println("Done. Idle.");
  }

  if (now.second() != lastSecond) {
    lastSecond = now.second();
    printTime(now);
    if (!done) {
      long remaining = LAUNCH_TIME.unixtime() - now.unixtime();
      showRemaining(remaining);
      Serial.print("  seconds until launch: ");
      Serial.println(remaining);
    }
  }
  delay(50);
}
