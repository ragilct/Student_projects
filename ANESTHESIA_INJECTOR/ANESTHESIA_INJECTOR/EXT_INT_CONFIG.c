/*
 * EXT_INT_CONFIG.c
 *
 * Created: 02-Dec-14 10:55:31 AM
 *  Author: ARUN V CHIDAMBARAM
 */ 

#include "EXT_INT_CONFIG.h"





void Ext_interrupt_enable( uint8_t Int_num,uint8_t sense_bit,uint8_t pull_up_val )
{
	Sense_control_set(Int_num,sense_bit);
						
	if(pull_up_val)
	Int_pin_pull_up_set(Int_num);
				
	Ext_int_enable_set(Int_num);
}



 void Int_pin_pull_up_set ( uint8_t Int_val)
{
	switch(Int_val)
	{
		
		case INT0:
		{
			INT_ddr &= ( ~(1<<INT0_pin) );
			
			INT_port |= ( 1<< INT0_pin );
			
			break;
		}
		
		case INT1:
		{
			INT_ddr &= ( ~(1<<INT1_pin) );
			
			INT_port |= ( 1<< INT1_pin );
			
			break;
		}
		default:
		{
			#warning: "Int_pin_set() not defined";
			break;
		}
		
	}
	
}



 void Sense_control_set ( uint8_t Int_val, uint8_t sense_bit)
{
	switch (Int_val)
	{
		
		case INT0:
		{
			Int_sense_control_set(sense_bit,INT0_pos);
			break;
		}
		
		case INT1:
		{
			Int_sense_control_set(sense_bit,INT1_pos);
			break;
		}
		
		
		default:
		
		{
			#warning: "Ext_interrupt_set() wrong parameter";
			break;
		}
		
	}
}



#ifndef  __AVR_ATmega8__

 void Ext_asynchronous_int_INT2_enable( uint8_t edge_bit,uint8_t pull_up_val )
 {
	 
	INT2_ddr &= ( ~ (1<<INT2_pin));
	
	
	if( pull_up_val == PULL_UP_ENABLE )
	 
		INT2_port |= ( 1 << INT2_pin );
	
	 	 
	 Ext_asynchronous_int_INT2_disable();
	 
	 if( edge_bit == RISING_EDGE )
	 
		{
			INT2_sense_RISING_EDGE();
		}
		
	else
		
		{
			INT2_sense_FALLING_EDGE();
		}
				
				
		GIFR |= ( 1 << INTF2 ); 
		
		
		INT2_int_enable();
		
 }	
 
 #endif
 
 
 