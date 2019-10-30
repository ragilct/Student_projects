/*
 * loadswitch16.c
 *
 * Created: 1/9/2014 10:18:26 AM
 *  Author: ADMIN
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void extinterrupt_control();
void timer0_control_program();

unsigned char t=100;

ISR(TIMER0_COMP_vect)
{
	 int i;	 
		 
	 PORTB |= (1<<PB1);
	for(i=0;i<150;i++);
	PORTB &= (~(1<<PB1));
	
	OCR0 = t;	
	TCNT0 =0x00;
//	TIFR &= (~(1<<2));
	 	
}


ISR (INT1_vect)
{
	
	t=20;
}

int main(void)
{
	DDRB |= (1<<DDB1);
	DDRA =0xff;
	
   extinterrupt_control();
   
   timer0_control_program();
   
   sei();
	
     while(1);
	 	//PORTB &= (~(1<<PB1));
	
	
    
        
}

void extinterrupt_control()
{
	
	
	MCUCR  |=  ((1<<ISC11)|(1<<ISC10));
	
	//MCUCR  |=  (1<<ISC11);
	
	GICR   |=  ( 1<<INT1 );

	DDRD   &=  (~( 1<<DDD3 ));
	
	PORTD  |=  ( 1 <<PD3 );
}

void timer0_control_program()
{
	    TCNT0 =0x00;
		OCR0 = 100;
		PORTB &= (~(1<<PB1));
	  
	    TIMSK |= (1<<OCIE0);  
		TCCR0 |= (1<<CS02)|(1<<CS00);
	 
	 //TCCR0 |= (1<<CS00);
	 
	

}



//////////////////////////////////////////////////////


