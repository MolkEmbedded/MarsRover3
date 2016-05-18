#define F_CPU 16000000UL
#include <stdio.h>
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include <compat/twi.h>
#include <IRremote.h>


void TWI_start(void);
void TWI_repeated_start(void);
void TWI_init_master(void);
void TWI_write_address(unsigned char);
void TWI_read_address(unsigned char);
void TWI_write_data(unsigned char);
void TWI_read_data(void);
void TWI_stop(void);
ISR(TWI_vect);
int IrFunction(void);

unsigned char address=0x40, read=1, write=0;
unsigned char data, write_data=0x01, recv_data;
int val1 = 0;
int val = 0;


    int RECV_PIN = 11;

  IRrecv irrecv(RECV_PIN);

  decode_results results;

  
 void setup(){
  pinMode(13, OUTPUT);
    irrecv.enableIRIn();
    Serial.begin(9600, SERIAL_8N1);
    TWI_init_master();
 }
 
  
//int main(void)
//{
 
void loop(){
  delay(200);
  digitalWrite(13, HIGH);
  
  if (irrecv.decode(&results)) {
      irrecv.resume();
      val = 1;
  }
  if (val == 1){
    digitalWrite(13, LOW);
    sei(); 
    TWI_start(); // Function to send start condition
   }
  }

void TWI_init_master(void){ // Function to initialize master
  TWBR = 0x0B;                                  // Set bit rate register. Defined in header file.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|(1<<TWIE);   
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

void TWI_start(void){
  // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
  while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
}
