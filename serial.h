#ifndef __SERIAL__
#define __SERIAL__

#include <avr/interrupt.h>

#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#define false 0
#define true 1
#define STARTMARKER '<'
#define ENDMARKER '>'
#define BACKSPACE 0x08
#define CMD_MAX_LENGHT 20

unsigned char cmd[CMD_MAX_LENGHT];
unsigned char cmd_updated[CMD_MAX_LENGHT];

void USARTCharTransmit(unsigned char tx);
void USART_Transmit( char *data );

#endif