#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047

#define F_CPU 16000000UL
#include <stdio.h>
#include<avr/io.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include <compat/twi.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * LCD LED_A pin to 5V
 * LCD LED_C pin to ground
*/

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define Cancel 7
#define NavDown 8
#define NavUp 9
#define Enter 10
#define BZR 13 //Buzzer, change pin.
//#define RECV_PIN 6

void TWI_start(void);
void TWI_init_master(void);
int IrFunction(void);
void playFanfare(void);
void playObstacleMelody(void);
void playStart(void);

ISR(TWI_vect);

// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G5,NOTE_G5, NOTE_G5, NOTE_G5, NOTE_C5,NOTE_C5, NOTE_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 16, 16, 4, 8, 16, 16, 4, 8, 16, 16, 8, 8, 4
};
int obstacleNoteDuration[] = {8,8};
int obstacleMelody[] = {NOTE_G5, NOTE_C5};
int startMelody[] = {NOTE_C5,0, NOTE_C5, 0, NOTE_C5, 0, NOTE_G5};
int startDuration[] = {8,8,8,8,8,8,2};

unsigned char address = 0x40, read = 1, write = 0;
unsigned char data, write_data = 0x01, recv_data;

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 3;

byte settingMenuPage = 1;
byte settingMenuPageOld = 1;
byte settingMenuTotal = 3;

int RECV_PIN = 6;
int val1 = 0;
int val = 0;
int modeState = 0;
int mode = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(BZR, OUTPUT); //Sets BZRpin to OUTPUT
  //pinMode(13, OUTPUT);
  irrecv.enableIRIn();
  TWI_init_master();

  startSequence();
  sei();
  MainMenuDisplay();
  delay(1000);
}

void loop() {
  btn_push = ReadKeypad();

  MainMenuBtn();

  if (btn_push == 'S') { //enter selected menu
    switch (mainMenuPage) {
      case 1:
        Manual();
        break;
      case 2:
        Autonom();
        break;
      case 3:
        Settings();
        break;
    }
    MainMenuDisplay();
  }
  delay(10);
}

/***Funktioner***/
void startSequence() {

  lcd.setCursor(2, 0);
  lcd.print("IS Robotics");
  lcd.setCursor(0, 1);
  lcd.print("****************");
  delay(2000);

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Project");
  lcd.setCursor(1, 1);
  lcd.print("Line-Follower");
  delay(2000);
}

void Manual() {
  // delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Manual Mode");
  lcd.setCursor(0, 1);
  lcd.print("Use IR remote");
  BuzzerTone();
  val = 0;
  //while(ReadKeypad()!= 'C'){
  while (1) {
    Serial.println(val);
    delay(100);
   // digitalWrite(13, HIGH);
   // Serial.println("1");

   if (irrecv.decode(&results)) {
      irrecv.resume();
      val = 1;
    }
    if (val == 1) {
  //    digitalWrite(13, LOW);
  Serial.println("HEJ");
    //  sei();
      TWI_start(); // Function to send start condition
    }
    /*
    Serial.println("Man");
    if (irrecv.decode(&results)){
      irrecv.resume();
     // mode = 8;
     Serial.println("HEJ");
      TWI_start(); // Function to send start condition
     // modeState = 1;
      }   
*/
  }
}

void Autonom() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Autonom Mode");
  lcd.setCursor(0, 1);
  lcd.print("GOGOGO!!!");
//  BuzzerTone();

  while (ReadKeypad() != 'C') {
    for( int i = 0; i < 1; i++); 
      playStart();
    }
}
void Settings() {
  BuzzerTone();

  SettingMenuDisplay();

  while (ReadKeypad() != 'C') {
    btn_push = ReadKeypad();

    SettingMenuBtn();

    if (btn_push == 'S') { //enter selected menu
      //     WaitBtnRelease();
      switch (settingMenuPage) {
        case 1:
          Calibrate();
          break;
        case 2:
          LightOn();
          break;
        case 3:
          TEST();
          break;
      }
      SettingMenuDisplay();
    }
    delay(10);
  }
}

void Calibrate() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration Mode");
  BuzzerTone();

  while (ReadKeypad() != 'C') {
    //Gör något skoj här
  }
}

void LightOn() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LightBlink");
  BuzzerTone();

  while (ReadKeypad() != 'C') {
   // digitalWrite(13, HIGH);
   // delay(1000);
    //digitalWrite(13, LOW);
    //delay(1000);
  }
}

void TEST() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEST");
  BuzzerTone();

  while (ReadKeypad() != 'C') {
    //Gör något skoj här
  }
}


