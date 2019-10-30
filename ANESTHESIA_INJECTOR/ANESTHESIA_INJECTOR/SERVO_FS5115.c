/*
 * SERVO_FS5115.c
 *
 * Created: 13-Mar-15 3:14:04 PM
 *  Author: ADMIN
 */ 



#include "SERVO_FS5115.h"


void servo_init_function (void)
{
	
	
	TCCR1A|= (1<<WGM11);		 						//NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11);	//PRESCALER=8 MODE 14(FAST PWM)
	
	ICR1= ( ( F_CPU / (8*50) ) + 1 );	//fPWM=50Hz (Period = 20ms Standard).

	#ifdef  __AVR_ATmega8__
	
	DDRB|= ((1<<PB1)|(1<<PB2));	//PWM Pins as Out
	
	#endif
	
	#ifdef  __AVR_ATmega16__
	DDRD |= ( 1<<PD4 ) | ( 1<<PD5 );
	
	#endif
}



void servo_angle_set_x_axis(float ang )
{
	
	
	TCNT1 = 0;
	TCCR1A	&= ( ~(1<<COM1B1));
	TCCR1A |= (1<<COM1A1);
	
	OCR1A =  ( ceil( (float) (8.33 * ( ang  ) )  ) ) + 376 ;
	
	
	_delay_ms (600);
	
	
}


void servo_angle_set_y_axis(float ang )
{
	
	TCNT1 = 0;
	TCCR1A	&= ( ~(1<<COM1A1));
	TCCR1A |= (1<<COM1B1);
	
	OCR1B =  ( ceil( (float) (8.33 * ( ang) )  ) ) + 376 ;
	
	_delay_ms (600);
	
}




