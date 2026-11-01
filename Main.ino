const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const int TRIGGER_CM = 15;              // fires when something is closer than this
const int HITS_NEEDED = 3;              // readings in a row required, avoids false triggers
const unsigned long ON_MS = 3000;       // how long the output stays on
// ======================

bool done = false;
int hits = 0;

long readCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long us = pulseIn(ECHO_PIN, HIGH, 30000UL);   // 30 ms timeout
  if (us == 0) return -1;                       // no echo
  return us / 58;                               // microseconds to cm
}

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Armed. Move something close to the sensor.");
}

void loop() {
  if (done) return;

  long cm = readCm();
  Serial.println(cm);

  if (cm > 0 && cm < TRIGGER_CM) hits++;
  else hits = 0;

  if (hits >= HITS_NEEDED) {
    done = true;                          // set first so it only fires once
    Serial.println("TRIGGERED");
    digitalWrite(FIRE_PIN, HIGH);
    delay(ON_MS);
    digitalWrite(FIRE_PIN, LOW);
    Serial.println("Done. Reset the board to run again.");
  }
  delay(60);
}
