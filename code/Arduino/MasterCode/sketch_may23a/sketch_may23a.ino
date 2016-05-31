
#define F_CPU 16000000UL
#include <stdio.h>
#include<avr/io.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include <compat/twi.h>
#include <IRremote.h>


#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27,20,4); 

#define Cancel 11
#define NavDown 9
#define NavUp 8
#define Enter 10


void TWI_start(void);
void TWI_init_master(void);

ISR(TWI_vect);


unsigned char address=0x40, read=1, write=0;
unsigned char data, write_data=0x01, recv_data;

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 3;

int IrFunction(void);
int RECV_PIN = 11;
int val1 = 0;
int val = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup(){
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(13, OUTPUT);
  irrecv.enableIRIn();
  TWI_init_master();
  
  startSequence();

  MainMenuDisplay();
  delay(1000);
}

void loop(){
    btn_push = ReadKeypad();
    
    MainMenuBtn();
    
    if(btn_push == 'S'){//enter selected menu
   //     WaitBtnRelease();
        switch (mainMenuPage){
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
void startSequence(){
  lcd.clear();
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

void Manual(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Manual Mode");
    
    while(ReadKeypad()!= 'C'){
      delay(200);
      digitalWrite(13, HIGH);
  
   if (irrecv.decode(&results)){
      irrecv.resume();
      val = 1;
     }
   if (val == 1){
      digitalWrite(13, LOW);
      sei(); 
      TWI_start(); // Function to send start condition
     }   
   }
}

void Autonom(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Autonom Mode");
    
    while(ReadKeypad()!= 'C'){
        //Gör något skoj här
    }
}
void Settings(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Settings");
    
    while(ReadKeypad()!= 'C'){
        //Gör något skoj här   
    }
}


void MainMenuDisplay(){
    lcd.clear();
    lcd.setCursor(0,0);
    
    switch (mainMenuPage){
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

void MainMenuBtn(){
    if(btn_push == 'U'){
        mainMenuPage++;
        if(mainMenuPage > mainMenuTotal)
          mainMenuPage = 1;
    }
    else if(btn_push == 'D'){
        mainMenuPage--;
        if(mainMenuPage == 0)
          mainMenuPage = mainMenuTotal;    
    }
    
    if(mainMenuPage != mainMenuPageOld){ //only update display when page change
        MainMenuDisplay();
        mainMenuPageOld = mainMenuPage;
    }
}

char ReadKeypad(){
   int val1 = digitalRead(Cancel);
   int val2 = digitalRead(NavDown);
   int val3 = digitalRead(NavUp);
   int val4 = digitalRead(Enter);
   
   delay(200);
   
    if (val1 == 0)
      return 'C';
    else if (val2 == 0)
      return 'D';
    else if (val3 == 0)
      return 'U';
    else if (val4 == 0)
      return 'S';  
  }




