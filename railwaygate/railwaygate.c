/*
 * railwaygate.c
 *
 * Created: 3/28/2014 9:56:34 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.c"

#define BUZZER_ON  (PORTB|=(1<<PB1))
#define BUZZER_OFF (PORTB&=(~(1<<PB1)))

#define GREEN_LIGHT_ON  (PORTB|=(1<<PB2))
#define GREEN_LIGHT_OFF (PORTB&=(~(1<<PB2)))

#define RED_LIGHT_ON  (PORTB|=(1<<PB3))
#define RED_LIGHT_OFF (PORTB&=(~(1<<PB3)))

#define SENSOR_1  (!(PIND&(1<<PD3)))
#define SENSOR_2  (!(PIND&(1<<PD4)))

#define TS  (!(PINC&(1<<PC0)))
#define DS  (!(PINC&(1<<PC1)))

#define M1_ON  (PORTD|=(1<<PD0))
#define M1_OFF (PORTD&=(~(1<<PD0)))
#define M2_ON  (PORTD|=(1<<PD1))
#define M2_OFF  (PORTD&=(~(1<<PD1)))

#define MESSEGE_ON   (PORTB|=(1<<PB4))
#define MESSEGE_OFF  (PORTB&=(~(1<<PB4)))


char s1_flag = '0',s2_flag = '0';


void system_init()
{
	DDRB |= (1<<PB1); // buzzer
	
	DDRB |= (1<<PB2); // green light
	
	DDRB |=(1<<PB3);  // red light
	
	DDRD &= (~(1<<PD3)); // sensor_1
	PORTD |= (1<<PD3);
	
	DDRD &= (~(1<<PD4));  //sensor_2
	PORTD |= (1<<PD4);
	
	DDRC &=(~(1<<PC0)); // ts
	PORTC |= (1<<PC0);
	
	DDRC &=(~(1<<PC1)); // ds
	PORTC |= (1<<PC1);
	
	DDRD |= (1<<PD0); // m1
	
	DDRD |= (1<<PD1); // m2
	
	DDRB |= (1<<PB4); // messege
	
	LCDInit(LS_NONE);
	
}



void gate_close();
void gate_open();
void messege();


int main(void)
{
	
	system_init();
	
	
	gate_open();
	
	BUZZER_OFF;
	RED_LIGHT_OFF;
	GREEN_LIGHT_ON;
	
	while(1)
    {
			   if( (SENSOR_1))
	   
			   {
				   /*
				   s1_flag = '1';
				   s2_flag = '0';
				   */
				   RED_LIGHT_ON;
				   GREEN_LIGHT_OFF;
				
					MESSEGE_ON;
				   LCDClear();
				   LCDWriteString("STOP1");
				   BUZZER_ON;
				   gate_close();
		   
				   _delay_ms(500);
		   
				   BUZZER_OFF;
		   
				   while(!SENSOR_2);
		   
				   while(SENSOR_2);
				   
				   MESSEGE_OFF;
		   
				   RED_LIGHT_OFF;
				   GREEN_LIGHT_ON;
					
				   
				   LCDClear();
				   LCDWriteString("GO1");
		
				   gate_open();
				   
				   _delay_ms(2000);
		  
		 			   
			  }	
	  
			  else if( (SENSOR_2) )
			  {
				  /*
				  s2_flag='1';
				  s1_flag = '0';
				  */
				   RED_LIGHT_ON;
				   GREEN_LIGHT_OFF;
				 //  messege();
				   MESSEGE_ON;
				   LCDClear();
				   LCDWriteString("STOP2");
				   BUZZER_ON;
				   gate_close();
		   
				   _delay_ms(500);
		   
				   BUZZER_OFF;
		   
				   while(!SENSOR_1);
		   
				   while(SENSOR_1);
				   
				   MESSEGE_OFF;
		   
				   RED_LIGHT_OFF;
				   GREEN_LIGHT_ON;
		   
				   LCDClear();
				   LCDWriteString("GO2");
		
				   gate_open();
				   
				   _delay_ms(2000);
			  }	
	  
	 }
	 
}


void gate_open()

{
	
	M1_ON;
	M2_ON;
	
	while(!TS)
	M2_OFF;
	
	M2_ON;
	
}

void gate_close()
{
	M1_ON;
	M2_ON;
	while(!DS)
	M1_OFF;
	
	M1_ON;
	
}

void messege()
{
	MESSEGE_ON;
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	
	MESSEGE_OFF;
}

