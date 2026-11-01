const int LED_PIN = 9;        // LED on pin 9 (swap to LED_BUILTIN to use the onboard LED)

// ===== EDIT THESE =====
const unsigned long DELAY_SEC = 10;       // seconds after power up before the LED turns on
const unsigned long ON_MS = 3000;         // how long the LED stays on
// ======================

bool done = false;
int lastShown = -1;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.print("LED on in ");
  Serial.print(DELAY_SEC);
  Serial.println(" seconds.");
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
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
    delay(ON_MS);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Done. Idle. Reset the board to run again.");
  }
}
