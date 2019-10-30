/*
 DRIVER FOR AVR IO OPERATION OF ATMEGA8 MICROCONTROLLER
 
 
 PIN_CONFIG.h
 
 Author			: ARUN V CHIDAMBARAM
 
 Designation	: EMBEDDED SYSTEM DEVELOPER
 
 Email			: arsej007@gmail.com

 Mobile			: 9496115504

 Company		: V-ELECTRO,VELLAYAMBALAM,THIRUVANANTHAPURAM,KERALA

			    
 */ 


#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_


#include <avr/io.h>
#include <inttypes.h>
#include <stdarg.h>



#define INPUT	0
#define OUTPUT	1


#define PULL_UP_DISABLE	0
#define PULL_UP_ENABLE	1


#define HIGH 1
#define LOW	 0




#define PortA 0				//  PORT A setting

#define PinA0	0x00
#define PinA1	0x01
#define PinA2	0x02
#define PinA3	0x03
#define PinA4	0x04
#define PinA5	0x05
#define PinA6	0x06
#define PinA7	0x07



#define PortB 1				//  PORT B setting

#define PinB0	0x10
#define PinB1	0x11
#define PinB2	0x12
#define PinB3	0x13
#define PinB4	0x14
#define PinB5	0x15
#define PinB6	0x16
#define PinB7	0x17

							

#define PortC 2				//  PORT C setting


#define PinC0	0x20
#define PinC1	0x21
#define PinC2	0x22
#define PinC3	0x23
#define PinC4	0x24
#define PinC5	0x25
#define PinC6	0x26
#define PinC7	0x27




#define PortD 3				//  PORT D setting


#define PinD0	0x30
#define PinD1	0x31
#define PinD2	0x32
#define PinD3	0x33
#define PinD4	0x34
#define PinD5	0x35
#define PinD6	0x36
#define PinD7	0x37

#define bit_set(sfr, bit) (sfr |= (1 << bit))
#define bit_clr(sfr, bit) (sfr &= ~(1 << bit))

void config_Port_pin( uint8_t port,uint8_t pos,uint8_t io,uint8_t pull_up );
void config_pin_input( uint8_t port,uint8_t pos,uint8_t pull_up);
void config_pin_output(uint8_t port,uint8_t pos);
void output_pin( uint8_t port,uint8_t pos,uint8_t state );
uint8_t read_pin (	uint8_t port,uint8_t pos);
uint8_t read_pin_wp (	uint8_t port,uint8_t pos);
uint8_t  read_pin_group ( int num,...);
uint8_t  read_pin_group_wp ( int num,...);
void output_pin_group(int num,...);

/*
void config_Port_pin( uint8_t ,uint8_t ,uint8_t ,uint8_t );
void config_pin_input( uint8_t,uint8_t,uint8_t );
void config_pin_output(uint8_t,uint8_t);
void output_pin( uint8_t,uint8_t,uint8_t );
uint8_t read_pin ( uint8_t,uint8_t );
uint8_t read_pin_wp ( uint8_t,uint8_t );
uint8_t  read_pin_group ( int ,...);
uint8_t  read_pin_group_wp ( int ,...);
void output_pin_group(int,...);
*/


#endif /* PIN_CONFIG_H_ */





/////////////////// EXAMPLE ///////////////////////////////////////////////

/*

#define F_CPU 6000000
#include <avr/io.h>
#include <util/delay.h>

#include "PIN_CONFIG.h"



int main(void)
{
	uint8_t x;
	
	while(1)
	{
		//output_pin(PortD,PD6,HIGH);
		//x=read_pin_group_wp(8,PinB7,PinB6,PinB5,PinB4,PinB3,PinB2,PinB1,PinB0); // MSB FIRST
		//output_pin_group(8,x,PinD0,PinD1,PinD2,PinD3,PinD4,PinD5,PinD6,PinD7);  // LSB FIRST
		//DDRD = 0xFF;
		
		//	x= read_pin_group_wp(4,PortB,PinB0,PinB1,PinB2,PinB3); // MSB first
		//output_pin_group(4,x,PinD0,PinD1,PinD2,PinD3);//,PinD4,PinD5,PinD6,PinD7);
		
		//output_pin(PORT_C,PC4,HIGH);
		
		//output_pin_group(4,x,PortC,PC0,PortC,PC1,PortC,PC2,PortC,PC3); // LSB first
		
		//x= read_pin_group_wp(2,PinD1,PinD2); // MSB first
		
		//output_pin(PortC,PC2,HIGH);
		//bit_set(PORTB,PB0);
		//bit_clr(PORTB,PB0);
		
	}
}


*/