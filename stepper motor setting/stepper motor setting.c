/*
 * stepper_motor_setting.c
 *
 * Created: 2/1/2014 5:42:13 PM
 *  Author: ADMIN
 */ 

#include <avr/io.h>
#include <util/delay.h>



#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3



#define	SM_DIR_REG		 DDRB					// PORT DIRECTION REGISTER FOR STEPPER MOTOR

#define SM_OUT_REG		 PORTB					// OUT PUT PORT FOR STEPPER MOTOR



#define SM_INIT_OUT		 PORTD					// STEPPER MOTOR INITIAL POSITION CHECK SWITCH OUTPUT PORT

#define SM_INIT_DIR		 DDRD				    // STEPPER MOTOR INITIAL POSITION CHECK SWITCH DIRECTION REGISTER

#define SM_INIT_IN		 PIND					// STEPPER MOTOR INITIAL POSITION CHECK SWITCH INPUT PORT







#define SWITCH			 (SM_INIT_IN&(1<<PIN0))
#define MAX 500



char step_clockwise( char );
char step_anti_clockwise ( char );
unsigned int adc_read(char);




int main(void)
{  
	
	char sp = 0B00001100;
	
	unsigned int mic[4],temp;
	
	unsigned char flag  ,i , ;
	
	
	
	SM_DIR_REG |= ( (1<<PIN0) | (1<<PIN1) | (1<<PIN2) | (1<<PIN3) ) ;
	
	SM_INIT_DIR  &= ( ~(1<<PIN0) ) ;
	
	SM_INIT_OUT |=  (1<<PIN0) ;
	
	
	
	while ( !SWITCH )
	{
		sp = step_clockwise ( sp );
		
		_delay_ms(500);
	}
	
	
	while(1)
	
    {
       	for( i=0;i<4;i++)
		   mic[i] = adc_read(i);
		
		if ( (mic[0] > mic[1]) && (mic[0] > mic[2]) && (mic[0] > mic[3]) && (mic[0] > MAX) )   
		
			flag = 'A';
		
		else if ( (mic[1] > mic[0]) && (mic[1] > mic[2]) && (mic[1] > mic[3]) && (mic[1] > MAX) )
		
			flag = 'B';
			
		else if ( (mic[2] > mic[0]) && (mic[2] > mic[1]) && (mic[2] > mic[3]) && (mic[2] > MAX) )
		
			flag = 'C';
			
		else if ( (mic[3] > mic[0]) && (mic[3] > mic[1]) && (mic[3] > mic[2]) && (mic[3] > MAX) )
		
			flag = 'D';	
		
		
		
    }
	
}	





char step_clockwise ( char sp )

{
	if( sp == 0B00000011 )
	
	{
		
		sp = 0B00001001;
		
	}
	
	else if( sp == 0B00001001)
	
	{
		
		sp = 0B00001100;
		
	}
	
	else
	{
		sp >>= 1;
	}
	
	return (sp);
	
}




char step_anti_clockwise ( char sp )

{
	if( sp == 0B00001100 )
	
	{
		
		sp = 0B00001001;
		
	}
	
	else if( sp == 0B00001001)
	
	{
		
		sp = 0B00000011;
		
	}
	
	else
	{
		
		sp <<= 1;
		
	}
	
	return (sp);
	
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
	
	ADCSRA &= (~( 1<<ADEN ));   // ADC off to reduce power consumption
	
	return ( a += (256*b) );
		
}