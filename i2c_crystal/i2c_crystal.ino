#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <NewPing.h>
#define trig 2
#define echo 3
NewPing sothyro(trig, echo, 100);
Servo servo;
int servoPin = 22;
int angle = 0;
int plastic = 0;
int cans = 0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
String number = "";
char keys[ROWS][COLS] = {
  {'1', '2', '+', '3'},
  {'4', '5', 'B', '6'},
  {'7', '8', 'C', '9'},
  {'*', '0', 'D', '#'}
};
SoftwareSerial mySerial(12, 11);
byte rowPins[ROWS] = { 4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 2);
  lcd.print("PH number input :");
  mySerial.begin(9600);
}


void loop() {


  int cool = sothyro.ping_cm();
  char key = keypad.getKey();
  Serial.print(key);
  lcd.setCursor(0, 0);
  lcd.print("Plastic :");
  lcd.print(plastic);
  lcd.setCursor(0, 1);
  lcd.print("Cans : ");
  lcd.print(cans);


  if (cool < 5 && cool >= 1) {
    for (angle = 90; angle <= 180; angle += 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 180; angle >= 90; angle -= 30) {
      servo.write(angle);
    }
    plastic += 1;
  }
  else if (cool >= 5 && cool < 10) {
    for (angle = 90; angle >= 0; angle -= 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 0; angle <= 90; angle += 30) {
      servo.write(angle);
    }
    cans += 1;
  }


  if (key == '#') // we check if someone pressed #
  {
    // enter the phone number
    number = Take_input();

  }

  // BUG: fix bug
  if (key == 'C')
  {
    bool success = send_sms(number);
    if (success == true)
    {
      plastic = 0;
      cans = 0;
    }
  }

  delay(1000);
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
  return num;
}
void print_content (String str)
{
  lcd.setCursor(0, 3);
  lcd.print(str);
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
