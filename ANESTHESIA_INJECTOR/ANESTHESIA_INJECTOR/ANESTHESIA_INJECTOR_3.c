/*
 * ANESTHESIA_INJECTOR_3.c
 *
 * Created: 01-Apr-15 2:25:40 PM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/sfr_defs.h>




//////////////////////////////////////////////////////////////

#include "PIN_CONFIG.h"
#include "ADC_CONFIG.h"
#include "SERVO_FS5115.h"
#include "lcd.h"
#include "EXT_INT_CONFIG.h"



void timer0_init ();




// initialization function for header files
void system_init(void)
{
	 
	 servo_init_function ();	
	 Ext_interrupt_enable (INT0,FALLING_EDGE,PULL_UP_ENABLE);
	 Ext_interrupt_enable (INT1,FALLING_EDGE,PULL_UP_ENABLE);
	 LCDInit (LS_NONE);	
	 ADC_Init (AVCC);  
	 timer0_init();
	
}
////////////////////////////////////////////////////////////

/*
USED PINS


I/O

		Speed control switches

		PB0		Pin no: 1

		PB1		Pin no: 2

		PB2		Pin no: 3

		PB3		Pin no: 4


ADC

		Temperature sensor
		
		PA0		ADC0		Pin no: 40




SERVO

		Timer 1 PWM outputs

		PD4		OC1B		Pin no: 18
		PD5		OC1A		Pin no: 19



LCD

		Control pins

		PC0		RS		Pin no: 22

		PC1		RW		Pin no: 23

		PC2		E		Pin no: 24

		Data pins

		PC3		D0		Pin no: 25

		PC4		D1		Pin no: 26

		PC5		D2		Pin no: 27

		PC6		D3		Pin no: 28
		

RESPIRATION SENSOR

		External interrupt 0
		
		PD2		INT0	Pin no: 16
		
HEART BEAT SENSOR

		External interrupt 1
		
		PD3		INT1	Pin no:17



*/
	
	
	
	
	



#define SPEED_1		read_pin_wp (PortB,PB0)
#define SPEED_2		read_pin_wp (PortB,PB1)
#define SPEED_3		read_pin_wp (PortB,PB2)
#define SPEED_4		read_pin_wp (PortB,PB3)


#define		BUZZER_ON		output_pin (PortD,PD0,HIGH)
#define		BUZZER_OFF		output_pin (PortD,PD0,LOW)


////////////////////////////////////////////////////////////
// user defined functions

uint32_t get_temperature(void);




///////////////////////////////////////////////////////////
// global variables

volatile	uint16_t	HRCount=0,HeartRate=0,sec=0;
volatile	uint16_t	capt_period = 0;
volatile	uint16_t	t_v = 0;
volatile	uint32_t	temperature = 0;
volatile	uint8_t		HeartRate_flag = 0;
volatile	uint8_t		respiration_flag = 0;
volatile	uint8_t		temperature_flag = 0;
volatile	uint8_t		respiration_count = 0;
volatile	uint8_t		i_resp = 0,i_heart=0;
volatile	uint8_t		speed_flag = 0;
volatile	uint8_t		speed_inc_flag = 0;
volatile	uint8_t		prev_angle =30 ;
volatile	uint8_t		ct_ang = 30;
volatile	uint8_t		pre_speed_inc_flag = 0;
volatile	uint8_t		inject_count = 0;
volatile	uint8_t		state_val = 0;
volatile	uint16_t	ir_val = 0;
volatile	uint8_t		ir_buzzer_flag = 0;
///////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////