void BuzzerTone() {
  for (int i = 0; i < 500; i++) { // generate a 1KHz tone for 1/2 second
    digitalWrite(BZR, HIGH);
    delayMicroseconds(500);
    digitalWrite(BZR, LOW);
    delayMicroseconds(500);
  }

}

void MainMenuDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  //digitalWrite(13, LOW);
  switch (mainMenuPage) {
    case 1:
      lcd.print("1. Manuell");
      break;
    case 2:
      lcd.print("2. Autonom");
      break;
    case 3:
      lcd.print("3. Settings");
      break;
  }
}

void MainMenuBtn() {
  if (btn_push == 'U') {
    mainMenuPage++;
    if (mainMenuPage > mainMenuTotal)
      mainMenuPage = 1;
  }
  else if (btn_push == 'D') {
    mainMenuPage--;
    if (mainMenuPage == 0)
      mainMenuPage = mainMenuTotal;
  }

  if (mainMenuPage != mainMenuPageOld) { //only update display when page change
    MainMenuDisplay();
    mainMenuPageOld = mainMenuPage;
  }
}

void SettingMenuDisplay() {
  lcd.clear();
  lcd.setCursor(0, 1);
 // digitalWrite(13, LOW);
  switch (settingMenuPage) {
    case 1:
      lcd.print("4. Calibrate");
      break;
    case 2:
      lcd.print("5. LightOn");
      break;
    case 3:
      lcd.print("6. TEST");
      break;
  }
}

void SettingMenuBtn() {
  if (btn_push == 'U') {
    settingMenuPage++;
    if (settingMenuPage > settingMenuTotal)
      settingMenuPage = 1;
  }
  else if (btn_push == 'D') {
    settingMenuPage--;
    if (settingMenuPage == 0)
      settingMenuPage = settingMenuTotal;
  }

  if (settingMenuPage != settingMenuPageOld) { //only update display when page change
    SettingMenuDisplay();
    settingMenuPageOld = settingMenuPage;
  }
}


char ReadKeypad() {
  int val1 = digitalRead(Cancel);
  int val2 = digitalRead(NavDown);
  int val3 = digitalRead(NavUp);
  int val4 = digitalRead(Enter);

  Serial.println("KEY");
  delay(100);

  if (val1 == 0)
    return 'C';
  else if (val2 == 0)
    return 'D';
  else if (val3 == 0)
    return 'U';
  else if (val4 == 0)
    return 'S';
}

int IrFunction(void) {

  if (results.value == 0x4658 || results.value == 0x14658)
    return 1;
  else if (results.value == 0x4659 || results.value == 0x14659)
    return 2;
  else if (results.value == 0x465B || results.value == 0x1465B)
    return 3;
  else if (results.value == 0x465A || results.value == 0x1465A)
    return 4;
  else if (results.value == 0x465C || results.value == 0x1465C)
    return 5;
  else
    return 0;

}

ISR(TWI_vect) {
  Serial.println(TWSR, HEX);
  switch (TWSR)
  {
    case 0x08:
      data = 0x40; // Address till Slave
      TWDR = data;
      TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWIE);
      break;
    case 0x18:
      // if (modeState == 1)
      data = IrFunction();
      //else
      TWDR = data;
     // Serial.println(TWDR);
      TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWIE);
      break;
    case 0x28:
      data = IrFunction();
      Serial.println(TWSR, HEX);
      TWDR = data;  // put data in TWDR
      TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWIE);
      break;
    case 0x30:
   //   Serial.println(TWSR, HEX);
      val = 0;
      TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWIE);
       //Serial.println(TWSR, HEX);
      break;
    case 0xF8:
    TWCR = (1 << TWINT)| (1 << TWEN) | (1 << TWIE);
      break;
     default:
     break;
  }
}

void TWI_init_master(void) { // Function to initialize master
  TWBR = 0x0B;                                  // Set bit rate register. Defined in header file.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1 << TWEN) | (1 << TWIE);
}

void TWI_start(void) {
  // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
 // while (!(TWCR & (1 << TWINT))); // Wait till start condition is transmitted
}
void playStart(){
  for( uint8_t thisNote = 0; thisNote < 7; thisNote++){
    Serial.println("HEJ");
    int noteDuration = 2000 / startDuration[thisNote];
    tone(BZR, startMelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BZR);
  }
}
void playObstacleMelody() {
  for( uint8_t thisNote = 0; thisNote < 2; thisNote++){
    int noteDuration = 1000 / obstacleNoteDuration[thisNote];
    tone(BZR, obstacleMelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BZR);
  }
}

void playFanfare(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 14; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 2000 / noteDurations[thisNote];
    tone(BZR, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BZR);
  }
}
