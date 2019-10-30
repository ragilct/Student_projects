
#define F_CPU 6000000
#include <util/delay.h>
#include <avr/io.h>

#include <avr/interrupt.h> 


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






int main()

{
	
DDRD=0xff;

DDRB |= (1<<PB0);
PORTB |= (1<<PB0);

DDRC |= (1<<PC2);
PORTC |= (1<<PC2);



USARTInit(71); 

sei();

while(1)
{

USARTWriteChar('C');               // adc 02
USARTWriteChar('O');
USARTWriteChar('2');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');

if( (PINB & (1<<PB0)  ))
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
USARTWriteChar('\n');
USARTWriteChar('\r');
_delay_ms(500);



_delay_ms(100);
USARTWriteChar('p');
USARTWriteChar('i');
USARTWriteChar('r');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');

if( (PINC & (1<<PC2)) )
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
USARTWriteChar('\n');
USARTWriteChar('\r');

_delay_ms(500);



}

}
