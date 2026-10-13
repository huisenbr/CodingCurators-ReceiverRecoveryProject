#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int LED_PIN = 9;
const int BUTTON_PIN = 2;

const bool SET_CLOCK = false;    // true for ONE upload to set the time, then back to false
const int PRE_DELAY_SEC = 40;    // countdown after the press, before it fires
const int BURN_MS = 4000;        // how long each fire lasts (stand-in for the burn)

int shots = 0;
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
  Serial.println("Press button to fire");
}

void loop() {
  DateTime now = rtc.now();

  if (digitalRead(BUTTON_PIN) == LOW) {
    shots++;
    Serial.print("Button pressed. Fire #");
    Serial.println(shots);

    for (int i = PRE_DELAY_SEC; i > 0; i--) {
      Serial.print("Firing in ");
      Serial.println(i);
      delay(1000);
    }

    Serial.print("FIRE at ");
    printTime(rtc.now());
    digitalWrite(LED_PIN, HIGH);
    delay(BURN_MS);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Done");

    while (digitalRead(BUTTON_PIN) == LOW) { }   // wait for release
    delay(200);                                  // simple debounce
  }

  if (now.second() != lastSecond) {   // print the time once per second
    lastSecond = now.second();
    printTime(now);
  }
  delay(50);
}
