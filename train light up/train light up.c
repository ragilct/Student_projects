/*
 * train_light_up.c
 *
 * Created: 3/9/2014 2:22:49 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000
#include <avr/io.h>
#include <util/delay.h>

#define MAX_P 1000
#define MAX_M 90
#define MAX_W 10

#define GP (!(PIND&(1<<PD1)))

#define RL_EP (PORTD|=(1<<PD0))
#define RL_GP (PORTD&=(~(1<<PD0)))


#define LED1_ON  (PORTD|=(1<<PD2))
#define LED1_OFF (PORTD&=(~(1<<PD2)))

#define LED2_ON  (PORTD|=(1<<PD3))
#define LED2_OFF (PORTD&=(~(1<<PD3)))

#define LED3_ON  (PORTD|=(1<<PD4))
#define LED3_OFF (PORTD&=(~(1<<PD4)))



unsigned int adc_read(char );
void system_init(void);

unsigned int piezo,mech,wind;

int main(void)
{
	system_init();
    while(1)
	
    {
		
		PORTD &= (~( (1<<PD2) | (1<<PD3) | (1<<PD4) ));
		
			wind  = adc_read(3);
			piezo = adc_read(0);
			mech  = adc_read(1);
			
			
			
			if( (piezo > MAX_P) )
			{
				LED1_ON;
			}
			else
			{
				LED1_OFF;
				
			}
			
			
			if( (mech > MAX_M) )
			{
				LED2_ON;
			}
			else
			{
				LED2_OFF;
			}
			
			
			if( ( wind > MAX_W) )
			{
				LED3_ON;
			}
			else
			{
				LED3_OFF;
			}
		
			
			if (!GP)
			{
				
				RL_EP;
			}
			else
			{
				RL_GP;
			}
			
			//_delay_ms(600);
			
	}

}

void system_init(void)
{
	DDRD |= ((1<<DDD0) | (1<<DDD2) | (1<<DDD3) | (1<<4) | (1<<DDD5) );
	
	DDRD &= (~(1<<DDD1));
	PORTD |= (1<<DDD1);
	
	
	
}





unsigned int adc_read(char nadc)
{
	unsigned int a,b;
	
	ADMUX = 0x00;     // initializing ADMUX and ADCSRA
	
	ADCSRA = 0x00;
	
	ADMUX = nadc;         // Selection of ADC channel 
	
	ADCSRA |= ( 1<<ADEN ); // Enabling ADC
	
	ADCSRA |= (1<<ADSC);  //Start conversion
	
	while (  !(ADCSRA & (1<<ADIF))  ); // wait for conversion complete ADIF= 0--->1
	
	a = ADCL;  
	
	b = ADCH ;
	
	ADCSRA |= (1<<ADSC);  //Start conversion
	
	while (  !(ADCSRA & (1<<ADIF))  ); // wait for conversion complete ADIF= 0--->1
	
	a = ADCL;  
	
	b = ADCH ;
	
	ADCSRA &= (~( 1<<ADEN ));   // ADC off to reduce power consumption
	
	return ( a += (256*b) );
		
}