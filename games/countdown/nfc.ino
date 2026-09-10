bool isMatchingAnyUid(const byte expectedUids[][uidLen], int expectedUidCount, byte actualUid[]) {
  for (int i = 0; i < expectedUidCount; i++) {
    if (isMatchingUid(expectedUids[i], actualUid)) return true;
  }
  return false;
}

bool isResetTag(byte actualUid[]) {
  const byte resetUid[4] = { 0xF4, 0x77, 0x78, 0x00 }; 
  return isMatchingUid(actualUid, resetUid);
}

bool isLockTag(byte actualUid[]) {
  const byte lockUid[4] = { 0x8F, 0x85, 0xD3, 0x1F };
  return isMatchingUid(actualUid, lockUid);
}

bool isImmunityCancelTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x30, 0xCD, 0x2E, 0x5E },
    { 0xB0, 0x8D, 0x09, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 2, actualUid);
}

bool isFortyPointTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x20, 0x09, 0x27, 0x5E },
    { 0x50, 0x0E, 0x2A, 0x5E },
    { 0x00, 0xC5, 0x34, 0x5E },
    { 0x60, 0x5D, 0x0F, 0x5E },
    { 0x80, 0x09, 0x28, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 5, actualUid);
}

bool isCaesarTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0xB0, 0x40, 0x6F, 0x5D },
  };
  return isMatchingAnyUid(tagUids, 1, actualUid);
}

bool isJustitiaTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x50, 0x08, 0x3C, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 1, actualUid);
}

bool isHalfHourDrainTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x70, 0xAE, 0x1D, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 1, actualUid);
}

bool isMinusFiftyTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x30, 0x49, 0x36, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 1, actualUid);
}

bool isAttackBonusTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0xD0, 0xD4, 0x29, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 1, actualUid);
}

bool isTwentyPointTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0xE0, 0x92, 0x0F, 0x5E },
    { 0x60, 0xE7, 0x3C, 0x5E },
    { 0x10, 0x0F, 0x31, 0x5E },
    { 0x10, 0xB6, 0x10, 0x5E },
    { 0x80, 0x62, 0x30, 0x5E },
    { 0xD0, 0x3A, 0x37, 0x5E },
    { 0xA0, 0x41, 0x23, 0x5E },
    { 0x00, 0xD0, 0x3A, 0x5E },
    { 0x20, 0xAE, 0x21, 0x5E },
    { 0xC0, 0x6E, 0x0B, 0x5E },
    { 0x10, 0xC0, 0x28, 0x5E },
    { 0xC0, 0x03, 0x2A, 0x5E },
    { 0x70, 0xF1, 0x0D, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 13, actualUid);
}

bool isImmunityTriggerTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x80, 0xBE, 0x1F, 0x5E },
    { 0xC0, 0x13, 0x0E, 0x5E },
  };
  return isMatchingAnyUid(tagUids, 2, actualUid);
}

bool isFuseExtendTag(byte actualUid[]) {
  return false;
}

bool isMysteryTag(byte actualUid[]) {
  const byte tagUids[][uidLen] = {
    { 0x40, 0xCC, 0x24, 0x5E },
    { 0xC0, 0x41, 0xEA, 0x5D },
  };
  return isMatchingAnyUid(tagUids, 2, actualUid);
}
