/*
 * accelerometer_load_switch.c
 *
 * Created: 1/11/2014 9:34:03 AM
 *  Author: ADMIN
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void)
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////	
    
			  DDRB = 0x08;   // Fast PWM on timer zero
			  DDRD = 0x80;   // Fast PWM on timer2 and ext int1 7th and 3rd pin
    
			  OCR0 = 25;    // ocr0=25 for 800us .(oc0=1 after 800 us).for 8mhz clock
			  OCR2 = 25;    // ocr2=25 for 800us .(oc0=1 after 800 us).for 8mhz clock
	   
		      TCNT0 = 0x00; 
			  TCNT2 = 0x00; 
			  
			  TIMSK = 0x82;//compare match int for timer 1&2
	   
			  TCCR0 = 0x7C;//Fast pwm,oc0=1 on compare match, prescalar=256
			  TCCR2 = 0x7E;//Fast pwm,oc0=1 on compare match, prescalar=256
			  
///////////////////////////////////////////////////////////////////////////////////////////////////////////

			  
			  PORTD = 0x7F;

			  GICR = 0x80; // int1 enable  . set GICR before MCUCR 
			  
		      MCUCR	= 0x08; // int1 on falling edge
			 			 
			  
////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		      		  
			  
			  
			  
			  sei();
	   
	while(1);   
    
}


ISR(INT1_vect)
{
	TCCR0 = 0x78;
	
	OCR0 = 50;
	
	TCNT0=0x00;
	
	TCCR0 = 0x7C;
}


ISR(TIMER0_COMP_vect)
{
	_delay_us(125);
	
	TCCR0 = 0x78;
	
	TCNT0 =0xfe;
	
	TCCR0 = 0x7c;
}


ISR(TIMER2_COMP_vect)
{
	_delay_us(125);
	
	TCCR2 = 0x78;
	
	TCNT2 =0xfe;
	
	TCCR2 = 0x7E;
}
