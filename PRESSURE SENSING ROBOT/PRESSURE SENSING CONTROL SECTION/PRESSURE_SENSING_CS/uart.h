

#include <avr/io.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)


// Function Prototypes:
void uart_init(void);
void uart_putc(unsigned char uart_data);
void uart_puts(char *uart_p);


//----------------------------------------------
void uart_init(void)
{
	UBRRL = BAUD_PRESCALE;
   	UBRRH = (BAUD_PRESCALE >> 8);
   	UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);         // 8-bit, no parity, 1 stop bit
	
}
//--------------------------------------------
void uart_putc(unsigned char uart_data)
{
	while((UCSRA &(1<<UDRE)) == 0);
	UDR = uart_data;
}
//-------------------------------------------
void uart_puts(char *uart_p)
{
   while(*uart_p)
   uart_putc(*uart_p++);
}
//--------------------------------------------

