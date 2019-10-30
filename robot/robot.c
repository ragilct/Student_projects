

//#define F_CPU 11059800UL 
#define F_CPU 6000000
#include <util/delay.h>
#include <avr/io.h>

#include <avr/interrupt.h> //INCLUDE HEADER FILES

int temp,hum,co2;

ISR(USART_RXC_vect)
{
char a;
a=UDR;
if(a=='s')
PORTD=0B10100000;
if(a=='w')
PORTD=0B01010000;
if(a=='a')
PORTD=0B01100000;
if(a=='d')
PORTD=0B10010000;
if(a=='q')
PORTD=0x00;
}

void USARTInit(uint16_t ubrr_value)
{
	
	
	UCSRB |= ((1<<RXEN)|(1<<TXEN)|(1<<RXCIE));								   // Enable receiver and transmitter 
		
    UBRRL = 38;
		
	UCSRC |= ((1 <<URSEL) | (1 << UCSZ0 ) | (1 << UCSZ1 ));					   // Use 8- bit character sizes
}

void USARTWriteChar(char data)
{
	while(!(UCSRA & (1<<UDRE)))
	{
	}
	UDR=data;
}



void convert_send(int v)
{
  unsigned char s1= 0x30, s2=0x30, s3=0x30;
  s1 = s1 | (v%10);
  v=v/10;
  s2 = s2 | (v%10);
  v=v/10;
  s3 = s3 | (v%10);
   
  
 USARTWriteChar(s3);
 USARTWriteChar(s2);
 USARTWriteChar(s1);
}


void main()
{
DDRD=0xff;
DDRB=0x00;
temp=0,hum=0,co2=0;
ADCSRA|=(_BV(ADEN) | _BV(ADPS2) | _BV(ADPS0)) ; //normal conversion without interrupt

USARTInit(71); 
sei();
while(1)
{
int i,a;

a=0;

ADMUX=0B11100010;	
for(i=0;i<14;i++)
{
ADCSRA|=_BV(ADSC); //start conversion
while(!((ADCSRA&_BV(ADIF))>>ADIF));
a=a+ADCH;
}
a=a/30;
a=200-a;
co2=a;
_delay_ms(500);

/
USARTWriteChar('C');               // adc 02
USARTWriteChar('O');
USARTWriteChar('2');
USARTWriteChar(' ');
USARTWriteChar('L');
USARTWriteChar('E');
USARTWriteChar('V');
USARTWriteChar('E');
USARTWriteChar('L');
USARTWriteChar('=');
convert_send(co2);
USARTWriteChar(0X0A);
USARTWriteChar(0X0D);
_delay_ms(100);
USARTWriteChar('p');
USARTWriteChar('i');
USARTWriteChar('r');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');

if(PINB & (1<<PB0))
{
USARTWriteChar('N');
USARTWriteChar('O');
USARTWriteChar('T');
}
USARTWriteChar(' ');
USARTWriteChar('D');
USARTWriteChar('E');
USARTWriteChar('T');
USARTWriteChar('E');
USARTWriteChar('C');
USARTWriteChar('T');
USARTWriteChar('E');
USARTWriteChar('D');
USARTWriteChar(0X0a);
USARTWriteChar(0X0d);
_delay_ms(100);
_delay_ms(50);



}

}
