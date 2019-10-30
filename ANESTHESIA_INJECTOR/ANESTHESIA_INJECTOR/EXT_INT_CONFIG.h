/*
 * EXT_INT_CONFIG.h
 *
 * Created: 02-Dec-14 10:54:11 AM
 *  Author: ARUN V CHIDAMBARAM
 */ 


#ifndef EXT_INT_CONFIG_H_
#define EXT_INT_CONFIG_H_



#include <avr/io.h>
#include <avr/interrupt.h >
#include <inttypes.h> 



#define INT_port	PORTD
#define INT_ddr		DDRD
#define INT0_pin	PD2
#define INT1_pin	PD3



#define LOW_LEVEL		0B00
#define LOGICAL_CHANGE	0B01
#define FALLING_EDGE	0B10
#define RISING_EDGE		0B11



#define PULL_UP_ENABLE	1
#define PULL_UP_DISABLE	0



#define INT0_pos	0
#define INT1_pos	2



#define Int_sense_control_set(sense_bit,Int_pos)	( MCUCR |= ( sense_bit << Int_pos ) ) 

#define Ext_int_enable_set(Int_val)		( GICR |= ( 1 << Int_val ))

#define Ext_int_disable(Int_val)	( GICR &= (~ ( 1<< Int_val ) ) )



#ifndef  __AVR_ATmega8__

#define  INT2_port		PORTB

#define  INT2_ddr		DDRB

#define  INT2_pin		PB2


#define INT2_sense_RISING_EDGE()	( MCUCSR|=( 1 << ISC2))

#define INT2_sense_FALLING_EDGE()  (MCUCSR &= ( ~ ( 1 << ISC2 )))

#define INT2_int_enable()		( GICR |= ( 1 << INT2 ))

#define Ext_asynchronous_int_INT2_disable()	( GICR &= ( ~ ( 1 << INT2 )))


#endif



 
void Ext_interrupt_enable( uint8_t Int_num,uint8_t sense_bit,uint8_t pull_up_val );
void Sense_control_set ( uint8_t Int_val, uint8_t sense_bit);
void Int_pin_pull_up_set ( uint8_t Int_val);
void Ext_asynchronous_int_INT2_enable( uint8_t edge_bit,uint8_t pull_up_val );
#endif /* EXT_INT_CONFIG_H_ */