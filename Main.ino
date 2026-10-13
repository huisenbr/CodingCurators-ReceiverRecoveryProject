#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
const int LED_PIN = 9;       // goes to the MOSFET gate
const int BUTTON_PIN = 2;

const bool SET_CLOCK = false;        // true for ONE upload to set the time, then back to false
const bool USE_FIXED_DATE = false;   // false = fire ALARM_DELAY_SEC after arming, true = fire at FIXED_ALARM
const long ALARM_DELAY_SEC = 120;    // test alarm: 2 minutes after the button press
const int BURN_MS = 4000;            // how long the fire lasts (stand-in for the burn)

// Used only when USE_FIXED_DATE is true: year, month, day, hour, minute, second
DateTime FIXED_ALARM(2026, 10, 12, 20, 30, 0);

bool armed = false;
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
  digitalWrite(LED_PIN, LOW);
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
    DateTime target = USE_FIXED_DATE ? FIXED_ALARM : now + TimeSpan(ALARM_DELAY_SEC);
    if (target.unixtime() <= now.unixtime()) {
      Serial.println("Alarm time is in the past. Not armed.");
    } else {
      alarmTime = target;
      armed = true;
      Serial.print("ARMED. Fires at ");
      printTime(alarmTime);
    }
    while (digitalRead(BUTTON_PIN) == LOW) { }   // wait for release
    delay(200);                                  // simple debounce
  }

  // Fire once when the alarm time arrives
  if (armed && now.unixtime() >= alarmTime.unixtime()) {
    Serial.print("FIRE at ");
    printTime(now);
    digitalWrite(LED_PIN, HIGH);
    delay(BURN_MS);
    digitalWrite(LED_PIN, LOW);
    armed = false;
    Serial.println("Done. Press button to arm again");
  }

  // Print status once per second
  if (now.second() != lastSecond) {
    lastSecond = now.second();
    printTime(now);
    if (armed) {
      Serial.print("  seconds left: ");
      Serial.println(alarmTime.unixtime() - now.unixtime());
    }
  }
  delay(50);
}
