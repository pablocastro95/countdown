

const int buttonPin = 2;


void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Taster nach GND
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // Taster gedrückt? (wegen Pullup ist gedrückt = LOW)
  bool pressed = (digitalRead(buttonPin) == LOW);

  if (pressed) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}