

#define F_CPU 6000000UL
#define BAUD_RATE 9600
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "adc.h"
#include "uart.h"

#define bit_set(sfr, bit)	(sfr |= (1 << bit))
#define bit_clr(sfr, bit)	(sfr &= ~(1 << bit))

void gsm_init();
void gsm_send_msg(char *msg, char *mob);
void gsm_delete_msg();


char mob_no[]			= "9998887770";

//========================================= MAIN FUNCTION ======================================

int main(void)
{
	
	uart_init();
	gsm_init();


	while(1)
	{
		gsm_send_msg("gsm checking msg..",mob_no);

	}	
}
	
   //*********************** GSM ***********************

void gsm_init()
{
	cli();
	
	uart_puts("AT\r\n");
	_delay_ms(200);
	
	uart_puts("AT\r\n");
	_delay_ms(200);
	
	uart_puts("AT\r\n");
	_delay_ms(200);
	
	uart_puts("ATE0\r\n");
	_delay_ms(200);
	
	uart_puts("AT+CMGF=1\r\n");
	_delay_ms(1000);
	
	sei();
}

//====================================

void gsm_send_msg(char *msg, char *mob)
{
	cli();

	uart_puts("AT+CMGS=\"");
	uart_puts(mob);
	uart_puts("\"\r\n");
	_delay_ms(500);
	uart_puts(msg);
	_delay_ms(500);
	uart_putc(0x1A);
	_delay_ms(2000);
	sei();
}

//==============================

void gsm_delete_msg()
{
	cli();

	uart_puts("AT+CMGD=1\r\n");
	_delay_ms(1000);

	sei();
}

//**********************************

