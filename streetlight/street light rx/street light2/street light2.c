/*
 * street_light2.c
 *
 * Created: 3/15/2014 10:15:43 AM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000UL


#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "lcd.c"

void system_init();
void USART_Init();
void send_data( unsigned char  );
void send_string ( char * );

char lf1='0',lf2='0',lf3='0',rf1='0',rf2='0',rf3='0';

char l_cf='0',nl_cf='0' ;

unsigned int rx_count=0;


int main(void)
{
	int str_i;
	system_init();
	LCDInit(LS_NONE);
	LCDClear();
	sei();
			
    while(1)
    {
		//cli();
        if( l_cf == '0')
		{
			LCDClear();
			LCDWriteString("LIGHT");
			l_cf = '1';
		}
		
		_delay_ms(200);
		
		
		if (lf1 == '1')
		{
			
			LCDWriteStringXY(1,1,"L1");
		}
		
		else
		{
			
			LCDWriteStringXY(1,1,"   ");
		}
		
		_delay_ms(200);	
		
		if (lf2 == '1')
		{
			
			LCDWriteStringXY(5,1,"L2");
		}
		
		else
		{
			
			LCDWriteStringXY(5,1,"  ");	
		}
		
		_delay_ms(200);
		
		if (lf3 == '1')
		{
			
			LCDWriteStringXY(10,1,"L3");
		}
		
		else
		{
			
			LCDWriteStringXY(10,1,"   ");	
		}
		
		nl_cf='0';
	//sei();
	//_delay_ms(2000);
	_delay_ms(200);
							
    }
}

void system_init()
{
	
	
	USART_Init();
	 int0_init ();
	_delay_ms(200);
	
	_delay_ms(200);
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
	
	//send_data('\0');
}


ISR(USART_RXC_vect)

{
	char data;
	data = UDR;
	

	
							switch (data)
							{
								case '1':
										{
											lf1 = '1';
											break;
										}
				
								case '2':
										{
											lf1 = '0';
											break;
										}
				
								case '3':
										{
											lf2 = '1';
											break;
										}
				
								case '4':
										{
											lf2 = '0';
											break;
										}
				
								case '5':
										{
											lf3 = '1';
											break;
										}
				
								case '6':
										{
											lf3 = '0';
											break;
										}
				
								case '7':
										{
											rf1 = '1';
											break;
										}
				
								case '8':
										{
											rf1 = '0';
											break;
										}
				
								case '9':
										{
											rf2 = '1';
											break;
										}
				
								case 'a':
										{
											rf2 = '0';
											break;
										}
				
								case 'b':
										{
											rf3 = '1';
											break;
										}
				
								case 'c':
										{
											rf3 = '0';
											break;
										}
				
		
							}
							
							
						
							
							
	}	
	
	
	
	void int0_init ()
{
	//MCUCR  |=  ( 1<<ISC00 );
	
	GICR   |=  ( 1<<INT0	);

	DDRD   &=  (~( 1<<DDD2 ));
	
	PORTD  |=  ( 1 <<PD2 );
}

ISR(INT0_vect)
{
	 if( nl_cf == '0')
		{
			LCDClear();
			LCDWriteString("RELAY");
			nl_cf = '1';
		}
		
		_delay_ms(100);
		
		
		if (rf1 == '1')
		{
			
			LCDWriteStringXY(1,1,"RL1");
		}
		
		else
		{
			
			LCDWriteStringXY(1,1,"    ");
		}
		
		_delay_ms(100);	
		
		if (rf2 == '1')
		{
			
			LCDWriteStringXY(5,1,"RL2");
		}
		
		else
		{
			
			LCDWriteStringXY(5,1,"   ");	
		}
		
		_delay_ms(100);
		
		if (rf3 == '1')
		{
			
			LCDWriteStringXY(10,1,"RL3");
		}
		
		else
		{
			
			LCDWriteStringXY(10,1,"    ");	
		}
		
	l_cf ='0';
	_delay_ms(100);
}

	
		
	
	

void USART_Flush( void )
{
unsigned char dummy;

while ( UCSRA & (1<<RXC) ) 
{
	dummy = UDR;
}

}
