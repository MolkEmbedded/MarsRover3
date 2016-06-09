#define F_CPU 16000000UL
#include <stdio.h>
#include<avr/io.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include <compat/twi.h>
#include <IRremote.h>

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

#define BZR 13

#define FORWARD 1
#define BACKWARD 2
#define RIGHT 3
#define LEFT 4
#define STOP 5
#define MANUAL 7
#define AUTONOM 8

void TWI_start(void);
void TWI_init_master(void);

ISR(TWI_vect);
int IrFunction(void);
void playFanfare(void);
void playObstacleMelody(void);
void playStart(void);


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

unsigned char address=0x40, read=1, write=0;
unsigned char data, write_data=0x01, recv_data;
int start = 0;
int val = 0;


  int RECV_PIN = 6;

  IRrecv irrecv(RECV_PIN);

  decode_results results;

  
 void setup(){
    irrecv.enableIRIn();
    Serial.begin(9600);
    TWI_init_master();
}
 
void loop(){
  if (start){
    playStart();
    start = 0;
  }
  if (irrecv.decode(&results)) {
      irrecv.resume();
      val = 1;
  }
  if (val == 1){
    sei(); 
    TWI_start(); // Function to send start condition
   }
  }

int IrFunction(void){

  if (results.value == 0x4658 || results.value == 0x14658)
    return FORWARD;
  else if (results.value == 0x4659 || results.value == 0x14659)
    return BACKWARD;
  else if (results.value == 0x465B || results.value == 0x1465B)
    return RIGHT;
  else if (results.value == 0x465A || results.value == 0x1465A)
    return LEFT;
  else if (results.value == 0x465C || results.value == 0x1465C)  
    return STOP;
  else if (results.value == 0x469A || results.value == 0x1469A)  
    return MANUAL;
  else if (results.value == 0x469C || results.value == 0x1469C){
    start = 1;
    return AUTONOM;
  }
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
      val = 0;
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
      break;
    case 0x28:
      data = IrFunction();    
      TWDR = data;  // put data in TWDR
      val = 0;
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
    int noteDuration = 2000 / noteDurations[thisNote];
    tone(BZR, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BZR);
  }
}
