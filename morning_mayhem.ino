/// LIBRARIES ///
#include <DHT11.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleRotary.h>

/// PINOUTS / MACROS ///
#define SIRENPIN 32
#define PUMPPIN 33
#define LEDSTRIPPIN 25
#define BTNLEDPIN 26

#define KNOBCLKPIN 17
#define KNOBDTPIN 16
#define KNOBSWPIN 4

#define BUZZERPIN 13
#define BUTTONPIN 23
#define H2OSENSEPIN 19
#define DHT11PIN 27

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_ADDR 0x27

/// OBJECT INITS ///
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
SimpleRotary rotary(KNOBCLKPIN, KNOBDTPIN, KNOBSWPIN);

/// VARIABLES ///
unsigned long lastMillis = 0;
int waveStep = 0;
bool preBtn = false;
bool preState = false;
int preRDir = 0;
int preRBtn = 0;

/// SETUP ///
void setup() {
  Serial.begin(115200);

  /// PINMODES ///
  pinMode(SIRENPIN, OUTPUT);
  pinMode(PUMPPIN, OUTPUT);
  pinMode(LEDSTRIPPIN, OUTPUT);
  pinMode(BTNLEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  pinMode(H2OSENSEPIN, INPUT);

  pinMode(KNOBCLKPIN, INPUT);
  pinMode(KNOBDTPIN, INPUT);
  pinMode(KNOBSWPIN, INPUT_PULLUP);

  /// LCDMENU ///
  lcd.init();
  lcd.backlight();

  lcd.print("Hello World!");
}

/// LOOP ///
void loop() {
  int rDir = rotary.rotate();
  int rBtn = rotary.push();

  if (rDir != 0) {
    preRDir = rDir;
  }
  if (rBtn == 1) {
    preRBtn = 1;
  }

  bool btn = !digitalRead(BUTTONPIN);
  bool h2o = !digitalRead(H2OSENSEPIN);

  if (btn != preBtn) {
    if (btn == true) {
      digitalWrite(SIRENPIN, !preState);
      digitalWrite(PUMPPIN, !preState);
      digitalWrite(LEDSTRIPPIN, !preState);
      digitalWrite(BTNLEDPIN, !preState);

      preState = !preState;
    }
    
    preBtn = btn;
  }

  if (millis() - lastMillis >= 0) {
    Serial.print("rDir:");
    Serial.print(preRDir == 2 ? -1 : preRDir);
    Serial.print(",rBtn:");
    Serial.print(preRBtn);
    Serial.print(",btn:");
    Serial.print(btn);
    Serial.print(",h2o:");
    Serial.println(h2o);

    preRDir = 0;
    preRBtn = 0;
  }

  lastMillis = millis();
}