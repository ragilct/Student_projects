

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
 
#ifndef UART_H
#define UART_H


// Function Prototypes:
void USART_Init();
void send_data( unsigned char data );
void send_string ( char *str );


#endif