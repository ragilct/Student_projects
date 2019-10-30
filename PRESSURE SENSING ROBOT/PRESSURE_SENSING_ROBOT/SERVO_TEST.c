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



