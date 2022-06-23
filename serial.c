#include "serial.h"

enum RX_sequence {DATA_ARRIVED, NEW_DATA};
enum RX_sequence RX_STATE = NEW_DATA;

volatile char ReceivedByte;
volatile char RxIsComplete = false;
volatile unsigned char cmd_idx = 0;


void USARTCharTransmit(unsigned char tx)
{
	while ( !(UCSR0A & (1<<UDRE0)));
	UDR0 = tx;
}

void USART_Transmit( char *data )
{
	while ( *data != '\0' ){
		
		/* Wait for empty transmit buffer */
		while ( !(UCSR0A & (1<<UDRE0)));

		/* Put data into buffer, sends the data */
		UDR0 = *data;
		data++;
	}
}

ISR(USART_RX_vect)
{
	ReceivedByte = UDR0;
	USARTCharTransmit(ReceivedByte);
	
	switch ( RX_STATE ) {
		case NEW_DATA:
		cmd_idx = 0;
		if ( ReceivedByte == STARTMARKER ) RX_STATE = DATA_ARRIVED;
		break;
		
		case DATA_ARRIVED:
		cmd[cmd_idx] = ReceivedByte;
		if ( ReceivedByte == ENDMARKER ) {
			RX_STATE = NEW_DATA;
			cmd[cmd_idx] = '\0';
			cmd_idx = 0;
			RxIsComplete = true;
			strcpy(cmd_updated,cmd);
		}
		else {
			if ( ReceivedByte == BACKSPACE ) {
				if ( --cmd_idx < 0 ) RX_STATE = NEW_DATA;
			}
			else {
				cmd_idx++;
				if( cmd_idx >= CMD_MAX_LENGHT ) RX_STATE = NEW_DATA;
			}
		}
		break;
	}
}