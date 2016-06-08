/*
 * IncFile1.h
 *
 * Created: 2016-05-10 13:24:15
 *  Author: hasse
 */ 
#include <avr/io.h>

#ifndef INCFILE1_H_
#define INCFILE1_H_

void UART_Init(unsigned int baud);
uint8_t UART_Recieve(void);
void UART_Send(unsigned char data);
void Uart_Flush(void);
void Error(unsigned int errorcode);



#endif /* INCFILE1_H_ */