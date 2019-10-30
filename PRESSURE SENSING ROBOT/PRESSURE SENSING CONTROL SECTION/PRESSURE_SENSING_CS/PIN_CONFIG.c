/*
 DRIVER FOR AVR IO OPERATION OF ATMEGA8 MICROCONTROLLER
 
  
 PIN_CONFIG.c
 
 Author			: ARUN V CHIDAMBARAM
 
 Designation	: EMBEDDED SYSTEM DEVELOPER
 
 Email			: arsej007@gmail.com

 Mobile			: 9496115504

 Company		: V-ELECTRO,VELLAYAMBALAM,THIRUVANANTHAPURAM,KERALA

			   

 */ 


#include "PIN_CONFIG.h"



void config_Port_pin( uint8_t port,uint8_t pos,uint8_t io,uint8_t pull_up )   // configures data direction and pull up registers

{	
	
	switch(port)
	
		{
				case PortB :	{													
					
										switch(io)
										
											{
												case OUTPUT :	{	DDRB |= (1<<pos);	break;  }
												
												case INPUT	:	{	DDRB &= ( ~(1<<pos) );	if(pull_up) PORTB |= (1<<pos);	break;	}
												
												default:		{	DDRB = 0X00;	break;	}
											}
											
											break;
													
								}
								
							  
				case PortC	:	{
					
										switch(io)
									
											{
												case OUTPUT :	{	DDRC |= (1<<pos);	break;  }
										
												case INPUT	:	{	DDRC &= ( ~(1<<pos) );	if(pull_up) PORTC |= (1<<pos);	break;	}
										
												default:		{	DDRC = 0X00;	break;	}
											}
					
											break;
					
								}
								
								
				case PortD	:	{
					
										switch(io)
										
											{
												case OUTPUT :	{	DDRD |= (1<<pos);	break;  }
											
												case INPUT	:	{	DDRD &= ( ~(1<<pos) );	if(pull_up) PORTD |= (1<<pos);	break;	}
											
												default:		{	DDRD = 0X00;	break;	}
											}
										
										
											break;
					
								}
								
															
				default:		{
					
										DDRB = 0x00;
										DDRC = 0x00;
										DDRD = 0x00;
					
								}
								
								
								
		}
		
		
		
}




void config_pin_input( uint8_t port,uint8_t pos,uint8_t pull_up) 
{
		uint8_t io = INPUT;
		
		config_Port_pin(port,pos,io,pull_up);
}




void config_pin_output(uint8_t port,uint8_t pos)
{
		uint8_t io = OUTPUT,pull_up = PULL_UP_DISABLE;
	
		config_Port_pin(port,pos,io,pull_up);
	
}




void output_pin( uint8_t port,uint8_t pos,uint8_t state )
{
		
		
		
		switch (port)
		{
			
			case PortB : {  if( ! ( DDRB & (1<<pos) ) )	config_pin_output(port,pos);	state	?	(PORTB |= (1<<pos))	:	(PORTB &= ( ~(1<<pos) ));		break;	}
						  
			case PortC : {  if( ! ( DDRC & (1<<pos) ) )	config_pin_output(port,pos);	state	?	(PORTC |= (1<<pos))	:	(PORTC &= ( ~(1<<pos) ));		break;	}
				
			case PortD : {  if( ! ( DDRD & (1<<pos) ) )	config_pin_output(port,pos);	state	?	(PORTD |= (1<<pos))	:	(PORTD &= ( ~(1<<pos) ));		break;	}
			
			default: { break; }		
					  
		}
	
	
}



