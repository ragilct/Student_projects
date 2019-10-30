
#define F_CPU 6000000
#include <avr/io.h>
#include <util/delay.h>





int main()
{ 

	
	 DDRD=0XFF;         //inc pins set pb and pd as output
	 DDRB=0XFF;
	 PINC=0XF0;
	 int a;
	
	 _delay_ms(1000);
	 _delay_ms(1000);
	 _delay_ms(1000);
	 _delay_ms(1000);
	 
TCCR1A = 0;     // disable all PWM on Timer1 whilst we set it up
ICR1 = 199;   // frequency is every 20ms
TCCR1A = (1 << WGM11);
TCCR1B = (1 << WGM13) | (1<<WGM12) | (1 << CS10);
TCCR1A |= 2 <<  6;//EN  PWM

OCR1A =80;
	while(1)
	{
		
				
		a=PINC;
		
		if(a==1)
		{
		OCR1A =255;
		}
		
		if(a==2)
		{
		OCR1A =160;
		}
		if(a==100)
		{
		OCR1A =0;
		}
		
		if(a==4)
		{
		PORTD=0x00;
		}
		if(a==5)
		{
		PORTD=0B10000000;
		}
		if(a==6)
		{
		PORTD=0B01000000;
		}
		
		}
	 
	 
return 0;
}
