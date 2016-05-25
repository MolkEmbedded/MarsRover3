/*+++++++++++++++++++++++++++++++++++++++++++++++++++
Author      : Fahmi Ghani
Date        : 5 July 2015 
Project     : LCD Menu Function
Component   : LCD Keypad shield
Description : Simple Menu function on LCD
              Button Function:
                Up/Down - change Menu Page
                Select - enter page
                Left - Exit

Video Link: http://youtu.be/cMqif5ICS5M

+++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 


#define Cancel 3
#define NavDown 4
#define NavUp 5
#define Enter 6

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 4;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  MainMenuDisplay();
  delay(1000);
}
void loop()
{
    btn_push = ReadKeypad();
    
    MainMenuBtn();
    
    if(btn_push == 'S')//enter selected menu
    {
   //     WaitBtnRelease();
        switch (mainMenuPage)
        {
            case 1:
              MenuA();
              break;
            case 2:
              MenuB();
              break;
            case 3:
              MenuC();
              break;
            case 4:
              MenuD();
              break;
        }

          MainMenuDisplay();
    }
    


    delay(10);
  
}

/***Funktioner***/
void MenuA()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu A");
    
    while(ReadKeypad()!= 'C')
    {
        //Gör något skoj här
       
    }
}
void MenuB()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu B");
    
    while(ReadKeypad()!= 'C')
    {
        //Gör något skoj här
       
    }
}
void MenuC()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu C");
    
    while(ReadKeypad()!= 'C')
    {
        //Gör något skoj här
       
    }
}
void MenuD()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu D");
    
    while(ReadKeypad()!= 'C')
    {
        //Gör något skoj här
       
    }
}

void MainMenuDisplay()
{
    lcd.clear();
    lcd.setCursor(0,0);
    switch (mainMenuPage)
    {
        case 1:
          lcd.print("1. Menu A");
          break;
        case 2:
          lcd.print("2. Menu B");
          break;
        case 3:
          lcd.print("3. Menu C");
          break;
        case 4:
          lcd.print("4. Menu D");
          break;
    }
}

void MainMenuBtn()
{
    if(btn_push == 'U')
    {
        mainMenuPage++;
        if(mainMenuPage > mainMenuTotal)
          mainMenuPage = 1;
    }
    else if(btn_push == 'D')
    {
        mainMenuPage--;
        if(mainMenuPage == 0)
          mainMenuPage = mainMenuTotal;    
    }
    
    if(mainMenuPage != mainMenuPageOld) //only update display when page change
    {
        MainMenuDisplay();
        mainMenuPageOld = mainMenuPage;
    }
}

char ReadKeypad()
{
   int val1 = digitalRead(Cancel);
   int val2 = digitalRead(NavDown);
   int val3 = digitalRead(NavUp);
   int val4 = digitalRead(Enter);
   delay(200);
    if (val1 == 1)
      return 'C';
    else if (val2 == 1)
      return 'D';
    else if (val3 == 1)
      return 'U';
    else if (val4 == 1)
      return 'S';  
  }




