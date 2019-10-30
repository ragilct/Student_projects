

#define F_CPU 6000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/sfr_defs.h>


#include "adc.h"
#include "uart.h"

#define bit_set(sfr, bit) (sfr |= (1 << bit))
#define bit_clr(sfr, bit) (sfr &= ~(1 << bit))

static char		longitude[12],lattitude[12],altitude[12];




//**************** MAIN FUNCTION ******************************

int main(void)
{

	
	uart_init();
	
	sei();			// GLOBAL INTERRUPT ENABLE
	
	
	while(1)
	{
		uart_puts(longitude);
		uart_putc("\r");
		uart_puts(lattitude);
		uart_putc("\r");
		uart_puts(altitude);
		uart_putc("\r");
		uart_putc("\r");
		_delay_ms(1000);		
	}
}


//**************** UART RX INTERRUPT ******************************

ISR (USART_RXC_vect)
{
	static int i;
	static char uart_buffer[85],temp_buffer[85];
	
	char *add_longitude;
	char *add_lattitude;
	char *add_altitude;
	char *add_gps_status;
	char *add_comma;
	
	uart_buffer[i]=UDR;
	uart_buffer[i+1]='\0';
	i++;
	
	
	if (uart_buffer[i-1]=='\n')	// Receive up to '\n'
	{
		
		i=0;
		strcpy(temp_buffer,uart_buffer);

	 if ( strncmp(uart_buffer,"$GPGGA",6) ==0 )			// If "$GPGGA" is received
		{
			if( strlen(temp_buffer) >60 )
			{
				
				// ************* LATTITUDE ***************
				
				add_comma		=	strchr(temp_buffer,',');		// address of first comma into add_comma
				add_lattitude	=	strchr(add_comma+1,',');		// starting address of lattitude value
				add_comma		=	strchr(add_lattitude+1,'N')-1;	// end of lattitude value
				*add_comma		=	'\0';							// make 3rd comma as '\0' to make the content pointed by add_lattitude as a string
				
				strcpy(lattitude,add_lattitude+1);					// copy lattitude value
				
				// ************** LONGITUDE *****************
				
				strcpy(temp_buffer,uart_buffer);					// copy original string back to temp_buffer
				
				add_longitude	=	strchr(temp_buffer,'N')+2;		// starting address of longitude value
				add_comma		=	strchr(add_longitude,'E')-1;	// end of longitude value
				*add_comma		=	'\0';							// make longitude value as string
				
				strcpy(longitude,add_longitude);					// copy longitude value
				
				// ******************* GPS STATUS ***********************
				
				strcpy(temp_buffer,uart_buffer);
				
				add_gps_status	=	strchr(temp_buffer,'E')+2;		// starting address of gps status
				add_comma		=	strchr(add_gps_status,',');		// end of gps status
				*add_comma		=	'\0';
				
				flags.gps_status=	atoi(add_gps_status);			// ascii to integer
				
				// ******************** ALTITUDE **************************
				
				strcpy(temp_buffer,uart_buffer);
				
				add_comma		=	strchr(temp_buffer,'E')+7;		// address of comma before altitude value comma
				add_altitude	=	strchr(add_comma,',')+1;		// starting address of altitude value
				add_comma		=	strchr(add_altitude,'M')-1;		// end of altitude value
				*add_comma		=	'\0';							// make altitude as string
				
				strcpy(altitude,add_altitude);
				
				select_zigbee();
				strcpy(uart_buffer,"\0");
				strcpy(temp_buffer,"\0");
			}	
			
			else
			{
				strcpy(lattitude,"0");
				strcpy(longitude,"0");
				strcpy(altitude,"0");
				flags.gps_status  = 0;
			}
		
		}
		
		else
		{
			strcpy(uart_buffer,"\0");
			strcpy(temp_buffer,"\0");
		}
	}


}


