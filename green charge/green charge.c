/*
 * green_charge.c
 *
 * Created: 3/3/2014 9:44:23 AM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000UL


#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define LOAD_MAX_VALUE  3  
#define LOAD_READ 0
               
 
#define SOLAR_RELAY_ON  (PORTC|=(1<<PC3))    // RELAY 
#define SOLAR_RELAY_OFF (PORTC&=(~(1<<PC3)))

#define BAT_RELAY_ON    (PORTC|=(1<<PC4))
#define BAT_RELAY_OFF   (PORTC&=(~(1<<PC4)))

#define GRID_RELAY_ON   (PORTC|=(1<<PC5))
#define GRID_RELAY_OFF  (PORTC&=(~(1<<PC5)))

#define SAVI  (!(PIND&(1<<PD6)))              // SOLAR POWER AVAILABILITY  
#define BAVI  (!(PIND&(1<<PD7)))              // BATTERY POWER AVAILABILITY  
#define GAVI  (!(PINB&(1<<PB0)))              // GRID POWER AVAILABILITY  


#define SW1 (!(PIND&(1<<PD3)))				 // SWITCHES
#define SW2 (!(PIND&(1<<PD4)))
#define SW3 (!(PIND&(1<<PD5)))

#define BULB1_ON   (PORTD|=(1<<PD0))		 // BULBS
#define BULB1_OFF  (PORTD&=(~(1<<PD0)))
 
#define BULB2_ON   (PORTD|=(1<<PD1))
#define BULB2_OFF  (PORTD&=(~(1<<PD1)))

#define BULB3_ON   (PORTD|=(1<<PD2))
#define BULB3_OFF  (PORTD&=(~(1<<PD2)))





#include "lcd.c"


void system_init();
void load_calculation();
unsigned int adc_read(char nadc);
void relay_control();
void  switch_control();



char time_string[5],time_flag  =  0;
unsigned int morning_load=0,evening_load=0,night_load=0,ret_value = 0;
int x=0,y=0,z=0,s=0;

unsigned char toggle1=0,toggle2=0,toggle3=0;

int main(void)
{
	int i;
	
	system_init();
	
	
	/*
	LCDClear();
	LCDWriteString("SET MODE");
	GRID_RELAY_ON;
	LCDWriteStringXY(0,1,"MORNING");
	time_flag  =  'M';
	for(i=1;i<15;i++)
			{

				itoa(i,time_string,10);
				
				LCDWriteStringXY(8,1,time_string);
						 
				_delay_ms(1000);
				
				 switch_control();			
				 load_calculation();
			}	
			
		morning_load = ret_value;
		ret_value = 0;
			
		/*	itoa(morning_load,time_string,10);
			LCDClear();
			LCDWriteString(time_string);
			
			while(1);
			
			
			
	LCDClear();
	LCDWriteString("SET MODE");
	LCDWriteStringXY(0,1,"EVENING");
	time_flag  =  'E';
	for(i=1;i<50;i++)
			{

				itoa(i,time_string,10);
				
				LCDWriteStringXY(8,1,time_string);
						 
				_delay_ms(1000);
				
				 switch_control();
				
				load_calculation();
			
			}	
	    evening_load = ret_value;
		ret_value = 0;
		
			
	
			
	LCDClear();
	LCDWriteString("SET MODE");
	LCDWriteStringXY(0,1,"NIGHT");
	time_flag  =  'N';
	for(i=1;i<50;i++)
			{

				itoa(i,time_string,10);
				
				LCDWriteStringXY(8,1,time_string);
										 
				_delay_ms(1000);
				
				 switch_control();
				
				load_calculation();
				
	
			}	
			
	    night_load = ret_value;
		ret_value = 0;
			
		GRID_RELAY_OFF;	
			
			*/
    while(1)
    {
		
		
			LCDClear();
			LCDWriteString("ACTIVE MODE");
			LCDWriteStringXY(0,1,"MORNING");
			time_flag  =  'M';
			if(morning_load >= LOAD_MAX_VALUE)
			{
				SOLAR_RELAY_OFF;
				BAT_RELAY_OFF;
				GRID_RELAY_ON;
			}
			for(i=1;i<50;i++)
					{

						itoa(i,time_string,10);
				
						LCDWriteStringXY(8,1,time_string);
						
						switch_control();
						 
						_delay_ms(1000);
						
						
				        load_calculation();
						//if( ((i%5) == 0) && (i!=45) )
						relay_control();
						 
					}	
			
				morning_load = ret_value;
				ret_value = 0;
			
			
			
			LCDClear();
			LCDWriteString("ACTIVE MODE");
			LCDWriteStringXY(0,1,"EVENING");
			time_flag  =  'E';
			if(evening_load > LOAD_MAX_VALUE)
			{
				SOLAR_RELAY_OFF;
				BAT_RELAY_OFF;
				GRID_RELAY_ON;
			}
			for(i=1;i<50;i++)
					{

						itoa(i,time_string,10);
				
						LCDWriteStringXY(8,1,time_string);
						 
						_delay_ms(1000);
				
						switch_control();
				        load_calculation();
						
						//if( ((i%5) == 0) && (i!=45) )
						relay_control();
			
					}	
				evening_load = ret_value;
				ret_value = 0;
		
			
			
			
			
			LCDClear();
			LCDWriteString("ACTIVE MODE");
			LCDWriteStringXY(0,1,"NIGHT");
			time_flag  =  'N';
			if(night_load > LOAD_MAX_VALUE)
			{
				SOLAR_RELAY_OFF;
				BAT_RELAY_OFF;
				GRID_RELAY_ON;
			}
			for(i=1;i<50;i++)
					{

						itoa(i,time_string,10);
				
						LCDWriteStringXY(8,1,time_string);
						 
						_delay_ms(1000);
						
						switch_control();
				        load_calculation();
						//if( ((i%5) == 0) && (i!=45) )
						relay_control();
				
	
					}	
			
				night_load = ret_value;
				ret_value = 0;
				
				
	}
}


