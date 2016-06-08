#include "UART.h"
#include <avr/io.h>


/************************************************************************************

	This is init code and use code for using the UART in order to debug an AVR.

	This code is adapted to a 328p AVR, using 8Mhz internal oscillator. You will
	need to look over the code if you are using a different AVR or frequency.
	
	
	When you have updated the code to your AVR, use a USB to TTL cable. We got one 
	with every kit in datalogi so there should be enough cables. This is the cable:
	http://www.electrokit.com/ftdikabel-usb-ttl-5v.47364
	
	Connect the TX on the cable to RX pin on the AVR and then the RX pin 
	on the cable to the TX pin on the AVR. 
	
	You can use the program called RealTerm if you are using windows: 
	https://sourceforge.net/projects/realterm/ (start it in administration mode)
	

************************************************************************************/


void UART_Init(unsigned int baud){
	
	/*****************************************************************************/
	/* Set the transfer to double speed. It increases the risk of errors but 	 */
	/* will probably affect you only if you have a high baudrate/transfer speed. */
	/*****************************************************************************/
	UCSR0A = (1<<U2X0);
	
	
	/*****************************************************************************/
	/* Set the baudrate from the given number in the argument, like 9600 baud.	 */
	/* 																			 */
	/* The mathematical calculation is Baudrate = Frequency/prescaler/baud-1     */
	/* 																			 */
	/* You will find this also in your datasheet. If you have normal speed,		 */
	/* change the 8 for a 16 instead and it should work.                         */
	/*****************************************************************************/
	unsigned int baudrate;
	baudrate = 16000000/8/baud-1;
	// Double speed use frequency/8/baud else it is 16.
	
	// Writes the baudrate in the registry
	UBRR0H = (unsigned char) (baudrate>>8);
	UBRR0L = (unsigned char) (baudrate);

	
	
	/*****************************************************************************/
	/* Sets the different register, make sure that you should use the same       */
	/* registers.																 */
	/*****************************************************************************/
	// Enables Receive and Transmit over UART
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); 
	
	// Sets to 1 stop bit and 8 databits
	UCSR0C = (0<<USBS0) | (3<<UCSZ00);
	
	// Disables Parity
	UCSR0C |= (0<<UPM01) | (0<<UPM00);
	
}
/************************************************************************************

	UART Recieve

	This function is ment only to recieve one byte and returns it as an unsigned
	integer with one byte size. 	

************************************************************************************/
uint8_t UART_Recieve(void){
	
	// Wait for data to be received (wait for flag to be set)
	while(!(UCSR0A & (1<<RXC0)));
	
	// Returns the data from buffer
	return UDR0;
	
}

/************************************************************************************

	UART Send

	This function sends one unsigned byte when called. 	

************************************************************************************/
void UART_Send(unsigned char data){
	
	// Waits for the transmit buffer to be empty (wait for flag)
	while(!(UCSR0A & (1<<UDRE0)));
	
	// Put data in buffer and sends it
	UDR0 = data;

}

/************************************************************************************

	UART Flush

	This function clears the recieve buffer. Can be used to clear unwanted values
	in the UDR register. 

************************************************************************************/
void Uart_Flush(void){
	
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0))
		dummy = UDR0;
	
}

/************************************************************************************

	Error

	Use this in your debugging, sending messages over the UART to your computer
	in order to figure out where things didn't work. 
	
	Depending on the frequency, the lights will blink faster or slower. Change the 
	value 10000 to a higher one in order to slow the blinking and the rate that 
	it will send the errormessage over the UART. 

************************************************************************************/
void Error(unsigned int errorcode){
	
	//Flashes the red lights and send the errorcode through the UART
	unsigned long ticks = 0;
	for(;;){
		if(ticks%10000 == 0){
			//PINB = (1<<ERRORLED); // Turns the light on
			UART_Send(errorcode); // Send the message over UART
		}
		if(ticks%10000 == 50000){
			//PINB = (0<<ERRORLED); // Turns the light off
		}
		ticks++;
	}
}

