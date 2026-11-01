const int MIC_PIN = A0;
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const int THRESHOLD = 100;                // sound swing (0 to 1023) that counts as a clap
const unsigned long WINDOW_MS = 1500;     // second clap must land within this long
const unsigned long MIN_GAP_MS = 150;     // ignores echo and ringing from the first clap
const unsigned long ON_MS = 3000;         // how long the output stays on
// ======================

bool done = false;
bool haveFirst = false;
unsigned long firstAt = 0;

int soundLevel() {
  int lo = 1023, hi = 0;
  unsigned long start = millis();
  while (millis() - start < 30) {         // 30 ms sample window
    int v = analogRead(MIC_PIN);
    if (v < lo) lo = v;
    if (v > hi) hi = v;
  }
  return hi - lo;
}

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Armed. Double clap to trigger.");
}

void loop() {
  if (done) return;

  int level = soundLevel();
  Serial.println(level);

  if (level > THRESHOLD) {
    unsigned long now = millis();
    if (haveFirst && now - firstAt >= MIN_GAP_MS && now - firstAt <= WINDOW_MS) {
      done = true;                        // set first so it only fires once
      Serial.println("TRIGGERED");
      digitalWrite(FIRE_PIN, HIGH);
      delay(ON_MS);
      digitalWrite(FIRE_PIN, LOW);
      Serial.println("Done. Reset the board to run again.");
    } else if (!haveFirst || now - firstAt > WINDOW_MS) {
      haveFirst = true;
      firstAt = now;
    }
  }
}
