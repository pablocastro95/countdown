#include <LiquidCrystal_I2C.h>


// Häufige Adressen: 0x27 oder 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);  // auch wenn du nur 1 Zeile nutzt

const int buttonPin = 2;

long score = 4000;
const unsigned long interval = 200;   // 200 ms pro Punkt
unsigned long lastTick = 0;

void showScore() {
  lcd.setCursor(0, 0);
  lcd.print("Punkte: ");
  lcd.print(score);

  // Rest der Zeile "löschen", falls vorher mehr Ziffern da waren
  lcd.print("        ");
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Taster nach GND

  lcd.init();
  lcd.backlight();

  showScore();
}

void loop() {
  // Taster gedrückt? (wegen Pullup ist gedrückt = LOW)
  bool pressed = (digitalRead(buttonPin) == LOW);

  if (pressed && score > 0) {
    unsigned long now = millis();

    // alle 200ms einen Punkt abziehen
    if (now - lastTick >= interval) {
      lastTick = now;

      score--;
      showScore();
    }
  } else {
    // optional: Timer "sauber" setzen, damit beim erneuten Drücken
    // nicht sofort ein Punkt abgezogen wird
    lastTick = millis();
  }
}