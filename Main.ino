const int LED_PIN = 9;
const int BUTTON_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {   // pressed
    digitalWrite(LED_PIN, HIGH);
    delay(3000);                          // stand-in for the 3 second burn
    digitalWrite(LED_PIN, LOW);
  }
}
