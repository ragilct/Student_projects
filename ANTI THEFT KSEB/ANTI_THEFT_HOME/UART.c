

#include "UART.h"



void USART_Init()
{
	
	
	UBRRL = 38;
				
	UCSRB |= ((1<<RXEN)|(1<<TXEN)|(1<<RXCIE));								   // Enable receiver and transmitter
		
	UCSRC |= ((1 <<URSEL) | (1 << UCSZ0 ) | (1 << UCSZ1 ));					   // Use 8- bit character sizes
	
}





void send_data( unsigned char data )
{
	
	while (!( UCSRA & (1 << UDRE )));												   // Wait for empty transmit buffer
	
	UDR = data;																       // Put data into buffer, sends the data

}

void send_string ( char *str )
{
	while(*str != '\0')
	{
		send_data( *str);
		
		str	++;

	}
	
	//send_data('\0');
}

/*

ISR(USART_RXC_vect)
{
	static char str_buffer[85];
	
	static uint8_t count;
	
	str_buffer[count] = UDR;
	count++;
	
	
	if( ( str_buffer[count - 1] == 0X0D ) || ( count >= 85 ) )
	{
		
		
		count = 0;
	}
	
}
*/
