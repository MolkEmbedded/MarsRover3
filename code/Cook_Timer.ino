#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//LCD I2C Pinout
#define I2C_ADDR    0x27 // I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//Pinout
#define navDownPin 8
#define navUpPin 9
#define navEnterPin 10
#define navBackPin 11
#define ledBluePin 7
#define buzzerPin 6

//Variables
short funcKey = 0000;

short navDown = 0;
short navUp = 0;
short navEnter = 0;
short navBack = 0;
short navView = 0;
short cookTime = 0;

short menuSelect = 0;
String menuSelectString;
float cookSize = 0.00;
float steakFactor = 0.00;
float doneness = 0.00;

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

//Functions
void startSequence();
void cookMenu();            //funcKey 0000
void cookSelectSize();      //funcKey 0001
void donenessSelect();      //funcKey 0002
int cookTimer();            //funcKey 0003
int cookTimerEndside();    //funcKey 0004
void ledBlinkShort();
void timerAlarm();


void setup()
{
  //LCD Setup
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  //Pinmodes
  pinMode(ledBluePin, OUTPUT);

  startSequence();

}

void loop()
{
  if (funcKey == 0000)
  {
    cookMenu();
    delay(150);
  }
  if (funcKey == 0001)
  {
    cookSelectSize();
    delay(150);
  }
  if (funcKey == 0002)
  {
    donenessSelect();
    delay(150);
  }
}

void startSequence()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("iEM Systems");
  lcd.setCursor(0, 1);
  lcd.print("****************");
  delay(1500);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Kitchen Timer");
  lcd.setCursor(0, 1);
  lcd.print("****************");
  delay(1500);
}

void cookMenu()
{
  navView = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose type:");

  while (navEnter == 0)
  {
    navDown = digitalRead(navDownPin);
    navUp = digitalRead(navUpPin);
    navEnter = digitalRead(navEnterPin);
    navBack = digitalRead(navBackPin);

    if (navDown == HIGH && navView != 0)
    {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      navView--;
      delay(200);
    }
    if (navUp == HIGH && navView != 4)
    {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      navView++;
      delay(200);
    }
    if (navView == 0)
    {
      menuSelectString = "Beef";
      steakFactor = 2.75;
      lcd.setCursor(0, 1);
      lcd.print("Beef");
    }
    if (navView == 1)
    {
      menuSelectString = "Pork";
      steakFactor = 3.50;
      lcd.setCursor(0, 1);
      lcd.print("Pork");
    }
    if (navView == 2)
    {
      menuSelectString = "Chicken";
      steakFactor = 3.65;
      lcd.setCursor(0, 1);
      lcd.print("Chicken");
    }
    if (navView == 3)
    {
      menuSelectString = "Fish";
      steakFactor = 2.50;
      lcd.setCursor(0, 1);
      lcd.print("Fish");
    }
    if (navView == 4)
    {
      menuSelectString = "Venison";
      steakFactor = 2.20;
      lcd.setCursor(0, 1);
      lcd.print("Venison");
    }
  }
  if (navEnter == 1)
  {
    menuSelect = navView;
    navEnter = 0;
    funcKey = 0001;
    return;
  }
}

void cookSelectSize()
{
  navEnter = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(menuSelectString);
  lcd.print(" in cm");

  while (navEnter == 0 && navBack == 0)
  {
    navDown = digitalRead(navDownPin);
    navUp = digitalRead(navUpPin);
    navEnter = digitalRead(navEnterPin);
    navBack = digitalRead(navBackPin);

    lcd.setCursor(0, 1);
    lcd.print(cookSize);

    if (navDown == HIGH && cookSize != 0.00)
    {
      cookSize = cookSize - 0.25;
      delay(150);
      lcd.setCursor(0, 1);
      lcd.print(cookSize);
    }
    if (navUp == HIGH && cookSize != 5.00)
    {
      cookSize = cookSize + 0.25;
      delay(150);
      lcd.setCursor(0, 1);
      lcd.print(cookSize);
    }
  }
  if (navEnter == 1)
  {
    navEnter = 0;
    funcKey = 0002;
    return;
  }
  if (navBack = 1)
  {
    navBack = 0;
    funcKey = 0000;
    delay(150);
    return;
  }
}

