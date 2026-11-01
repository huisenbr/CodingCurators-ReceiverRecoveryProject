const int MIC_PIN = A0;
const int FIRE_PIN = 9;       // MOSFET gate, same as before

// ===== EDIT THESE =====
const float TARGET_HZ = 1000.0;           // tone to listen for
const float MIN_RMS = 3.0;                // ignores near-silence (raise if it false triggers)
const float MIN_RATIO = 0.5;              // 0 to 1, how pure the tone must be
const int HITS_NEEDED = 8;                // windows in a row (each ~25 ms), so ~0.2 s of tone
const unsigned long ON_MS = 3000;         // how long the output stays on
// ======================

const int N = 200;                        // samples per window
const float SAMPLE_RATE = 8000.0;         // 125 us between samples, 40 Hz per bin
const unsigned long SAMPLE_US = 125;

int samples[N];
float coeff;
int hits = 0;
bool done = false;

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  Serial.begin(9600);
  coeff = 2.0 * cos(2.0 * PI * TARGET_HZ / SAMPLE_RATE);
  Serial.print("Listening for ");
  Serial.print(TARGET_HZ);
  Serial.println(" Hz");
}

void loop() {
  if (done) return;

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

  bool heard = (rms > MIN_RMS) && (ratio > MIN_RATIO);
  hits = heard ? hits + 1 : 0;

  Serial.print("rms ");
  Serial.print(rms, 1);
  Serial.print("  ratio ");
  Serial.print(ratio, 2);
  Serial.print("  hits ");
  Serial.println(hits);

  if (hits >= HITS_NEEDED) {
    done = true;                          // set first so it only fires once
    Serial.println("TONE DETECTED");
    digitalWrite(FIRE_PIN, HIGH);
    delay(ON_MS);
    digitalWrite(FIRE_PIN, LOW);
    Serial.println("Done. Reset the board to run again.");
  }
}