void system_init()
{
	LCDInit(LS_NONE);
		
	DDRB |= ( (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5)  ); 
	DDRC |= ( (1<<PC0) | (1<<PC1) | (1<<PC3) | (1<<PC4) | (1<<PC5) );
	DDRD |= ( (1<<PD0) | (1<<PD1) | (1<<PD2) );
	
	DDRD &= ( ~( (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7) ) );
	PORTD |= ( (1<<PD3) | (1<<PD4) | (1<<PD5) |(1<<PD6) | (1<<PD7) );
	
	DDRB &= (~(1<<PB0));
	PORTB |= (1<<PB0);
}



void load_calculation()
{
	
	 int load_value;
	
	load_value = s;
	
	if(load_value > ret_value)
	ret_value = load_value;
	
}




void switch_control()
{
	if(SW1)
	{
		BULB1_ON;
		x=1;	
	}
	else
	{
		BULB1_OFF;
		x=0;	
	}
	
	
	if(SW2)
	{
		BULB2_ON;
		y=1;	
	}
	else
	{
		BULB2_OFF;
		y=0;	
	}
	
	
	if(SW3)
	{
		BULB3_ON;
		z=1;	
	}
	else
	{
		BULB3_OFF;
		z=0;	
	}
	
	s=x+y+z;
	
}


void relay_control()
{
			switch (time_flag)
	
					{
						case'M':
									{
										if((SAVI&& (s < LOAD_MAX_VALUE)))
										{
											GRID_RELAY_OFF;
											BAT_RELAY_OFF;
											SOLAR_RELAY_ON;
										}
										
										
										else if(( (BAVI) && (!GAVI) && (s < LOAD_MAX_VALUE) ) )
										
										{
											GRID_RELAY_OFF;
											SOLAR_RELAY_OFF;
											BAT_RELAY_ON;
																	
											
										}
										
										else 
										{
											SOLAR_RELAY_OFF;
											BAT_RELAY_OFF;
											GRID_RELAY_ON;
																														
										}
										
												
										break;							
																	
																		
								    }
										
										
					  case 'E':	
									{
										if((SAVI&& (s < LOAD_MAX_VALUE)))
										{
											GRID_RELAY_OFF;
											BAT_RELAY_OFF;
											SOLAR_RELAY_ON;
										}
										
										
										else if((BAVI && ( s < LOAD_MAX_VALUE)))
										
										{
											GRID_RELAY_OFF;
											SOLAR_RELAY_OFF;
											BAT_RELAY_ON;
										}
										
										else 
										{
											
											SOLAR_RELAY_OFF;
											BAT_RELAY_OFF;
											GRID_RELAY_ON;
																					
										}
										
												
										break;
										
									}
									
									
									
					case 'N':
									{
										if(( (BAVI) && (!GAVI ) && (s < LOAD_MAX_VALUE)))
										
										{
											
											GRID_RELAY_OFF;
											SOLAR_RELAY_OFF;
											BAT_RELAY_ON;
										}
										
										else 
										{
											
											SOLAR_RELAY_OFF;
											BAT_RELAY_OFF;
											GRID_RELAY_ON;
																					
										}
										
										break;
										
									}
									
									
																	
					}
					
					
}