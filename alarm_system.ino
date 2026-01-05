#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Піни датчиків
#define DOOR 2
#define WINDOW1 3
#define WINDOW2 4

// Піни виходів
#define BUZZER 8
#define LED 9

// PIN-код
String password = "1234";
String input = "";

bool armed = false;

// Клавіатура 4x4
char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {5, 6, 7, 10};
byte colPins[4] = {11, 12, A0, A1};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void setup() {
  pinMode(DOOR, INPUT_PULLUP);
  pinMode(WINDOW1, INPUT_PULLUP);
  pinMode(WINDOW2, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("ARMING SYSTEM");
  lcd.setCursor(0,1);
  lcd.print("WAIT 10 SEC");

  delay(10000); // затримка на вихід
  armed = true;

  lcd.clear();
  lcd.print("SYSTEM ARMED");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      if (input == password) {
        armed = false;
        digitalWrite(BUZZER, LOW);
        digitalWrite(LED, LOW);
        lcd.clear();
        lcd.print("DISARMED");
      } else {
        lcd.clear();
        lcd.print("WRONG PIN");
        delay(1000);
      }
      input = "";
    } else {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
  }

  if (armed &&
     (digitalRead(DOOR) == HIGH ||
      digitalRead(WINDOW1) == HIGH ||
      digitalRead(WINDOW2) == HIGH)) {

    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    lcd.clear();
    lcd.print("!!! ALARM !!!");
  }
}