void donenessSelect()
{
  navEnter = 0;
  navView = 1;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Doneness ");
  lcd.setCursor(9, 1);

  while (navEnter == 0 && navBack == 0)
  {
    navDown = digitalRead(navDownPin);
    navUp = digitalRead(navUpPin);
    navEnter = digitalRead(navEnterPin);
    navBack = digitalRead(navBackPin);

    if (navDown == HIGH && navView != 0)
    {
      navView--;
      delay(200);
    }
    if (navUp == HIGH && navView != 2)
    {
      navView++;
      delay(200);
    }
    if (navView == 0)
    {
      doneness = 0.80;
      lcd.setCursor(9, 1);
      lcd.print("Rare  ");
    }
    if (navView == 1)
    {
      doneness = 1.0;
      lcd.setCursor(9, 1);
      lcd.print("Medium");
    }
    if (navView == 2)
    {
      doneness = 1.20;
      lcd.setCursor(9, 1);
      lcd.print("Well  ");
    }
  }
  if (navEnter == 1)
  {
    navEnter = 0;
    funcKey = 0000;
    if (cookTimer() == 0) {
      funcKey = 0000;
      return;
    }
    else
    {
      funcKey = 0002;
      return;
    }

  }
  if (navBack = 1)
  {
    navBack = 0;
    funcKey = 0001;
    delay(150);
    return;
  }
}

int cookTimer()
{
  short mins = 0;
  short secs = 0;

  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(150);   //Realese navEnter delay

  while (navEnter == 0)
  {
    navEnter = 0;
    lcd.setCursor(0, 0);
    lcd.print(menuSelectString);
    lcd.print(" ");
    lcd.print(cookSize);
    lcd.print("cm");

    cookTime = cookSize * steakFactor * 60.00 * doneness; //Steak Factor * 1 minute * Doneness
    mins = cookTime / 60;
    secs = cookTime % 60;
    lcd.setCursor(0, 1);
    lcd.print("Time ");
    lcd.print(mins);
    lcd.print(":");
    lcd.print(secs);
    lcd.print("min        ");

    navEnter = digitalRead(navEnterPin);
    navBack = digitalRead(navBackPin);

    if (navBack == 1)
    {
      navBack = 0;
      delay(150);
      return 1;
    }
  }


  while (navEnter == 1 && navBack == 0)
  {
    ledBlinkShort();

    for (cookTime; cookTime > 0; cookTime--)
    {
      mins = cookTime / 60;
      secs = cookTime % 60;
      lcd.setCursor(0, 1);
      lcd.print("Time ");
      lcd.print(mins);
      lcd.print(":");
      lcd.print(secs);
      lcd.print("min      ");
      delay(1000);
      navBack = digitalRead(navBackPin);
      if (navBack == 1)
      {
        navBack = 0;
        ledBlinkShort();
        delay(150);
        return 1;
      }
    }
    timerAlarm("Switch side-->");
    if (cookTimerEndside() == 0) {
      return 0;
    }
    else
    {
      return 1;
    }
  }
}

int cookTimerEndside()
{
  short mins = 0;
  short secs = 0;

  cookTime = cookSize * (steakFactor * 0.80) * 60.00 * doneness; //Steak Factor Endside * 1 minute * Doneness
  mins = cookTime / 60;
  secs = cookTime % 60;
  lcd.setCursor(0, 1);
  lcd.print("Time ");
  lcd.print(mins);
  lcd.print(":");
  lcd.print(secs);
  lcd.print("min        ");

  for (cookTime; cookTime > 0; cookTime--)
  {
    mins = cookTime / 60;
    secs = cookTime % 60;
    lcd.setCursor(0, 1);
    lcd.print("*Time ");
    lcd.print(mins);
    lcd.print(":");
    lcd.print(secs);
    lcd.print("min     ");
    delay(1000);
    navBack = digitalRead(navBackPin);
    if (navBack == 1)
    {
      navBack = 0;
      ledBlinkShort();
      delay(150);
      return 1;
    }
  }
  timerAlarm(" --Steak Done-- ");
  return 0;
}

void ledBlinkShort()
{
  digitalWrite(ledBluePin, HIGH);
  tone(buzzerPin, 3000, 300);
  delay(50);
  digitalWrite(ledBluePin, LOW);
  delay(200);
  digitalWrite(ledBluePin, HIGH);
  delay(50);
  digitalWrite(ledBluePin, LOW);
}

void timerAlarm(String lcdPrint)
{
  short i = 0;

  navEnter = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(lcdPrint);

  while (navEnter == 0)
  {
    navEnter = digitalRead(navEnterPin);

    digitalWrite(ledBluePin, HIGH);
    tone(buzzerPin, 4000, 50);
    delay(50);
    digitalWrite(ledBluePin, LOW);
    delay(200);
    digitalWrite(ledBluePin, HIGH);
    tone(buzzerPin, 4000, 50);
    delay(50);
    digitalWrite(ledBluePin, LOW);
    delay(200);
    i++;
    if (i >= 100)
    {
      navEnter = 1;
    }
  }

  if (navEnter == 1)
  {
    ledBlinkShort();
    navEnter = 0;
  }
}

