#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void lcdSetup() {
  lcd.init();
  lcd.backlight();
}


void showScore() {
  showInFirstRowWithPadding(String("Punkte: ") + score);
}

void showImmunity(long remainingMillis) {
  int remainingSeconds = remainingMillis / 1000;
  int minutes = remainingSeconds / 60;
  int seconds = remainingSeconds % 60;

  String immunityText = "Noch ";

  if(minutes < 10) {
    immunityText += "0";
  }
  immunityText += minutes;
  immunityText += "m";

  if(seconds < 10) {
    immunityText += "0";
  }
  immunityText += seconds;
  immunityText += "s";

  showInSecondRowWithPadding(immunityText);
}

void showFuse() {
  String fuseText = "";

  if(fuse < 10) {
    fuseText += "0";
  }
 
  fuseText += fuse;

  showInSecondRowWithPadding(fuseText);
}

void showEmptyRow() {
  showInSecondRowWithPadding("");
}

void showInFirstRowWithPadding(String text) {
  showInRowWithPadding(0, text);
}

void showInSecondRowWithPadding(String text) {
 showInRowWithPadding(1, text);
}

void showInRowWithPadding(int row, String text) {
  lcd.setCursor(0, row);
  String output = text;
  int remainingChars =  16 - text.length();
  while (remainingChars > 0) {
      output = output + " ";
      remainingChars--;
  }
  lcd.print(output);
}