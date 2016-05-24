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
int IrFunction(void);

ISR(TWI_vect);

unsigned char address=0x40, read=1, write=0;
unsigned char data, write_data=0x01, recv_data;

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 3;

byte settingMenuPage = 1;
byte settingMenuPageOld = 1;
byte settingMenuTotal = 3;

int RECV_PIN = 12;
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
      val = 0;
      irrecv.enableIRIn();
      delay(200);
      digitalWrite(13, HIGH);
   if (irrecv.decode(&results)){
      irrecv.resume();
      val = 1;
     }
   if (val == 1){
      digitalWrite(13, LOW);
      sei(); 
      //TWI_start(); // Function to send start condition
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

    SettingMenuDisplay();
       
    while(ReadKeypad()!= 'C'){
        btn_push = ReadKeypad();
    
        SettingMenuBtn();
    
    if(btn_push == 'S'){//enter selected menu
   //     WaitBtnRelease();
        switch (settingMenuPage){
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

void Calibrate(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calibration Mode");
    
    while(ReadKeypad()!= 'C'){
        //Gör något skoj här
    }
}

void LightOn(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LightOn");
    
    while(ReadKeypad()!= 'C'){
        //Gör något skoj här
    }
}

void TEST(){  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TEST");
    
    while(ReadKeypad()!= 'C'){
        //Gör något skoj här
    }
}


void MainMenuDisplay(){
    lcd.clear();
    lcd.setCursor(0,0);
    digitalWrite(13, LOW);
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

void SettingMenuDisplay(){
    lcd.clear();
    lcd.setCursor(0,1);
    digitalWrite(13, LOW);
    switch (settingMenuPage){
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

void SettingMenuBtn(){
    if(btn_push == 'U'){
        settingMenuPage++;
        if(settingMenuPage > settingMenuTotal)
          settingMenuPage = 1;
    }
    else if(btn_push == 'D'){
        settingMenuPage--;
        if(settingMenuPage == 0)
          settingMenuPage = settingMenuTotal;    
    }
    
    if(settingMenuPage != settingMenuPageOld){ //only update display when page change
        SettingMenuDisplay();
        settingMenuPageOld = settingMenuPage;
    }
}


void sendtoRobot(){
  int val = 0;

  if(Autonom()){
    val = 1
    }
    return val;
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
  
int IrFunction(void){

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

ISR(TWI_vect){
  switch (TWSR)
  {
    case 0x08:
      data = 0x40; // Address till Slave
      TWDR = data;  
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
      break;
    case 0x18:
      data = IrFunction();
      TWDR = data;
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
      break;
    case 0x28:
      data = IrFunction();    
      TWDR = data;  // put data in TWDR
      TWCR =(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
      break;
    case 0x30:
      val = 0;
      TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)|(1<<TWIE);
      break;
    default:
      break;
  }
}

void TWI_init_master(void){ // Function to initialize master
  TWBR = 0x0B;                                  // Set bit rate register. Defined in header file.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|(1<<TWIE);   
}

void TWI_start(void){
  // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
  while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
}


