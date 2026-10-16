#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int FIRE_PIN = 9;       // MOSFET gate (or LED while testing)

// ===== EDIT THESE =====
DateTime LAUNCH_TIME(2026, 10, 15, 19, 30, 0);  // year, month, day, hour (24h), minute, second
const unsigned long BURN_MS = 4000;             // how long the fire lasts
const bool SET_CLOCK = false;                   // true for ONE upload to set the clock, then back to false
// ======================

const int SEG_PINS[8] = {2, 3, 4, 5, 6, 7, 8, A1};   // A, B, C, D, E, F, G, DP
const int DIGIT_PINS[4] = {10, 11, 12, 13};          // digit 1 to 4, LOW = on

const byte NUM[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
const byte DOT_BIT = 0x80;
const byte LETTER_d = 0x5E;
byte buf[4] = {0, 0, 0, 0};

bool done = false;
bool firing = false;
unsigned long fireStart = 0;
unsigned long lastRead = 0;
int lastSecond = -1;

void printTime(const DateTime& t) {
  Serial.print(t.year());   Serial.print("-");
  Serial.print(t.month());  Serial.print("-");
  Serial.print(t.day());    Serial.print(" ");
  Serial.print(t.hour());   Serial.print(":");
  Serial.print(t.minute()); Serial.print(":");
  Serial.println(t.second());
}

void setBuf(byte a, byte b, byte c, byte d) {
  buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d;
}

void showRemaining(long secs) {
  if (secs < 0) secs = 0;
  if (secs >= 360000L) {                 // 100+ hours: d plus days
    int days = secs / 86400L;
    setBuf(LETTER_d, NUM[(days / 100) % 10], NUM[(days / 10) % 10], NUM[days % 10]);
  } else if (secs >= 3600) {             // HH.MM with a trailing dot
    int h = secs / 3600;
    int m = (secs % 3600) / 60;
    setBuf(NUM[h / 10], NUM[h % 10] | DOT_BIT, NUM[m / 10], NUM[m % 10] | DOT_BIT);
  } else {                               // MM.SS
    int m = secs / 60;
    int s = secs % 60;
    setBuf(NUM[m / 10], NUM[m % 10] | DOT_BIT, NUM[s / 10], NUM[s % 10]);
  }
}

// Must run constantly. Lights one digit at a time, fast enough to look steady.
void refreshDisplay() {
  for (int d = 0; d < 4; d++) {
    for (int k = 0; k < 4; k++) digitalWrite(DIGIT_PINS[k], HIGH);   // all digits off
    for (int s = 0; s < 8; s++) digitalWrite(SEG_PINS[s], (buf[d] >> s) & 1);
    digitalWrite(DIGIT_PINS[d], LOW);                                // this digit on
    delayMicroseconds(2000);
  }
  for (int k = 0; k < 4; k++) digitalWrite(DIGIT_PINS[k], HIGH);
}

void setup() {
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  for (int i = 0; i < 8; i++) { pinMode(SEG_PINS[i], OUTPUT); digitalWrite(SEG_PINS[i], LOW); }
  for (int i = 0; i < 4; i++) { pinMode(DIGIT_PINS[i], OUTPUT); digitalWrite(DIGIT_PINS[i], HIGH); }
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
    setBuf(0x5E, 0x5C, 0x54, 0x79);      // "donE"
  }
}

void loop() {
  unsigned long ms = millis();

  if (ms - lastRead >= 250) {            // read the clock 4 times a second
    lastRead = ms;
    DateTime now = rtc.now();

    if (!done && !firing) {
      long remaining = (long)LAUNCH_TIME.unixtime() - (long)now.unixtime();
      if (remaining <= 0) {
        firing = true;
        fireStart = ms;
        digitalWrite(FIRE_PIN, HIGH);
        setBuf(0x71, 0x06, 0x50, 0x79);  // "FirE"
        Serial.print("FIRE at ");
        printTime(now);
      } else {
        showRemaining(remaining);
      }
    }

    if (now.second() != lastSecond) {
      lastSecond = now.second();
      printTime(now);
    }
  }

  if (firing && ms - fireStart >= BURN_MS) {
    digitalWrite(FIRE_PIN, LOW);
    firing = false;
    done = true;
    setBuf(0x5E, 0x5C, 0x54, 0x79);      // "donE"
    Serial.println("Done. Idle.");
  }

  refreshDisplay();
}
