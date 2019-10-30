/*

 PIN_CONFIG	1.0

 DRIVER FOR IO OPERATION OF ATMEGA MICROCONTROLLER
 
 THIS DRIVER CAN BE USED ONLY TO CONFIGURE 
 
	PORTB, PORTC, AND  PORTD
 
 
 PIN_CONFIG.h
 
 Author			: ARUN V CHIDAMBARAM
 
 Designation	: EMBEDDED SYSTEM DEVELOPER
 
 Email			: arsej007@gmail.com

 Mobile			: 9496115504

 Company		: V-ELECTRO,VELLAYAMBALAM,THIRUVANANTHAPURAM,KERALA
 
 
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 
 This driver can be effectively used for read and output to a pin or to a group of pins without bothering 
 about internal register settings.
 
 
 /////////////.................THE INCLUDE FILES.................
 
 
	PIN_CONFIG.h  // should be included in the user program
	
	PIN_CONFIG.c  // should be added to user program 
 
 
 
/////////...................... WRITIING OPERATION..............
 
 
............... output_pin( );............................
 
	This function can be used for controlling output of single pin
	
	ex:	output_pin( PortB,4,HIGH); // 4th pin of PORTB is high
	
		
................output_pin_group( );	.........................................
	
	This function can be used for controlling output of a group of pins
	
	ex: output_pin_group(4,x,PinD0,PinD1,PinD2,PinD3); 
	
	// first digit is the number of pins used , second the variable that contains output data,
	//	from third position pins are given..LSB is given first.The data may be a 16 bit value.
 


//////////....................READING OPERATION........................

........................read_pin ( );................

	  This function is used for reading pins without pull up, returns 1 if pin is high
	  
	  ex:  read_pin ( PortB,5);
	  
	  
........................read_pin_wp (  port, pos);............
	
	 This function is used for reading pins with pull up, returns 1 if pin is grounded
	 
	 ex:  read_pin_wp ( PortB,5);
	 
	 
........................ read_pin_group ( );....................
	 
	 This function is used for reading group of pins without pull up, returns a right shifted 16 bit value
	 
	 MSB pin should be given first.
	 
	 ex: x = read_pin_group(4,PinB7,PinB6,PinB5,PinB4); // 4 represents number of pins used for reading
	
	 // PinB7 is the MSB
	 // stores a left shifted value in variable x
	 
	 
	 
	 
	 
........................ read_pin_group_wp ( );....................


	This function is used for reading group of pins with pull up, returns a right shifted 16 bit value

	MSB pin should be given first.

	ex: x = read_pin_group_wp (4,PinB7,PinB6,PinB5,PinB4); // 4 represents number of pins used for reading

	// PinB7 is the MSB
	// stores a left shifted value in variable x
	 
	 
	 
	 

			    
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




void config_Port_pin( uint8_t port,uint8_t pos,uint8_t io,uint8_t pull_up ); /*This is the general function 
 used for configuring port pins as input or output .The parameters of this function are 
 port(its value will be 0,1,2,3 for PORTA,PORTB,PORTC,PORTD respectively),
 pos (this represents position of the pin.it can be 0,1,2...,7)
 io (this is for selection of input or output.its value will be 0 for input and 1 for output)
 pull_up(It determines pull ups are enabled or not.its value will be 0 for pull up disable and 1 for pull up enable)*/



void config_pin_input( uint8_t port,uint8_t pos,uint8_t pull_up);/*These function is used for configuring pins as input. The pull up can be 
enabled or disabled depends on pull_up parameter value */
void config_pin_output(uint8_t port,uint8_t pos);/* These function is used for configuring pin as output*/


uint8_t read_pin (	uint8_t port,uint8_t pos); /* this function is used for reading pins without pull up*/
uint8_t read_pin_wp (	uint8_t port,uint8_t pos);/* this function is used for reading pins with pull up*/

uint8_t  read_pin_group ( int num,...);/* this function is used for reading a group of  pins without pull up.
The return value of this function is right shifted value.
The function returns 16 bit value so it is possible to read 16 pins simultaneously.*/
uint8_t  read_pin_group_wp ( int num,...);/* this function is used for reading a group of  pins with pull up.
The return value of this function is right shifted value.
The function returns 16 bit value so it is possible to read 16 pins simultaneously.*/

void output_pin( uint8_t port,uint8_t pos,uint8_t state );/*This function is used to output HIGH or LOW . The state parameter 
represents the state of the pin.it can be 0 for LOW and 1 for HIGH */
void output_pin_group(int num,...);/*This function is used to output HIGH or LOW to a group of pins. The state parameter
represents the state of the pin.it can be 0 for LOW and 1 for HIGH .*/


#endif /* PIN_CONFIG_H_ */