/*
 * checkantitheft.c
 *
 * Created: 2/24/2014 2:24:47 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000
#include <avr/io.h>
#include <string.h>




void send_data( unsigned char  );

void send_string ( char * );

char temp1[] = "check one",temp2[];




int main(void)
{
	char *p;
	
	USART_Init();
	
    
	
        strcpy(temp2,temp1);
		
		send_string(temp1);
		send_string(temp2);
		
		p= strchr(	temp1,'k');
		
		
   
    while(1); 
}

void USART_Init()
{
			
		UCSRB |= ((1<<RXEN)|(1<<TXEN)|(1<<RXCIE));								   // Enable receiver and transmitter 
		
        UBRRL = 38;
		
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
	
	send_data('\0');
}	