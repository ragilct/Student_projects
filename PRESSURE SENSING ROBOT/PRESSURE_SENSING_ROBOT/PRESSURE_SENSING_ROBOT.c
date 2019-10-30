/*
 * PRESSURE_SENSING_ROBOT.c
 *
 * Created: 13-Mar-15 3:05:37 PM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>


#include "SERVO_FS5115.h"
#include "PIN_CONFIG.h"
#include "UART.h"
#include "adc.h"


#define FSR_ADC_CHANNEL	 4		// PIN 27 



#define MOTOR_2_EN	output_pin (PortB,PB0,HIGH)		// PIN 14

#define MOTOR_2_IN_1		PinD7					// PIN 13

#define MOTOR_2_IN_2		PinD6					// PIN 12


#define MOTOR_1_EN	output_pin (PortC,PB3,HIGH)		// PIN 17

#define MOTOR_1_IN_1		PinB4					// PIN 18

#define MOTOR_1_IN_2		PinB5					// PIN 19





/// MOTOR CONTROL ////////////////////////////////////////////////////////

#define MOTOR_CTRL(x)		output_pin_group(4,x,MOTOR_1_IN_1,MOTOR_1_IN_2,MOTOR_2_IN_1,MOTOR_2_IN_2)


#define ROBOT_STOP			MOTOR_CTRL(0B0000)

#define ROBOT_FORWARD		MOTOR_CTRL(0B1010)

#define ROBOT_RIGHT		    MOTOR_CTRL(0B1001)

#define ROBOT_LEFT			MOTOR_CTRL(0B0110)

#define ROBOT_BACKWARD		MOTOR_CTRL(0B0101)

//////////////////////////////////////////////////////////////////////










#define PRESSURE_VAL	ADC_Read (FSR_ADC_CHANNEL)


uint8_t catch_object( uint8_t angle_val_max, uint8_t angle_val_min, uint8_t angle_inc, uint16_t pressure_val_allowed  );
uint8_t pick_object( uint8_t y_up, uint8_t y_down, uint8_t angle_inc );
uint8_t move_robo_hand_down( uint8_t angle_down,uint8_t current_angle, uint8_t angle_inc);
uint8_t move_robo_hand_up( uint8_t angle_up,uint8_t current_angle, uint8_t angle_inc);

volatile uint16_t		max_allowed_pressure=0;
volatile uint8_t		grip_flag = 0;
volatile uint8_t		reset_flag = 0;
volatile uint8_t		regrip_flag = 0;

volatile uint8_t		current_angle_y = 150;
volatile uint8_t		current_x_angle = 180;

volatile uint16_t		gripping_pressure = 350;




int main(void)
{
	
	
	//uint8_t current_x_angle;
	//char str[20];
	
	servo_init_function ();
	ADC_Init ();
	USART_Init ();
	sei();
	
	
		/*itoa (pressure_val,str,10);
		
		send_string (str);
		
		send_data ('\n');*/
		
	//current_angle_y = move_robo_hand_up (140,current_angle_y,3);
	
	
	

	
	servo_angle_set_x_axis (160);
	
	_delay_ms (1000);
	
	servo_angle_set_y_axis (140);
	
	_delay_ms (1000);
	
	
	
		
    while(1)
    {
		
	
	/*	
      current_x_angle = catch_object (160,105,3,600);
	   
	  	   
	   if (current_x_angle != 0 )
	   {
		   output_pin (PortC,PC5,LOW);
		   	
				   	      
		   servo_angle_set_x_axis (160);
		   
		   		   
		   _delay_ms (5000);
	   }
	   
	
	 */
	

	
	
	   
	  
    }


}








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t catch_object( uint8_t angle_open, uint8_t angle_closed, uint8_t angle_inc, uint16_t pressure_val_allowed  )
{
	
	/*
		This function grips up to allowed pressure value and then returns 
		the current angle position of x axis servomotor
		
		180 degree is the maximum open position and 0 degree is the minimum closed position
		
		
		initial opening angle can be set by angle_val_max 
		
		minimum closing angle can be set by angle_val_min 
		
				
		closing angle angle_val_min should be < opening angle angle_val_max 
		
	*/
			
		uint8_t		i;
		uint8_t		pressure_exceed_flag=0;
		uint16_t	pressure_val=0,pressure_val_tot = 0;
		
		output_pin (PortC,PC5,HIGH);
		
		char t_str[20];
		
		
				
		
		for (  i=angle_open ; i >= angle_closed ; ( i = i - angle_inc ) )
		{
			servo_angle_set_x_axis (i);
		
		//	pressure_val = PRESSURE_VAL;
			
			for (uint8_t pj=0;pj<5;pj++)
			{
				
				if ( i <= 150)
				{
					pressure_val_tot += ( PRESSURE_VAL ) ;
				}
				else
				{
					pressure_val_tot = 0;
				}
				
			}
			
			pressure_val = pressure_val_tot/5;
			
			pressure_val_tot = 0;
			
			itoa (pressure_val,t_str,10);
			
			send_string (t_str);
			send_data ('\t');
			
			
			
			if( pressure_val >= pressure_val_allowed )		
			/* returns current angle value if applied 
				pressure is greater than allowed pressure	*/	
			{
				pressure_exceed_flag = 1;
				break;
			}
			
			
			
		//	char t_str[20];
			
			itoa (pressure_val,t_str,10);
			
			send_string (t_str);
			send_data ('\t');
			
			itoa (i,t_str,10);
			
			send_string (t_str);
			send_data ('\n');
			
			
			
			
		}
	
	
	
	
		if (pressure_exceed_flag == 1 )
		{	
			//output_pin (PortC,PC5,HIGH);
			_delay_ms (500);
			SERVO_X_OFF;
			return(i);	// returns current angle
		}
		
		
		output_pin (PortC,PC5,LOW);
		
		
		return(0);    //  returns minimum angle 0
	
			
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








uint8_t move_robo_hand_down( uint8_t angle_down,uint8_t current_angle, uint8_t angle_inc)
{
	
		uint8_t i=0;
	
		for ( i = current_angle; i > angle_down ; i = i - angle_inc  )
		{
			servo_angle_set_y_axis (i);
			
			if ( i <= 55)
			{
				break;
			}
		}
		
		
		return(i);
	
}


uint8_t move_robo_hand_up( uint8_t angle_up,uint8_t current_angle, uint8_t angle_inc)
{
	uint8_t i=0;
	
	for ( i = current_angle; i < angle_up ; i = i + angle_inc  )
	{
		servo_angle_set_y_axis (i);
		
		if ( i >= 150 )
		{
			break;
		}
	}
	
	return(i);
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(USART_RXC_vect)
{
	static char str_buffer[85];
	
	char	temp_buffer[85];
	
	static uint8_t count;
	
	str_buffer[count] = UDR;
	count++;
	
	
	if( ( str_buffer[count - 1] == '\n' ) || ( count >= 85 ) )
	{
		
		count = 0;
		
		strcpy( temp_buffer,str_buffer);
				
		if ( strncmp (temp_buffer,"PS_ROBOT_MOVE",13) == 0 )		// checks first eight received characters
		{
			
				if( strstr(temp_buffer,"FORWARD") )
				{
					// robot move forward
					
					ROBOT_FORWARD;
					
					
				}
				else if( strstr (temp_buffer,"BACKWARD"))
				{
					
					// robot move back ward
					
					ROBOT_BACKWARD;
					
					
				}
				else if( strstr (temp_buffer,"RIGHT"))
				{
					
					// robot move back right
					
					ROBOT_RIGHT;
					
					
				}
				else if( strstr (temp_buffer,"LEFT"))
				{
					
					// robot move back left
					
					ROBOT_LEFT;
					
					
				}
				else if( strstr( temp_buffer,"STOP"))
				{
					
					// robot stop
					
					ROBOT_STOP;
									
				}
				else
				{
					// robot stop
					
					ROBOT_STOP;
					
				}
				
			
			
			
		}
		else if ( strncmp ( temp_buffer,"PS_ROBOT_CTRL",13) == 0 )
		{
			
			char *pv_str_add;
			
			char *pv_add;
			
									
			
			// maximum pressure value extraction
			
			strcpy (temp_buffer,str_buffer);
			
			pv_str_add = strstr( temp_buffer,"PRESSURE=" );
						
			if ( pv_str_add )				// extracts pressure value
			{
				
				output_pin (PortC,PC5,HIGH);
				
				_delay_ms (1000);
				
				output_pin (PortC,PC5,LOW);
				
				_delay_ms (1000);
					
								
				pv_add = strchr(pv_str_add,'$');
				
				*pv_add = '\0';		// terminates string at $
				
				pv_add = strchr( pv_str_add,'=');
											
				gripping_pressure =(uint16_t) atoi( (pv_add + 1));
				
							
				
			}
			
			
			
			
			// grip command extraction
					
			strcpy (temp_buffer,str_buffer);
			
			if( strstr( temp_buffer,"GRIP" ) ) 
			{
							
				output_pin (PortC,PC5,HIGH);
				
				_delay_ms (1000);
				
				output_pin (PortC,PC5,LOW);
				
				_delay_ms (1000);
							
				
				current_angle_y = move_robo_hand_down (50,current_angle_y,3);
				
				
				
				while( 1 )
				{
					
					
					gripping_pressure = 440;
					
					current_x_angle = catch_object (160,105,3,gripping_pressure);
					
										
					if ( ( current_x_angle != 0 )  )	// pressure exceeds the limit
					{
						
						current_angle_y = move_robo_hand_up (90,current_angle_y,3);
						
						_delay_ms (1000);
						
						//current_angle_y = 90;
												
						break;
					}
					else
					{
						if( current_x_angle == 0 )		
						{
							
							servo_angle_set_x_axis (160);
							
							_delay_ms (1000);
														
							break;
						}
					}
					
							
				}
				
				
				strcpy (str_buffer,"\0");
				
			
			}
			
			// gripped and picked
			
				
			
			
			strcpy (temp_buffer,str_buffer);
			
			if( strstr( temp_buffer,"INTIALIZE" ) )	
			{
				
				
				output_pin (PortC,PC5,HIGH);
				
				_delay_ms (1000);
				
				output_pin (PortC,PC5,LOW);
				
				_delay_ms (1000);
				
				
				gripping_pressure = 480;
				
				
				current_angle_y = move_robo_hand_down (53,current_angle_y,3);
				
				
				servo_angle_set_x_axis (160);
				_delay_ms (1000);
				
				
				current_angle_y = move_robo_hand_up (90,current_angle_y,3);
				
				_delay_ms (1000);
				
				
				
			
				
				strcpy( str_buffer,"\0");
				
				
				
			}
			
			
			
			
			
			
			// re grip command extraction
			
			strcpy( temp_buffer,str_buffer);
			
			if( strstr (temp_buffer,"REGR_IP") )
			{
				
				output_pin (PortC,PC5,HIGH);
				
				_delay_ms (1000);
				
				output_pin (PortC,PC5,LOW);
				
				_delay_ms (1000);
				
				servo_angle_set_x_axis (160);
				
				_delay_ms (1000);
								
							
				current_angle_y = move_robo_hand_down (53,current_angle_y,3);
				
				gripping_pressure = 450;
					
				
				while( 1 )
				{						
									
					current_x_angle = catch_object (160,105,3,gripping_pressure);
					
					
					if ( ( current_x_angle != 0 )  )	// pressure exceeds the limit
					{
						
						current_angle_y = move_robo_hand_up (90,current_angle_y,3);
						
						_delay_ms (1000);
													
						break;
					}
					else
					{
						if( current_x_angle == 0 )
						{
							servo_angle_set_x_axis (160);
							_delay_ms (1000);
							break;
						}
					}
					
					
				}
				
				
				gripping_pressure = 440;
				
				
			}
			
			
			
		// re grip completed
			
					
			
			
		}
		else if ( strncmp (temp_buffer,"PS_ROBOT_YPOS",13) == 0)
		{
			
						
			char *ct_str_add;
			
			char *ct_add;
			
			uint8_t dest_ang=150;
						
			ct_str_add = strchr (temp_buffer,'$');
			
			*ct_str_add = '\0';
			
			ct_add = ( strchr( temp_buffer,'=') + 1 );
			
			dest_ang = ( uint8_t ) atoi (ct_add);
			
			
			if ( current_angle_y < dest_ang )
			{
				current_angle_y = move_robo_hand_up (dest_ang,current_angle_y,5);
			}
			else
			{
				current_angle_y = move_robo_hand_down ( dest_ang,current_angle_y,5);
			}
			
			
			
		}
		
		
		
	}
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






