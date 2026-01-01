#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// LCD (häufig 0x27 oder 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RC522 Pins (Nano)
#define SS_PIN  10   // RC522 SDA/SS
#define RST_PIN 9    // RC522 RST
MFRC522 mfrc522(SS_PIN, RST_PIN);

void lcdClearLine(byte row) {
  lcd.setCursor(0, row);
  lcd.print("                "); // 16 Leerzeichen
}

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("NFC bereit...");
  lcd.setCursor(0, 1);
  lcd.print("Tag anhalten");

  SPI.begin();
  mfrc522.PCD_Init();
}

void printUIDToLCD() {
  // Zeilen leeren
  lcdClearLine(0);
  lcdClearLine(1);

  // Zeile 1 startet mit "UID:"
  lcd.setCursor(0, 0);
  lcd.print("UID:");

  // Wir bauen die UID als Text und verteilen sie auf 2 Zeilen (16 Zeichen je Zeile)
  // Format: DE AD BE EF (mit Leerzeichen)
  byte col = 4;   // nach "UID:" auf Zeile 0
  byte row = 0;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Wechsel in Zeile 2, wenn Zeile 1 voll ist
    // Pro Byte brauchen wir 2 Hex-Zeichen + 1 Leerzeichen (außer evtl. am Ende)
    if (row == 0 && col >= 14) {   // ab hier wird's eng, also in Zeile 2
      row = 1;
      col = 0;
    }
    if (row == 1 && col >= 16) {
      // LCD voll -> abbrechen (bei sehr langen UIDs)
      break;
    }

    // Byte als 2-stellige HEX-Ausgabe
    byte b = mfrc522.uid.uidByte[i];

    lcd.setCursor(col, row);
    if (b < 0x10) lcd.print("0");
    lcd.print(b, HEX);

    col += 2;

    // Leerzeichen zwischen Bytes (wenn noch Platz)
    if (row == 0 && col < 16) {
      lcd.setCursor(col, row);
      lcd.print(" ");
      col += 1;
    } else if (row == 1 && col < 16) {
      lcd.setCursor(col, row);
      lcd.print(" ");
      col += 1;
    }
  }
}

void loop() {
  // Warten auf neue Karte
  if (!mfrc522.PICC_IsNewCardPresent()) return;

  if (!mfrc522.PICC_ReadCardSerial()) return;

  printUIDToLCD();

  // Karte sauber beenden
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  // kleine Pause, damit es nicht dauernd neu schreibt
  delay(800);
}