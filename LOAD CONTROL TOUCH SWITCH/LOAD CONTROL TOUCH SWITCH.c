/*
 * LOAD_CONTROL_TOUCH_SWITCH.c
 *
 * Created: 12/28/2013 10:41:02 AM
 *  Author: Arun
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



void triac_firing (unsigned int);

unsigned int time_set=800;


ISR(INT0_vect)
{
	 time_set = time_set + 800;
	 
	if(time_set >= 8000) 
	{
		time_set=8000;
	}
	//sei();
}

ISR(INT1_vect)
{
	time_set = time_set - 800;
	 
	if(time_set <= 800) 
	{
		time_set=800;
	}
	//sei();
}

	

int main(void)
{

	
	DDRB=0x00;
	
	DDRC=0xff;
	
	PORTC = 0X00;
	
	
	MCUCR |= (1<<ISC01);
		
    GIMSK |= ((1<<INT0)|(1<<INT1));
		
	DDRD   &=  (~(( 1<<DDD2 )|(1<<DDD3)));
	
   PORTD  |=  (( 1 <<PD2 )|(1<<PD3));
	
     sei();
	
    while(1)
    {
		
		
		
  /*    if((PINB & (1<<PINB0) ) && (time_set <= 8000) && (~(PINB & (1<<PINB1) ) ) )            // to decrease speed
	     	  
	    time_set = time_set + 800;
			   	
	  else if ((PINB & (1<<PINB1)) && (time_set >= 800) && (~(PINB & (1<<PINB0) ) ) )       // to increase speed
	   
		time_set = time_set - 800 ;
*/	   
	  triac_firing(time_set);   
		
		
	
	   	   
    }
}


void triac_firing (unsigned int time_set)
{
	unsigned char zc = 0;
	
	//cli();
	while(zc != 0x04)
	{
		zc = PINB;
		
		zc &= 0x04;
	}
	
	                            // wait if portb2==0.zero crossing detection
	
	_delay_us(time_set);				 // phase angle selection
	
	PORTC |= (1<<PC0);			   // trigger pulse high on port c.0 pin
	
	_delay_us(250);               // trigger width
	
	PORTC &= (~(1<<PC0));      //  trigger  pulse low on port c.0 pin
	//sei();	
}