uint8_t read_pin (	uint8_t port,uint8_t pos)
{
	uint8_t pullup = PULL_UP_DISABLE;
	
	switch(port)
	{
		
			case PortB:{	PORTB &= (~(1<<pos));	if(  DDRB & (1<<pos) )	config_pin_input(port,pos,pullup);	if(	PINB & (1<<pos)) return(1);else return(0);  break;	}
				
			case PortC:{	PORTC &= (~(1<<pos));	if(  DDRC & (1<<pos) )	config_pin_input(port,pos,pullup);	if(	PINC & (1<<pos)) return(1);else return(0);	break;  }
				
			case PortD:{	PORTD &= (~(1<<pos));	if(  DDRD & (1<<pos) )	config_pin_input(port,pos,pullup);	if(	PIND & (1<<pos)) return(1);else return(0);	break;  }
				
			default:{ return (0);break; }
			
	}	
}


uint8_t read_pin_wp (	uint8_t port,uint8_t pos)
{
	uint8_t pullup = PULL_UP_ENABLE;
	
	switch(port)
	{
		
		case PortB:{	if(  ( DDRB & (1<<pos) ) || ( !(PORTB & (1<<pos) ) ) )	 config_pin_input(port,pos,pullup);	if(	!(PINB & (1<<pos)) ) return(1);else return(0);  break;	}
		
		case PortC:{	if(  ( DDRC & (1<<pos) ) || ( !(PORTC & (1<<pos) ) ) )	 config_pin_input(port,pos,pullup);	if(	!(PINC & (1<<pos)) ) return(1);else return(0);	break;  }
		
		case PortD:{	if(  ( DDRD & (1<<pos) ) || ( !(PORTD & (1<<pos) ) ) )	 config_pin_input(port,pos,pullup);	if(	!(PIND & (1<<pos)) ) return(1);else return(0);	break;  }
		
		default:{ return (0);break; }
		
	}
}





uint8_t  read_pin_group ( int num,...)
{
	
	uint8_t i,port,pos,temp=0,x,j=0,tag=0;
	
	va_list valist;
	
	va_start(valist,num);
	
	i=0;
	
	while(i<num)
		
		{
			j++;
			
			tag = va_arg(valist,int);	//	reads code for pin
			i++;
			
			pos = (tag & 0x0f);			// extracts pin position
			port= (	(tag>>4)& 0x0f);	// extracts port
			
			x = read_pin(port,pos);
			
			if(x)
			
				{
					temp <<=(j-1);
					
					temp |= 0B00000001;
				}
			else
				{
					temp <<=(j-1);
				}
				
				
		}
		
		
		va_end(valist);
		
		return(temp);
		
	
}


uint8_t  read_pin_group_wp ( int num,...)
{
	
	uint8_t i,port,pos,temp=0,x,j=0,tag=0;
	
	va_list valist;
	
	va_start(valist,num);		//	number of parameters set
	
	i=0;
	
	while(i<num)
		
		{
			j++;
			
			tag = va_arg(valist,int);	//	reads code for pin
			i++;
			
			pos = (tag & 0x0f);			// extracts pin position
			port= (	(tag>>4)& 0x0f);	// extracts port
			
			x = read_pin_wp(port,pos);
			
			if(x)
			
				{
										
					temp <<= 1;
										
					temp |= 0B00000001;
				}
			else
				{
										
					temp <<= 1;
				}
				
				
		}
		
		
		va_end(valist);
		
		return(temp);
		
	
}



void output_pin_group(int num,...)
{
	uint8_t i,port,pos,x,j=0,tag=0 ;
	
	int data;
	
	num=num+1;
		
	va_list valist;
	
	va_start(valist,num);
	
	i=0;
	
	data = va_arg(valist,int);    // reads data
	i++;
	
	j=0;
	while(	i<num )
	{
		if( data &(1<<j) )		// checks each bit of data from LSB
		x=1;
		else
		x=0;
		
		j++;
		
		tag = va_arg(valist,int);	//	reads code for pin
		i++;
		
		pos = (tag & 0x0f);			// extracts pin position
		port= (	(tag>>4)& 0x0f);	// extracts port
		
		output_pin(port,pos,x);
		
	}
	
	va_end(valist);


}



