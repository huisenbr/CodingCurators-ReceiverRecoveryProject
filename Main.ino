#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>

RTC_DS3231 rtc;

const int WAKE_PIN = 2;       // DS3231 SQW/INT, goes LOW when the alarm fires
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const unsigned long SLEEP_SEC = 30;       // seconds to sleep before it fires (test value)
const unsigned long ON_MS = 3000;         // how long the output stays on
// ======================

void wakeISR() {}             // does nothing, just wakes the chip

void goToSleep() {
  Serial.flush();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();                // sleeps here until the interrupt
  sleep_disable();
}

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  pinMode(WAKE_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("RTC not found. Check SDA/SCL wiring.");
    while (true) {}
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   // set to compile time
  }

  rtc.disableAlarm(2);
  rtc.clearAlarm(1);
  rtc.writeSqwPinMode(DS3231_OFF);        // makes the pin an alarm output, not a square wave

  DateTime fireAt = rtc.now() + TimeSpan(SLEEP_SEC);
  rtc.setAlarm1(fireAt, DS3231_A1_Date);  // matches date, hour, minute, second

  Serial.print("Now:    ");
  Serial.println(rtc.now().timestamp());
  Serial.print("Fires:  ");
  Serial.println(fireAt.timestamp());
  Serial.println("Sleeping...");

  attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeISR, LOW);
  goToSleep();
  detachInterrupt(digitalPinToInterrupt(WAKE_PIN));

  // Awake again
  Serial.println("WAKE");
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);
    Serial.println("FIRE");
    digitalWrite(FIRE_PIN, HIGH);
    delay(ON_MS);
    digitalWrite(FIRE_PIN, LOW);
  }
  rtc.disableAlarm(1);
  Serial.println("Done. Sleeping until reset.");
  Serial.flush();
  goToSleep();                // no wake source left, so it stays off until reset
}

void loop() {}
