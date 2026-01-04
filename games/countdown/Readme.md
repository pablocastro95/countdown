### Boards
* Arduino ESP32 Boards 
* Arduino AVR Boards


### Libraries

* LiquidCrystal_I2C https://github.com/markub3327/LiquidCrystal_I2C
* MFRC522 https://github.com/miguelbalboa/rfid

### NFC Tags


bool isResetTag(byte actualUid[]) {
const byte resetUid[4] = { 0x67, 0xAB, 0xCD, 0xEF }; 
return isMatchingUid(actualUid, resetUid);
}

bool isLockTag(byte actualUid[]) {
const byte lockUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(actualUid, lockUid);
}

bool isImmunityCancelTag(byte actualUid[]) {
const byte immunityCancelUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(actualUid, immunityCancelUid);
}

bool isThirtyPointTag(byte actualUid[]) {
const byte tagUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(tagUid,actualUid);
}

bool isTenPointTag(byte actualUid[]) {
const byte tagUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(tagUid,actualUid);
}

bool isImmunityTriggerTag(byte actualUid[]) {
const byte tagUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(tagUid,actualUid);
}

bool isFuseExtendTag(byte actualUid[]) {
const byte tagUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(tagUid,actualUid);
}

bool isMysteryTag(byte actualUid[]) {
const byte tagUid[4] = { 0x67, 0xAB, 0xCD, 0xEF };
return isMatchingUid(tagUid,actualUid);
}

