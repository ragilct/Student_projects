/*
 * ANTI_THEFT_TRANSFORMER.c
 *
 * Created: 24-Mar-15 6:24:14 AM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <string.h>
#include <stdio.h>



#include "PIN_CONFIG.h"
#include "UART.h"
#include "adc.h"

#define  CURRENT_SENSOR		ADC_Read (5)
#define  OVER_LOAD	1
#define		NORMAL 0

#define RELAY_ON		output_pin (PortB,PB0,HIGH)
#define RELAY_OFF		output_pin (PortB,PB0,LOW)
	

volatile uint8_t over_load_flag = 0;
volatile uint8_t gui_flag = 0;


volatile uint16_t tr_current = 0;

char str[20];

volatile uint8_t load_val = 0;
volatile uint8_t relay_flag = 1;

int main(void)
{
	
	
	USART_Init ();
	ADC_Init ();
	sei();
	
	/*output_pin (PortD,PD4,HIGH);
	
	_delay_ms (1000);
	
	output_pin (PortD,PD4,LOW);
	
	_delay_ms (1000);
	
	output_pin (PortD,PD4,HIGH);
	
	_delay_ms (1000);
	
	output_pin (PortD,PD4,LOW);
	
	_delay_ms (1000);*/
	
	
	
	
	while(1)
	{
		tr_current = CURRENT_SENSOR;
	
		
		if ( ( tr_current >= 40 ) && ( tr_current <= 70 ) )		// 40w load selection
		{
			load_val = 5;
			
		}
		else if ( ( tr_current >= 71) && ( tr_current <= 130 ) )
		{
			load_val = 40;
		}
		else if ( ( tr_current > 130) && ( tr_current <= 170 ) )
		{
			load_val = 45;
		}
		else if ( ( tr_current >= 171) && ( tr_current <= 245 ) )
		{
			load_val = 100;
		}
		else if( tr_current > 245 )
		{
			load_val = 150;
		}
		else
		{
			load_val = 0;
		}
		
		
		if (relay_flag == 1 )
		{
			RELAY_ON;
		}
		else
		{
			RELAY_OFF;
		}
		
			
		sprintf (str,"#ATT$%5d\n",load_val);

		send_string (str);

		_delay_ms(1000);
		
	}
	
		
 }




ISR(USART_RXC_vect)
{
	
	static char str_buffer[100];
	static uint8_t count;
	
	str_buffer[count] = UDR;
	str_buffer[ count + 1 ] = '\0';
	count ++;
	
		
	
	if (  (str_buffer[count - 1] == '\n')  ||  ( count >= 100 ))
	{
		
		
		if( strstr (str_buffer,"#ATSTON") )
		{
			relay_flag = 1;
			
			strcpy (str_buffer,"\0");
		}
		else if ( strstr ( str_buffer,"#ATSTOFF") )
		{
			relay_flag = 0;
			
			strcpy (str_buffer,"\0");
		}
		else
		{
			count =0;
		}
		
						
		
		count = 0;
		
	}
	
	
	
	
}


