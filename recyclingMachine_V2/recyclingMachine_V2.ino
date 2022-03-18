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

// timing
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds


//event triggering
int currentDistanceFlag = 0;
int waitTimerFlag = 0;
int previousDistanceFlag = 0;
int servoFlag = 0;

int cool = 0;

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  lcd.begin();
  lcd.clear();
  lcd.backlight();
}
void loop() {

  //  Serial.print("Plastic : ");
  //  Serial.print(plastic);
  //  Serial.print("Cans: ");
  //  Serial.print(cans);
  //Serial.print(key);
  //Serial.print(cool);
  //lcd.setCursor(0, 2);
  //lcd.print("Amount: ");
  //lcd.print(calculation);
  //lcd.print(" riels");
  lcd.setCursor(0, 0);
  lcd.print("Plastic :");
  lcd.print(plastic);
  lcd.setCursor(0, 1);
  lcd.print("Cans : ");
  lcd.print(cans);
  //delay(1000);
  //Serial.println(cool);

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    cool = sothyro.ping_cm();
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

    // check the state of the cool
    if (cool <= 5 && cool >= 1) {
      currentDistanceFlag = 1; // can
    }
    else if (cool <= 8 && cool >= 7) {
      currentDistanceFlag = 2; // plastic
    }
    else
    {
      currentDistanceFlag = 0;
    }
    
    // check the state
    if (currentDistanceFlag != previousDistanceFlag) {
      previousDistanceFlag = currentDistanceFlag;
      waitTimerFlag = 0;
    }
    else if (currentDistanceFlag == previousDistanceFlag) {
      waitTimerFlag++;

      if (waitTimerFlag == 3)
      {
        if (currentDistanceFlag != 0)
        {
          servoFlag = 1;
          waitTimerFlag = 0;
        }
        else
        {
          waitTimerFlag = 0;
        }
      }
    }

    Serial.print("Distance stat: ");
    Serial.print(cool);
    Serial.print(" currentDistanceFlag: ");
    Serial.print(currentDistanceFlag);
    Serial.print(" waitTimerFlag: ");
    Serial.println(waitTimerFlag);
  }

  if (servoFlag == 1 && currentDistanceFlag == 1) {
    Serial.println("Move Plastic");
    
    for (angle = 90; angle <= 180; angle += 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 180; angle >= 90; angle -= 30) {
      servo.write(angle);
    }

    plastic += 1;
    calculation += 10;

    servoFlag = 0;
    currentDistanceFlag = 0;
  }
  else if (servoFlag == 1 && currentDistanceFlag == 2) {
    Serial.println("Move Can");

    for (angle = 90; angle <= 180; angle += 30) {
      servo.write(angle);
    }
    delay(1500);
    for (angle = 180; angle >= 90; angle -= 30) {
      servo.write(angle);
    }

    cans += 1;
    calculation += 50;

    servoFlag = 0;
    currentDistanceFlag = 0;
  }

  // --- Check keypad input ----
  char key = keypad.getKey();
  if (key == '#') // we check if someone pressed #
  {
    Serial.println(key);
    // enter the phone number
    number = Take_input();

  }
}

String Take_input (void)
{
  String num = "+855";
  int len = 0;
  Serial.print(num);
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
        Serial.print(key);
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
  lcd.print("Money Sent");
  Serial.println("Money Sent");
  plastic = 0;
  cans = 0;
  calculation = 0;
  return num;
}
void print_content (String str)
{
  lcd.setCursor(0, 0);
  lcd.print(str);
}
