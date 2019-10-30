/*
 * ANTI_THEFT_HOME.c
 *
 * Created: 25-Mar-15 3:24:27 PM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>



#include "PIN_CONFIG.h"
#include "lcd.h"
#include "UART.h"
#include "adc.h"



#define RLY_ON		output_pin (PortB,PB0,HIGH)
#define RLY_OFF		output_pin (PortB,PB0,LOW)





volatile uint16_t adc_val = 0;
volatile uint8_t  load_val = 0;

volatile uint16_t load_5_time = 0;
volatile uint16_t load_40_time = 0;
volatile uint16_t load_45_time = 0;


volatile uint32_t time = 0;
volatile uint32_t tot_load = 0;


volatile uint8_t relay_flag  = 1;


char str[50];







int main(void)
{
	
	ADC_Init ();
	LCDInit (LS_NONE);
	USART_Init ();
	
	sei();
	
	LCDClear();
	
	LCDWriteString (" INITIALISING");
	
	_delay_ms (3000);
	
	LCDClear();
	LCDWriteString ("SYSTEM ACTIVE ");
	
	_delay_ms (1000);
	
	LCDClear();
	
	
    while(1)
		{
		
		time ++;
		
		adc_val = ADC_Read (5);
		
				
		
		if ( ( adc_val >= 50 ) && ( adc_val <= 70 ) )		// 40w load selection
		{
			load_val = 5;
			
		}
		else if ( ( adc_val >= 71) && ( adc_val <= 130 ) )
		{
			load_val = 40;
		}
		else if ( ( adc_val > 130) && ( adc_val <= 170 ) )
		{
			load_val = 45;
		}
		else if ( ( adc_val >= 171) && ( adc_val >= 245 ) )
		{
			load_val = 100;
		}
		else
		{
			load_val = 0;
		}
		
		
		
		if (load_val == 5 )
		{
			load_5_time ++;
		}
		
		
		if ( load_val == 40 )
		{
			load_40_time ++;
		}
		
		
		if ( load_val == 45 )
		{
			load_45_time ++;
		}
		
		
		if ( load_val == 0 )
		{
			load_5_time = 0;
			
			load_40_time = 0;
			
			load_45_time = 0;
		}
		
		
		
		if ( time == 10 )
		{
			
			tot_load = tot_load + (  ( load_5_time * 5 ) + ( load_40_time * 40 ) + ( load_45_time * 45 ) );
			
			load_5_time = 0;
			
			load_40_time = 0;
			
			load_45_time = 0;
			
			time = 0;
			
			sprintf(str,"#ATS$%5ld\n",tot_load);
			
			send_string (str);
			
		}
		
		
		LCDWriteStringXY(0,0,"UNIT");
		LCDWriteIntXY( 0,1,tot_load,4);
		
		
		if( relay_flag == 1 )
		{
			RLY_ON;
		}
		else
		{
			RLY_OFF;
		}
	
		
					
		_delay_ms ( 1000 );
		
		
		
	        
    }
}





ISR(USART_RXC_vect)
{
	
	static char str_buffer[100];
	
	static uint8_t count;
	
	
	str_buffer[count] = UDR;
	
	str_buffer[ count + 1 ] = '\0';
	
	count++;
	
		
	
	if ( ( str_buffer[count - 1 ] == '\n' ) || ( count >= 100 )  )
	{
		
		count = 0;
		
			
		if( strstr (str_buffer,"#ATSHON") )
		{
			relay_flag = 1;
			
			strcpy (str_buffer,"\0");
		}
		else if ( strstr ( str_buffer,"#ATSHOFF") )
		{
			relay_flag = 0;
			
			strcpy (str_buffer,"\0");
		}
		else
		{
			count =0;
		}
		
		
	}
	
	
}