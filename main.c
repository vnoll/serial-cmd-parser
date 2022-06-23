/*
 * Simple Parsing Command from Serial for ATMEGA328
 * 
 * Created: 06/2022 12:48:10
 * Author : Valdir Noll, Dr. Eng.
 * Mechatronics - IFSC - Florianopolis/BR
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "serial.h"


extern volatile char RxIsComplete;
extern unsigned char cmd[CMD_MAX_LENGHT];
extern unsigned char cmd_updated[CMD_MAX_LENGHT];

#define NEWLINE 13
#define LED PORTB5

enum commands {LED_ON, LED_OFF, LED_BLINK, HELP, STOP_FUNCTION, INVALID};
enum commands CMD = STOP_FUNCTION;

#define CMD_LED_ON			"led-on"
#define CMD_LED_OFF			"led-off"
#define CMD_LED_BLINK		"led-blink"
#define CMD_HELP			"help"
#define CMD_DO_NOTHING		"stop"


void menu()
{
	USART_Transmit("---------------------------------------\r");
	USART_Transmit("| command     |  description          |\r");
	USART_Transmit("---------------------------------------\r");
	USART_Transmit("| <led-on>    |  LED on  B5 pin       |\r");
	USART_Transmit("| <led-off>   |  LED Off B5 pin       |\r");
	USART_Transmit("| <led-blink> |  LED blink B5 @ 1Hz   |\r");
	USART_Transmit("| <help>      |  Repeat this menu     |\r");
	USART_Transmit("---------------------------------------\r\r");
	USART_Transmit("cmd: ");
}

enum commands discoveryCommand(char *c)
{
	if (strcmp(c,CMD_HELP)==0)       return HELP;
	if (strcmp(c,CMD_LED_ON)==0)     return LED_ON;
	if (strcmp(c,CMD_LED_OFF)==0)    return LED_OFF;
	if (strcmp(c,CMD_LED_BLINK)==0)  return LED_BLINK;
	if (strcmp(c,CMD_DO_NOTHING)==0) return STOP_FUNCTION;
	return INVALID;
}

void setup()
{
		DDRB |= (1<<LED);
		PORTB &= ~(1<<LED);

		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);	
		UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);		
		UBRR0H = (BAUD_PRESCALE >> 8);
		UBRR0L = BAUD_PRESCALE;	
		
		sei();
		menu();
}

int main (void)
{
	enum commands cmd_local, cmd_local_last;	
	enum main_sequence {WAIT_RX_COMPLETE, DECODER_FUNCTION, EXECUTE_FUNCTION_LOOP};	
	enum main_sequence SEQUENCE = WAIT_RX_COMPLETE;
	
	cmd_local = STOP_FUNCTION;
	cmd_local_last = cmd_local;
	
	setup();
	
	while (1)
	{	
		switch (SEQUENCE)
		{
			case WAIT_RX_COMPLETE:
						
				if (RxIsComplete) {
					RxIsComplete = false;
					SEQUENCE = DECODER_FUNCTION;
				}
				break;
				
			case DECODER_FUNCTION:			 
					
					cmd_local_last = cmd_local;	
					cmd_local = discoveryCommand(cmd_updated);					
					if (cmd_local == INVALID){
						USART_Transmit("\r--> Command Invalidated <--");
						USART_Transmit("\rcmd: ");
					} else USART_Transmit("\rcmd: ");									
					SEQUENCE = EXECUTE_FUNCTION_LOOP;
				break;
			
			case EXECUTE_FUNCTION_LOOP:
		
				switch(cmd_local)
				{
					case HELP:
						USARTCharTransmit(NEWLINE);
						menu();
						cmd_local = cmd_local_last;						
					break;
							
					case LED_ON:
						PORTB |= (1<<LED);
						cmd_local = STOP_FUNCTION;
					break;
							
					case LED_OFF:
						PORTB &= ~( 1<<LED);
						cmd_local = STOP_FUNCTION;
					break;
							
					case LED_BLINK:
						PORTB ^= (1<<LED);
						_delay_ms(1000);
					break;
							
					case STOP_FUNCTION:
						// do-nothing
					break;
							
					case INVALID:
						cmd_local = cmd_local_last;	
					break;
				}
				if (RxIsComplete) SEQUENCE = WAIT_RX_COMPLETE;
			}
	}
}
