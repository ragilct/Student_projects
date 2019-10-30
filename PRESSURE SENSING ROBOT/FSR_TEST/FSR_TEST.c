/*
 * FSR_TEST.c
 *
 * Created: 13-Mar-15 11:26:01 AM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


#include "adc.h"
#include "PIN_CONFIG.h"
#include "UART.h"


int main(void)
{
	
	uint16_t adc_val;
	char str[10];
	
	ADC_Init ();
	USART_Init ();
	
    while(1)
    {
		adc_val = ADC_Read (4);
		
		itoa (adc_val,str,10);
		
		send_string (str);
		
		send_data ('\n');
		
		_delay_ms (1000);
		         
    }
	
}