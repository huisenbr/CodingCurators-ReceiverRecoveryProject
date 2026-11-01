const int MIC_PIN = A0;
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const int MARGIN = 40;                    // how far above room noise counts as a clap
const unsigned long WINDOW_MS = 1500;     // second clap must land within this long
const unsigned long MIN_GAP_MS = 150;     // ignores ringing right after the first clap
const unsigned long ON_MS = 3000;         // how long the output stays on
// ======================

int threshold = 0;
bool done = false;
bool haveFirst = false;
bool wasLoud = false;
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
  Serial.println("Calibrating, stay quiet...");

  int ambient = 0;
  for (int i = 0; i < 60; i++) {          // about 2 seconds
    int l = soundLevel();
    if (l > ambient) ambient = l;
  }
  threshold = ambient + MARGIN;
  Serial.print("Room noise: ");
  Serial.print(ambient);
  Serial.print("  Threshold: ");
  Serial.println(threshold);
  Serial.println("Armed. Double clap to trigger.");
}

void loop() {
  if (done) return;

  int level = soundLevel();
  Serial.println(level);

  bool loud = level > threshold;
  if (loud && !wasLoud) {                 // new sound, not a continuing one
    unsigned long now = millis();
    if (haveFirst && now - firstAt >= MIN_GAP_MS && now - firstAt <= WINDOW_MS) {
      done = true;                        // set first so it only fires once
      Serial.println("TRIGGERED");
      digitalWrite(FIRE_PIN, HIGH);
      delay(ON_MS);
      digitalWrite(FIRE_PIN, LOW);
      Serial.println("Done. Reset the board to run again.");
    } else {
      haveFirst = true;
      firstAt = now;
    }
  }
  wasLoud = loud;
}
