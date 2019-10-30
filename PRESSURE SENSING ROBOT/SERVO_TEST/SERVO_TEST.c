/*
 * SERVO_TEST.c
 *
 * Created: 13-Mar-15 12:15:05 AM
 *  Author: ADMIN
 */ 


#include <avr/io.h>

#include <util/delay.h>



void servo_angle_set_x_axis( float);
void servo_angle_set_y_axis(float );
void servo_init_function (void);


int main(void)
{
	
	servo_init_function ();
	
	
	servo_angle_set_x_axis (150);
	
	_delay_ms (5000);
	
	
    while(1)
    {
		
		
		for (uint16_t i = 260;i>150;i--)
		{
			servo_angle_set_x_axis (i);
			
			_delay_ms (1000);
		}
		
		_delay_ms(5000);
		
		
		/*
       servo_angle_set_x_axis (180);
	   
	   _delay_ms (1000);
	   
	   servo_angle_set_x_axis (100);
	   
	   _delay_ms (1000);
	   
	   
	   servo_angle_set_x_axis (120);
	   
	   _delay_ms (1000);
	   
	   
	   servo_angle_set_x_axis (110);
	   
	   _delay_ms (1000);
	   
	  
	   
	   servo_angle_set_x_axis (80);
	   
	   _delay_ms (1000);
	  */ 
	   
	//   _delay_ms(2000);
	   
	   
    }
}



void servo_init_function (void)
{
	
	
	TCCR1A|= (1<<WGM11);		 						//NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11);	//PRESCALER=8 MODE 14(FAST PWM)
	
	ICR1= ( ( F_CPU / (8*50) ) + 1 );	//fPWM=50Hz (Period = 20ms Standard).

	DDRB|= ((1<<PB1)|(1<<PB2));	//PWM Pins as Out
	
	
}



void servo_angle_set_x_axis(float ang )
{
	
	
	TCNT1 = 0;
	TCCR1A	&= ( ~(1<<COM1B1));
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0);
	
	OCR1A =  ( ceil( (float) (8.33 * ang)  ) ) + 376 ;
	
	
}


void servo_angle_set_y_axis(float ang )
{
	
	TCNT1 = 0;
	TCCR1A	&= ( ~(1<<COM1A1));
	TCCR1A |= (1<<COM1B1)|(1<<COM1B0);
	
	OCR1B =  ( ceil( (float) (8.33 * ang)  ) ) + 376 ;
	
}
