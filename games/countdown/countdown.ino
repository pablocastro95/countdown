#include <SPI.h>
#include <MFRC522.h>


#define BUTTON_PIN 2
// RC522
#define SS_PIN  10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);


const byte uidLen = 4;

//Score starts with some value and is reduced if opponent players achieve a hit on your device
const long resetScoreValue = 1500;
long score = resetScoreValue;

//Fuse is the amount of seconds a player most hold the button to land a hit
const long resetFuseValue = 5;
long fuse = resetFuseValue;

const unsigned long interval = 1000;
unsigned long lastTick = 0;

//Immunity is the min amount of milliseconds which have to pass between two hits
const unsigned long immunityMillis = 10L * 60L * 1000L; //10 minute immunity
unsigned long immunityEndMillis = 0;
const int hitPoints = 30;

bool locked = false;

int fuseExtensionFactor = 1;

const uint8_t MAX_USED_UIDS = 60;   // je nach Anzahl Tags anpassen (RAM!)


byte usedUids[MAX_USED_UIDS][uidLen];
uint8_t usedUidCount = 0;


bool isMatchingUid(byte expected[], byte actual[]) {  
  for (byte i = 0; i < uidLen; i++) {
    if (expected[i] != actual[i]) return false;
  }
  return true;
}

bool isUidUsed(const byte uid[]) {
  for (uint8_t i = 0; i < usedUidCount; i++) {
    if (isMatchingUid(usedUids[i], uid)) return true;
  }
  return false;
}

bool rememberUid(const byte uid[]) {
  if (usedUidCount >= MAX_USED_UIDS) return false; // Liste voll
  for (byte i = 0; i < uidLen; i++) usedUids[usedUidCount][i] = uid[i];
  usedUidCount++;
  return true;
}

void clearUsedUids() {
  usedUidCount = 0;
}

bool isOneTimeTag(byte uid[]) {
  return isThirtyPointTag(uid)
      || isTenPointTag(uid)
      || isImmunityCancelTag(uid)
      || isImmunityTriggerTag(uid)
      || isFuseExtendTag(uid)
      || isMysteryTag(uid);
}


void reset() {
  score = resetScoreValue;
  fuse = resetFuseValue;
  immunityEndMillis = millis();
  locked = false;
  fuseExtensionFactor = 1;
  clearUsedUids();

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

long clampScore(long v) {
  if (v < 0) return 0;
  return v;
}

void applyScoreDelta(long delta) {
  score = clampScore(score + delta);
  showScore();
}

long doMystery() {
  // Werte: -50,-40,-30,-20,-10, +10,+20,+30,+40,+50
  int step = random(1, 6) * 10;     // 10..50
  int sign = random(0, 2) ? 1 : -1; // - oder +
  return (long)step * (long)sign;
}

String makePointChangeMessage(long points) {
  String sign = (points > 0) ? "+" : "";
  int absPts = abs(points);
  return sign + String(points) + " Punkte"; 
}

void resetFuse() {
  fuse = resetFuseValue * fuseExtensionFactor;
}




void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcdSetup();

  randomSeed(analogRead(A0));

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  showScore();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    if (mfrc522.uid.size != uidLen) return ;

    byte *uid = mfrc522.uid.uidByte;
    String message = "";

    if (isResetTag(uid)) {
      reset();
    } else if (isLockTag(uid)) {
      toggleLock();

    } else if (isOneTimeTag(uid)) {
      if (isUidUsed(uid)) {
        message = "Verbraucht";
      } else if (isImmunityCancelTag(uid)) {
        immunityEndMillis = millis();
        message = "Immunity AUS";
      } else if (isThirtyPointTag(uid)) {
        applyScoreDelta(30);
        message = makePointChangeMessage(30);
      } else if (isTenPointTag(uid)) {
        applyScoreDelta(10);
        message = makePointChangeMessage(10);
      } else if (isImmunityTriggerTag(uid)) {
        immunityEndMillis = millis() + immunityMillis;
        message = "Immunity AN";
      } else if (isFuseExtendTag(uid)) {
        fuseExtensionFactor = 2;
        message = "Fuse x2";
      } else if (isMysteryTag(uid)) {
        long delta = doMystery();
        applyScoreDelta(delta);
        message = makePointChangeMessage(delta);
      }
      rememberUid(uid);
    } else {
      message = "Falscher Tag";
    }
    showInSecondRowWithPadding(message);
    delay(800);
  }

  if (locked) {
    return;
  }


  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if(score <= 0) {
    return;
  }

  unsigned long now = millis();

  if(now < immunityEndMillis) {
    showImmunity(immunityEndMillis - now);
    return;
  }

  if (pressed) {
    showFuse();
    
    // alle sekunde einen Punkt abziehen
    if (now - lastTick >= interval) {

      fuse--;

      if(fuse == 0) {
        immunityEndMillis = now + immunityMillis;
        score -= hitPoints;
        fuseExtensionFactor = 1;
        resetFuse();
        showScore();
      }

      lastTick = now;
    }
  } else {
    resetFuse();
    showEmptyRow();
    
    // optional: Timer "sauber" setzen, damit beim erneuten Drücken
    // nicht sofort ein Punkt abgezogen wird
    lastTick = millis();
  }
}
