#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int LED_PIN = 9;
const int BUTTON_PIN = 2;

const bool SET_CLOCK = false;      // true for ONE upload to set the time, then back to false
const long FIRST_DELAY = 30;       // seconds from button press to first fire
const long INTERVAL = 30;          // seconds between fires
const int FIRE_COUNT = 3;          // how many times to fire per arming
const int BURN_MS = 3000;          // how long each fire lasts (stand-in for the burn)

bool armed = false;
int shots = 0;
DateTime nextFire;
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

  // Arm on button press (only when idle)
  if (!armed && digitalRead(BUTTON_PIN) == LOW) {
    armed = true;
    shots = 0;
    nextFire = now + TimeSpan(FIRST_DELAY);
    Serial.print("Armed. First fire at ");
    printTime(nextFire);
    delay(300);   // simple debounce
  }

  // Fire when the scheduled time arrives
  if (armed && now.unixtime() >= nextFire.unixtime()) {
    digitalWrite(LED_PIN, HIGH);
    delay(BURN_MS);
    digitalWrite(LED_PIN, LOW);
    shots++;
    Serial.print("Fired ");
    Serial.print(shots);
    Serial.print(" of ");
    Serial.println(FIRE_COUNT);

    if (shots >= FIRE_COUNT) {
      armed = false;
      Serial.println("Done. Press button to arm again");
    } else {
      nextFire = nextFire + TimeSpan(INTERVAL);
      Serial.print("Next fire at ");
      printTime(nextFire);
    }
  }

  // Print the time once per second
  if (now.second() != lastSecond) {
    lastSecond = now.second();
    printTime(now);
  }
  delay(100);
}
