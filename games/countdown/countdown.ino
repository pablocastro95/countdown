#include <SPI.h>
#include <MFRC522.h>



#define BUTTON_PIN 2
// RC522
#define SS_PIN  10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

const byte uidLen = 4;
const int maxRedeemedUids = 64;
byte redeemedUids[maxRedeemedUids][uidLen] = {};
int redeemedUidCount = 0;

//Score starts with some value and is reduced if opponent players achieve a hit on your device
const long resetScoreValue = 800;
long score = resetScoreValue;

//Fuse is the amount of seconds a player most hold the button to land a hit
const long resetFuseValue = 5;
long fuse = resetFuseValue;

const unsigned long interval = 1000;
unsigned long lastTick = 0;

//Immunity is the min amount of milliseconds which have to pass between two hits
const unsigned long immunityMillis = 10L * 60L * 1000L; //10 minute immunity
unsigned long immunityEndMillis = 0;
const int hitPoints = 50;

const unsigned long attackBonusMillis = 10L * 60L * 1000L;
bool attackBonusActive = false;
unsigned long attackBonusEndMillis = 0;

const unsigned long halfHourDrainMillis = 30L * 60L * 1000L;
bool halfHourDrainActive = false;
unsigned long lastHalfHourDrainMillis = 0;

bool locked = false;




bool isMatchingUid(const byte expected[], const byte actual[]) {  
  for (byte i = 0; i < uidLen; i++) {
    if (expected[i] != actual[i]) return false;
  }
  return true;
}


void reset() {
  score = resetScoreValue;
  fuse = resetFuseValue;
  immunityEndMillis = millis();
  attackBonusActive = false;
  locked = false;

  //TODO Reset redeemed tags

  showScore();
  showInSecondRowWithPadding("Reset OK");
  delay(800);
}

void toggleLock() {
  locked = !locked;
}

void stopHalfHourDrain() {
  halfHourDrainActive = false;
}

void startHalfHourDrain() {
  halfHourDrainActive = true;
  lastHalfHourDrainMillis = millis();
}

void startAttackBonus() {
  attackBonusActive = true;
  attackBonusEndMillis = millis() + attackBonusMillis;
}

bool redeemTag(byte uid[]) {
  if (redeemedUidCount >= maxRedeemedUids) return false;
  memcpy(redeemedUids[redeemedUidCount],uid, 4);
  redeemedUidCount++;
  return true;
}

bool isRedeemedTag(byte uid[]) {
  for (int i = 0; i < redeemedUidCount; i++) {
    if (isMatchingUid(redeemedUids[i], uid)) return true; 
  }
  return false;
}


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(A0) ^ micros());

  lcdSetup();

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  showScore();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    if (mfrc522.uid.size != uidLen) return ;

    String message = "";

    if (isRedeemedTag(mfrc522.uid.uidByte)) {
      message = "Bereits eingeloest";
    } else if (isResetTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      reset();
    } else if (isLockTag(mfrc522.uid.uidByte)) {
      toggleLock();
      if (locked) {
        message = "GESPERRT";
      } else {
        message = "ENTSPERRT";
      }
    } else if (isTwentyPointTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      score += 20;  
      message = "+20 Punkte!";
      redeemTag(mfrc522.uid.uidByte);
    }else if (isFortyPointTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      score += 40;  
      message = "+40 Punkte!";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isCaesarTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      score += 250;
      message = "Caesar +250";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isJustitiaTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      score += 100;
      message = "Justitia +100";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isMinusFiftyTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      score -= 50;
      message = "-50 Punkte";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isAttackBonusTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      startAttackBonus();
      message = "+50 bei Treffer";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isHalfHourDrainTag(mfrc522.uid.uidByte)) {
      startHalfHourDrain();
      message = "-5 je 30min";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isImmunityCancelTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      immunityEndMillis = millis();
      message = "Immunitaet aufgehoben!";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isImmunityTriggerTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      immunityEndMillis = millis() + immunityMillis;
      message = "Immunitaet startet.";
      redeemTag(mfrc522.uid.uidByte);
    } else if (isFuseExtendTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      fuse += resetFuseValue;
      message = String("Countdown: ") + fuse;
      redeemTag(mfrc522.uid.uidByte);
    } else if (isMysteryTag(mfrc522.uid.uidByte)) {
      stopHalfHourDrain();
      int change = (random(21) - 10) * 10;
      score += change;
      if(change > 0) {
        message = String("+") + change + " Punkte";
      } else {
        message = String(change) + " Punkte";
      }
      redeemTag(mfrc522.uid.uidByte);
    } else {
      message = "Falscher Tag";
    }

    showInSecondRowWithPadding(message);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(800);
  }

  unsigned long now = millis();

  if(halfHourDrainActive && now - lastHalfHourDrainMillis >= halfHourDrainMillis) {
    score -= 5;
    lastHalfHourDrainMillis += halfHourDrainMillis;
    showScore();
    showInSecondRowWithPadding("-5 Punkte");
    delay(800);
  }

  if(attackBonusActive && now >= attackBonusEndMillis) {
    attackBonusActive = false;
  }

  if (locked) {
    return;
  }


  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if(score <= 0) {
    return;
  }

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
        if(attackBonusActive && now < attackBonusEndMillis) {
          score += 50;
          attackBonusActive = false;
        } else {
          score -= hitPoints;
        }
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