int main(void)
{
	
	system_init ();
	
	LCDClear();
	LCDWriteString ("INITIALIZING");
	_delay_ms (2000);
	LCDClear();
	 sei();
		
    while(1)
    {
        
		
		ir_val = ADC_Read (1);
						
		if (ir_val > 500 )
		{
			BUZZER_ON;
			
			ir_buzzer_flag = 1;
		}
		else
		{
			BUZZER_OFF;
			
			ir_buzzer_flag = 0;
		}
		
		
		
		if (SPEED_1)
		{
			speed_flag = 1;
			
			servo_angle_set_x_axis (30);
			
			ct_ang = 30;
			
			prev_angle = 30;
		}
		else if ( SPEED_2)
		{
			speed_flag = 2;
			
			servo_angle_set_x_axis (30);
			
			ct_ang = 30;
			
			prev_angle = 30;
		}
		else if ( SPEED_3)
		{
			speed_flag = 3;
			
			servo_angle_set_x_axis (30);
			
			ct_ang = 30;
			
			prev_angle = 30;
			
		}
		else if (SPEED_4)
		{
			speed_flag = 0;
			
			servo_angle_set_x_axis (30);
			
			ct_ang = 30;
			
			prev_angle = 30;
			
		}
		
		
			
		if ( ( HeartRate_flag == 1 ) || ( respiration_flag == 1 ) || ( temperature_flag == 1 ) )
		{
			state_val = 1;
		}
		else if ( (HeartRate_flag == 0) || ( respiration_flag == 0 ) || ( temperature_flag == 0))
		{
			state_val = 10;
		}
		else
		{
			state_val = 5;
		}
		
		
		
		
		
		if ( ( ( HeartRate_flag == 1 ) && ( respiration_flag == 1 ) && ( temperature_flag == 1 ) ) )
		{
			
			BUZZER_ON;
		}
		else
		{
			
			if (ir_buzzer_flag == 0)
			{
				BUZZER_OFF;
			}
			
		}
		
		
		
		
		
		
		
		inject_count ++;
		
		if (inject_count > state_val)
		{
			inject_count = 0;
			
			prev_angle = ct_ang;
			ct_ang = prev_angle - speed_flag;
			
			if (ct_ang <= 1 )
			{
				ct_ang = 0;
				
				prev_angle = 0;
				
				speed_flag = 0;
			}
			
			
			prev_angle = ct_ang;
			servo_angle_set_x_axis (ct_ang);
			
		}
		
		
			
				
		if ( HeartRate_flag == 1 )
		{
			HeartRate = 0;
			LCDWriteStringXY(0,0,"HR");
			LCDWriteStringXY(0,1,"H");
		}
		else if( HeartRate_flag == 0 )
		{
			LCDWriteStringXY(0,0,"HR");
			LCDWriteStringXY(0,1,"L");
		}
		else
		{
			LCDWriteStringXY(0,0,"HR");
			LCDWriteStringXY(0,1,"N");
		}
		
		
		
		
		
		if ( respiration_flag == 0 )
		{
			LCDWriteStringXY(4,0,"RE");
			LCDWriteStringXY(4,1,"L");
		}
		else if( respiration_flag == 1)
		{
			LCDWriteStringXY(4,0,"RE");
			LCDWriteStringXY(4,1,"H");
		}
		else
		{
			LCDWriteStringXY(4,0,"RE");
			LCDWriteStringXY(4,1,"N");
		}
		
			
		
		
		if ( temperature_flag == 0 )
		{
			LCDWriteStringXY(8,0,"TMP");
			LCDWriteStringXY(8,1,"L");
		}
		else if( temperature_flag == 1)
		{
			LCDWriteStringXY(8,0,"TMP");
			LCDWriteStringXY(8,1,"H");
		}
		else
		{
			LCDWriteStringXY(8,0,"TMP");
			LCDWriteStringXY(8,1,"N");
		}
		
		
		
			
       
    }
	
}



////////////////////////////////////////////////////////////////////////////

uint32_t get_temperature(void)
{
	uint16_t temp_val=0;
	temp_val = ADC_Read(0);
	return (temp_val*500/1024);
}

///////////////////////////////////////////////////////////////////////////

ISR(INT0_vect)
{
	respiration_count ++;
	
	
}


ISR(INT1_vect)
{
	HRCount++;
	
		
}


/////////////////////////////////////////////////////////////////////////






void timer0_init()
{
	TIMSK |= _BV(TOIE2);
	TCNT2 = 0;
	TCCR2 |= _BV(CS22) | _BV(CS21) | _BV(CS20);
	
}


ISR (TIMER2_OVF_vect)
{
	
	static uint16_t x,y;
	
	x++;
	
	if ( x > ( 23 * 10 ) )
	{
		x=0;
		
				temperature = get_temperature ();
		
				if ( temperature < 25 )
				{
					temperature_flag = 0;
				}
				else if(temperature > 45)
				{
					temperature_flag = 1;
				}
				else
				{
					temperature_flag = 2;
				}
		
					
			
				if ( HRCount < 15 )
				{
					HeartRate_flag = 0;
				}
				else if( HRCount >= 30 )
				{
					HeartRate_flag = 1;
				}
				else
				{
					HeartRate_flag = 2;
				}
				
						
		
		
				if (respiration_count < 10)
				{
					respiration_flag = 0;
				}
				else if ( respiration_count > 30 )
				{
					respiration_flag = 1;
				}
				else
				{
					respiration_flag = 2;
				}
				
				
							
				HRCount = 0;
				respiration_count = 0;
	
	}
	
	
	TCNT2 = 0;
	
}





/////////////////////////////////////////////////////////////////////////