/*
 * SMART_INVERTER_BARTONHILL.c
 *
 * Created: 27-Mar-15 5:54:47 PM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>


#include "lcd.h"
#include "adc.h"
#include "PIN_CONFIG.h"


#define  BAT_LEVEL		ADC_Read (4)
#define  BAT_MAX_THRESHOLD	800
#define  BAT_MIN_THRESHOLD	500

#define RELAY_ON	output_pin (PortD,PD7,HIGH)
#define RELAY_OFF	output_pin (PortD,PD7,LOW)


void Timer_1_capt_init( void );
uint64_t freq_change_adc( void );


volatile uint64_t freq = 0;
volatile uint16_t capt_period = 0;
volatile uint16_t capt_freq = 0;
volatile uint16_t bat_val = 0;


int main(void)
{
	sei();
	ADC_Init ();
	LCDInit (LS_NONE);
	
	LCDWriteString ("INITIALIZING");
	
	_delay_ms (2000);
	
	LCDClear();
	
	
	Timer_1_capt_init ();
	
	
    while(1)
    {
        
		freq = freq_change_adc ();
		
		
		LCDWriteStringXY(0,0,"FREQ");
		LCDWriteIntXY(0,1,freq,2);
		
		
		bat_val = BAT_LEVEL;
		
		LCDWriteStringXY(6,0,"BAT");
		LCDWriteIntXY(6,1,bat_val,4);
		
		
		if ( ( ( bat_val < BAT_MAX_THRESHOLD ) && ( freq >= 50 ) ) || ( bat_val < BAT_MIN_THRESHOLD ) )
		{
			
			RELAY_ON;
			
		}
		else
		{
			RELAY_OFF;
		}
		
		
		
		
		_delay_ms (500);
		
	
    }


}


void Timer_1_capt_init( void )
{
	TCCR1B |= ( 1<< ICES1 ); // rising edge
			
	TIMSK |= (1<<TICIE1);		// timer 1 capture interrupt enable
	
	TCNT1	= 0x00;
	
	TCCR1B |=  ( ( 1<< CS11) | ( 1<< CS10 ) );	// clock select .. 64 prescalar
}


ISR(TIMER1_CAPT_vect)
{
	
	static uint16_t capt_val;
	static uint16_t pre_capt_val;
	
	
	pre_capt_val = capt_val;
	capt_val = ICR1;
	
	if (pre_capt_val < capt_val )
	{
		capt_period = capt_val - pre_capt_val;
	}
	else
	{
		TCNT1 = 0x00;
	}
	
		
}



uint64_t freq_change_adc( void )
{
	
	int16_t adc_val;
	
		adc_val = ADC_Read (5);
	
	
	capt_freq = ( ( F_CPU/64/capt_period) / 2 );
	
	if (  adc_val > 800 )
	{
		
		capt_freq += 2;	
		
	}
	else if ( adc_val > 600 )
	{
		
		capt_freq += 1;
	}
	else if ( adc_val > 400 )
	{
		
		capt_freq -= 0;
	}
	else  if ( adc_val > 200 )
	
	{
		capt_freq -= 1;
	}
	else
	{
		capt_freq -= 2;
	}
	
	
	return(capt_freq);
	
}