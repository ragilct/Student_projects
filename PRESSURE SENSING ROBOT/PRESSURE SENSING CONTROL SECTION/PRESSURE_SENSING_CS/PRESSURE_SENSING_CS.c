/*
 * PRESSURE_SENSING_CS.c
 *
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "uart.h"
#include "PIN_CONFIG.h"
#include "lcd.h"

#define LED_ON	output_pin(PortB,PB1,HIGH);
#define LED_OFF	output_pin(PortB,PB1,LOW);

#define RIGHT_BTN	PD7
#define LEFT_BTN	PD5
#define FWD_BTN		PD6
#define BWD_BTN		PB0
#define STOP_BTN	PD4

#define PICK_BTN	PD3
#define RE_PICK_BTN	PD2

#define RIGHT	read_pin_wp(PortD,RIGHT_BTN)
#define LEFT	read_pin_wp(PortD,LEFT_BTN)
#define FWD		read_pin_wp(PortD,FWD_BTN)
#define BWD		read_pin_wp(PortB,BWD_BTN)
#define STOP	read_pin_wp(PortD,STOP_BTN)

#define GRIP	read_pin_wp(PortD,PICK_BTN)
#define REGRIP	read_pin_wp(PortD,RE_PICK_BTN)

void system_init();


volatile uint16_t adc_val = 0;
volatile uint16_t adc_previous = 0;
volatile uint8_t	disp_flag = 1;

volatile uint16_t y_ang = 0;
volatile uint16_t y_pre = 0;

volatile uint16_t	grip_pressure = 350;




int main(void)
{
	system_init();
	_delay_ms(500);
	LCDClear();
	LCDWriteStringXY(0,0,"PRESSURE SENSING");
	LCDWriteStringXY(6,1,"ROBOT");
	
	uart_puts ("PS_ROBOT_CTRL_INTIALIZE");
	
	uart_putc ('\n');

	
    while(1)
    {
        LED_ON;
		if(RIGHT)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"RIGHT");
			
			uart_puts ("PS_ROBOT_MOVE_RIGHT");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
			
		}
		
		else if(LEFT)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"LEFT");
			
			uart_puts ("PS_ROBOT_MOVE_LEFT");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
			
		}
		
		else if(FWD)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"FORWARD");
			
			uart_puts ("PS_ROBOT_MOVE_FORWARD");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
		
		}
		
		else if(BWD)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"BACKWARD");
			
			uart_puts ("PS_ROBOT_MOVE_BACKWARD");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
		
		}
		
		else if(STOP)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"STOP");
			
			uart_puts ("PS_ROBOT_MOVE_STOP");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
		
		}
		
		else if(GRIP)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"GRIP");
			
			grip_pressure = 350;
			
			uart_puts ("PS_ROBOT_CTRL_GRIP");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
			
			_delay_ms(1000);
			
		}
		
		else if(REGRIP)
		{
			
			char	re_grip_str[30] = "PS_ROBOT_CTRL_PRESSURE=";
			
			char	temp_re_grip_str[20];
			
			grip_pressure = grip_pressure + 50;	
			
			if (grip_pressure >= 600 )
			{
				grip_pressure = 600;
			}
					
			itoa (grip_pressure,temp_re_grip_str,10);
			
			
			LCDClear();
			LCDWriteStringXY(0,0,"REGRIP");
			
			LCDWriteIntXY(10,0,grip_pressure,4);
			
			
			strcat (re_grip_str,temp_re_grip_str);
			strcat( re_grip_str,"$");
			uart_puts (re_grip_str);
			
			
				
			
			uart_puts ("_REGR_IP");
			
			uart_putc ('\n');
			
			disp_flag = 1;
			
			_delay_ms(1000);
			
		}
		
		
		else
		{
			
			if (disp_flag == 1 )
			{
				LCDClear();
				LCDWriteStringXY(0,0,"PRESSURE SENSING");
				LCDWriteStringXY(6,1,"ROBOT");
				
				disp_flag = 0;
			}
			
		//	_delay_ms(100);
		}
		
		
		
		adc_val = ADC_Read (3);
		
		y_ang = ( adc_val/10.23) + 50;
		
		
		int16_t y_ang_tst,y_pre_tst,y_diff;
		
		y_ang_tst = ( int16_t)y_ang;
		
		y_pre_tst = (int16_t)y_pre;
		
		y_diff	= y_pre_tst - y_ang_tst;
		
		if ( ( y_diff > 2 ) || ( y_diff < -2 ) )
		{
			y_pre = y_ang;
			
			char	y_str[30]="PS_ROBOT_YPOS=";
			char	temp_str[10];
			
			LCDClear();
			
			LCDWriteStringXY(0,0,"ROBO HAND POS");
			
			LCDWriteIntXY(0,1,y_ang,4);
			
			_delay_ms (1000);
			
			
			itoa(y_ang,temp_str,10);
			
			strcat( y_str,temp_str);
			
			strcat(y_str,"$");
			
			
			uart_puts (y_str);
			uart_putc ('\n');
			
			
			LCDClear();
			LCDWriteStringXY(0,0,"PRESSURE SENSING");
			LCDWriteStringXY(6,1,"ROBOT");
			
			
					
		}
		
		
		
		
		
		
		
		
		
		
		
		
    }
}



//********************************************

void system_init()
{
	ADC_Init();
	uart_init();
	LCDInit(LS_NONE);
	
}



/*

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
		
		
		
		
		
	}
	
}

*/