
#include <SPI.h>
#include <MFRC522.h>



#define BUTTON_PIN 2
// RC522
#define SS_PIN  10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// >>> HIER RESET-TAG UID EINTRAGEN <<<

const byte uidLen = 4;

const int jokerPoints = 250;



//Score starts with some value and is reduced if opponent players achieve a hit on your device
const long resetScoreValue = 4000;
long score = resetScoreValue;

//Fuse is the amount of seconds a player most hold the button to land a hit
const long resetFuseValue = 10;
long fuse = resetFuseValue;

const unsigned long interval = 1000;
unsigned long lastTick = 0;

//Immunity is the min amount of milliseconds which have to pass between two hits
const unsigned long immunityMillis = 10L * 60L * 1000L; //10 minute immunity
unsigned long lastHit = 0;
const int hitPoints = 50;

bool locked = false;


bool isMatchingUid(byte expected[], byte actual[]) {  
  for (byte i = 0; i < uidLen; i++) {
    if (expected[i] != actual[i]) return false;
  }
  return true;
}


void resetScore() {
  score = resetScoreValue;
  fuse = resetFuseValue;
  showScore();
  showInSecondRowWithPadding("Reset OK");
  delay(800);
}

void toggleLock() {
  locked = !locked;

  if (locked) {
    showInSecondRowWithPadding("GESPERRT");
  }
}

void redeemJoker(byte joker[]) {

  // remove joker from list
  invalidateJoker(joker);

  score += jokerPoints;  
  showInSecondRowWithPadding(String("+") + jokerPoints + " Punkte!" );
  
}


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Taster nach GND

  lcdSetup();

  SPI.begin();
  mfrc522.PCD_Init();

  resetScore();
}

void loop() {
  showScore();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    if (mfrc522.uid.size != uidLen) return ;

    if (isResetTag(mfrc522.uid.uidByte)) {
      resetScore();
    } else if (isLockTag(mfrc522.uid.uidByte)) {
      toggleLock();
    } else if (isJokerTag(mfrc522.uid.uidByte)) {
      redeemJoker(mfrc522.uid.uidByte);
    } else {
      showInSecondRowWithPadding("Falscher Tag");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(800);
  }

  if (locked) {
    return;
  }


  // Taster gedrückt? (wegen Pullup ist gedrückt = LOW)
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if(score <= 0) {
    return;
  }

  unsigned long now = millis();

  if(lastHit != 0 && ((now - lastHit) <= immunityMillis)) {
    showImmunity(immunityMillis - (now - lastHit));
    return;
  }

  if (pressed) {
    showFuse();
    
    // alle 200ms einen Punkt abziehen
    if (now - lastTick >= interval) {

      fuse--;

      if(fuse == 0) {
        lastHit = millis();
        score = score - hitPoints;
        fuse = resetFuseValue;
        showScore();
      }

      lastTick = now;
    }
  } else {
    fuse = resetFuseValue;
    showEmptyRow();
    
    // optional: Timer "sauber" setzen, damit beim erneuten Drücken
    // nicht sofort ein Punkt abgezogen wird
    lastTick = millis();
  }
}