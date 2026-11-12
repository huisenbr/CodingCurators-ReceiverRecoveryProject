#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>

RTC_DS3231 rtc;

const int WAKE_PIN = 2;       // DS3231 SQW/INT, goes LOW when the alarm fires
const int MIC_PIN = A0;
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const unsigned long SLEEP_SEC = 30;       // seconds asleep once at power-up, then it stays awake
const float TARGET_HZ = 1000.0;           // tone to listen for
const float MIN_RMS = 3.0;                // ignores near-silence
const float MIN_RATIO = 0.5;              // 0 to 1, how pure the tone must be
const int HITS_NEEDED = 8;                // windows in a row (~0.2 s of tone)
const unsigned long ON_MS = 3000;         // how long the output stays on
// ======================

const int N = 200;                        // samples per window
const float SAMPLE_RATE = 8000.0;
const unsigned long SAMPLE_US = 125;

int samples[N];
float coeff;

void wakeISR() {}             // does nothing, just wakes the chip

void sleepFor(unsigned long sec) {
  rtc.clearAlarm(1);
  DateTime wakeAt = rtc.now() + TimeSpan(sec);
  rtc.setAlarm1(wakeAt, DS3231_A1_Date);

  Serial.print("Now:    ");
  Serial.println(rtc.now().timestamp());
  Serial.print("Wakes:  ");
  Serial.println(wakeAt.timestamp());
  Serial.println("Sleeping...");
  Serial.flush();

  attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeISR, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();                // sleeps here until the alarm
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(WAKE_PIN));

  rtc.clearAlarm(1);          // releases the pin so it goes back to high
  Serial.println("Awake.");
}

bool heardTone() {
  // Collect one window at a fixed sample rate
  unsigned long next = micros();
  for (int i = 0; i < N; i++) {
    while ((long)(micros() - next) < 0) {}
    samples[i] = analogRead(MIC_PIN);
    next += SAMPLE_US;
  }

  // Remove the DC offset
  long sum = 0;
  for (int i = 0; i < N; i++) sum += samples[i];
  float mean = (float)sum / N;

  // Goertzel filter plus total energy
  float s1 = 0, s2 = 0, sumsq = 0;
  for (int i = 0; i < N; i++) {
    float x = samples[i] - mean;
    sumsq += x * x;
    float s = x + coeff * s1 - s2;
    s2 = s1;
    s1 = s;
  }
  float mag2 = s1 * s1 + s2 * s2 - coeff * s1 * s2;
  float rms = sqrt(sumsq / N);
  float ratio = (sumsq > 0) ? mag2 / (sumsq * N / 2.0) : 0;

  Serial.print("rms ");
  Serial.print(rms, 1);
  Serial.print("  ratio ");
  Serial.println(ratio, 2);

  return (rms > MIN_RMS) && (ratio > MIN_RATIO);
}

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  pinMode(WAKE_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  coeff = 2.0 * cos(2.0 * PI * TARGET_HZ / SAMPLE_RATE);

  if (!rtc.begin()) {
    Serial.println("RTC not found. Check SDA/SCL wiring.");
    while (true) {}
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   // set to compile time
  }

  rtc.disableAlarm(2);
  rtc.clearAlarm(1);
  rtc.writeSqwPinMode(DS3231_OFF);        // makes the pin an alarm output

  sleepFor(SLEEP_SEC);                    // sleep once, until the wake time
  Serial.println("Listening for tone. Staying awake from here on.");
}

void loop() {
  // Stays awake and keeps listening. A later tone fires again (a retry).
  int hits = 0;
  while (hits < HITS_NEEDED) {
    hits = heardTone() ? hits + 1 : 0;
  }

  Serial.println("TONE DETECTED");
  digitalWrite(FIRE_PIN, HIGH);
  delay(ON_MS);
  digitalWrite(FIRE_PIN, LOW);
  Serial.println("Fired. Still awake, still listening.");
}
