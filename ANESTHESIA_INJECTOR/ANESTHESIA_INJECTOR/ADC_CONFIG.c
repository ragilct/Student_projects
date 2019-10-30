/*
 * ADC_CONFIG.c
 *
 * Created: 22-Feb-15 8:03:57 AM
 *  Author: ADMIN
 */ 


#include "ADC_CONFIG.h"


void ADC_Init( uint8_t adc_ref )// AREF,AVCC or IV ( internal 2.56 voltage reference)
{
	
	ADMUX &= ( ~ (1<<ADLAR) ); // right justification of result
	
	ADMUX &= ( ~ ( (1<<REFS1) | (1<<REFS0) ) ); // clearing reference selection bits
	
	ADMUX |= ( adc_ref << 6 ); // sets voltage reference
	
	ADCSRA &= ( ~ (1<<ADATE) );// Adc auto trigger disabled
	
	ADCSRA &= ( ~ ( (1<<ADIF) | (1<<ADIE) ) ); // Adc interrupt flag cleared and adc interrupt disabled
	
	ADCSRA &= ( ~ ( (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) ) ); // clearing all prescalar selection bits
	
	ADCSRA |= ( (1<<ADPS2) | (1<<ADPS0) ); // sets prescalar to div by 32 get 187.5khz frequency which is below 200khz for 6 mhz crystal
	
	
}




uint16_t ADC_Read( uint8_t channel) // channel 0 to 7
{
		
	ADMUX &= ( ~ ( ( 1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0) ) ); // clear all channel selection multiplexer bits
	
	ADMUX |= channel; // set channel
	
	ADCSRA |= ( (1<<ADEN) | (1<<ADSC) ); // adc enable and start conversion
	
	while( !( ADCSRA & (1<<ADIF) ) ); // wait for adc conversion complete ie ADIF = 1
	
	ADCSRA |= ( 1<< ADIF ); // clear ADIF flag by writing 1 to it.
			
	return(ADC);
	

}