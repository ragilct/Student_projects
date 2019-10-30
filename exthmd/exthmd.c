/*
 * exthmd.c
 *
 * Created: 1/10/2014 10:09:02 AM
 *  Author: ADMIN
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char ZC;
unsigned int value;
unsigned int Count;

ISR(INT0_vect)
{
     ZC = 1;
}

void main()
 {
	 int i;
	
	//MCUCR  |=  (1<<ISC11);
	
	

	DDRD   = 0xFB;				  //PORTD all output except PD2 - INT0
	
	PORTD  =4;						 //Clear PORTD and enable pullup on PD2
	
	DDRB = 0;					    //PORTB all input
	 PORTB = 0xFF;				//Enable pullups on PORTB
									
	MCUCR  |=  ((1<<ISC01)|(1<<ISC00));			  //External interrupt on falling edge									 
											 
    GICR   |=  ( 1<<INT0 );			//Enable ext int 0
	
	GICR  &= (~(1<<INT1));			 //Disable ext int 1					
    								
   sei();				  //Enable interrupts

     while (1)
	 {
           if (ZC)
				{//zero crossing occurred
						Count = value;
				}						

					  for (i = 0; i < Count; i++)
					    {
						 _delay_us(800);
					   }	
						 

				   PORTD |= (1<<PD0); //Send a pulse
				  _delay_us(250);
				   PORTD &= (~(1<<PD0));
            
					ZC = 0;
           
     
			if( PINB = 0x00)		 
			{
				value = 3;
			}
	}				  
	}