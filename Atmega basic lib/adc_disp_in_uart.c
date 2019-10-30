
#define F_CPU 6000000UL
#define BAUD_RATE 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/sfr_defs.h>

#include "adc.h"
#include "uart.h"

#define bit_set(sfr,bit)	(sfr |= (1<<bit))
#define bit_clr(sfr,bit)	(sfr &= ~(1<<bit))


char s[10];

int main(void)
{
	uart_init();			// Initialize UART
	ADC_Init();			// initialize ADC
	
		
    while(1)
    {
		
		x=ADC_Read(0);	// READ ADC
		itoa(x,s,10);	// INTEGER TO ASCII CONVERSION
		uart_puts(s);	// 
		uart_putc('\r'); // carriage return
		_delay_ms(500);
		
    }
}