#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <NewPing.h>
#define trig 4
#define echo 2
NewPing sothyro(trig, echo, 100);
Servo servo;
int servoPin = 3;
int angle = 0;
int plastic = 0;
int cans = 0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
String number = "";
int calculation = 0;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = { 6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 12, 13}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  lcd.begin();
  lcd.clear();
  lcd.backlight();
}
void loop() {
  int cool = sothyro.ping_cm();
  char key = keypad.getKey();
  Serial.print(key);
  Serial.print(cool);
  lcd.setCursor(0, 2);
  lcd.print("Amount: ");
  lcd.print(calculation);
  lcd.print(" riels");
  lcd.setCursor(0, 0);
  lcd.print("Plastic :");
  lcd.print(plastic);
  lcd.setCursor(0, 1);
  lcd.print("Cans : ");
  lcd.print(cans);
  if (cool <= 3 && cool >= 1) {
    delay(1000);
    for (angle = 90; angle <= 180; angle += 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 180; angle >= 90; angle -= 30) {
      servo.write(angle);
    }   
    plastic += 1;
    calculation += 10;
  }
  else if (cool <= 8 && cool >= 7) {
    delay(1000);
    for (angle = 90; angle <= 180; angle += 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 180; angle >= 90; angle -= 30) {
      servo.write(angle);
    }   
    cans += 1;
    calculation += 50;
  }
    if (key == '#') // we check if someone pressed #
  {
    // enter the phone number
    number = Take_input();

  }
}

String Take_input (void)
{
  String num = "+855";
  int len = 0;
  while (len <= 13)
  {
    len = num.length();
    char key = keypad.getKey();
    if ((key != 'A') && (key != 'B') && (key != 'C') && (key != 'a'))
    {
      if ((key != '#') && (key != '*') && (key != 'D'))
      {
        num += String(key);
        print_content(num);
      }

      else if (key == '*')
      {
        num.setCharAt(len - 1, '*');
        print_content(num);
        num.remove(len - 1);
      }
    }
  }
  lcd.clear();
  lcd.setCursor(1,5);
  lcd.print("Money Sent");
  lcd.clear();
  plastic = 0;
  cans = 0;
  calculation =0;
  return num;
}
void print_content (String str)
{
  lcd.setCursor(0, 3);
  lcd.print(str);
}
