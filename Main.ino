const int FIRE_PIN = 9;       // MOSFET gate, driving the burn wire

// ===== EDIT THESE =====
const unsigned long DELAY_SEC = 10;       // seconds after power up before it fires
const unsigned long BURN_MS = 1500;       // how long the burn lasts (start short, increase slowly)
// ======================

const unsigned long MAX_BURN_MS = 8000;   // safety cap, no burn can ever exceed this

bool done = false;
int lastShown = -1;

void setup() {
  digitalWrite(FIRE_PIN, LOW);
  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);
  Serial.begin(9600);
  Serial.print("Firing in ");
  Serial.print(DELAY_SEC);
  Serial.println(" seconds. Hands clear.");
}

void loop() {
  unsigned long elapsed = millis() / 1000UL;

  if (!done && elapsed < DELAY_SEC) {
    int left = (int)(DELAY_SEC - elapsed);
    if (left != lastShown) {
      lastShown = left;
      Serial.println(left);
    }
  }

  if (!done && elapsed >= DELAY_SEC) {
    done = true;                          // set first so it can only fire once
    unsigned long ms = BURN_MS;
    if (ms > MAX_BURN_MS) ms = MAX_BURN_MS;
    Serial.println("FIRE");
    digitalWrite(FIRE_PIN, HIGH);
    delay(ms);
    digitalWrite(FIRE_PIN, LOW);
    Serial.println("Done. Idle. Reset the board to run again.");
  }
}
