

#define F_CPU 6000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define LDR (!(PINC&(1<<PC4)))

#define  PIR1 (!(PINC&(1<<PC0)))
#define  CT1  (!(PINC&(1<<PC1)))
#define  PS1  (!(PINC&(1<<PC2)))
#define RLY1_ON (PORTC|=(1<<PC3))
#define RLY1_OFF (~(PORTC&=(~(1<<PC3))))



#define  PIR2 (!(PINB&(1<<PB1)))
#define  CT2  (!(PINB&(1<<PB2)))
#define  PS2  (!(PINB&(1<<PB3)))
#define RLY2_ON (PORTB|=(1<<PB4))
#define RLY2_OFF (~(PORTB&=(~(1<<PB4))))


#define  PIR3 (!(PIND&(1<<PD5)))
#define  CT3  (!(PIND&(1<<PD6)))
#define  PS3  (!(PIND&(1<<PD7)))
#define RLY3_ON (PORTB|=(1<<PB0))
#define RLY3_OFF (~(PORTB&=(~(1<<PB0))))



void system_init();
void USART_Init();
void send_data( unsigned char );
void send_string ( char * );
void timer0_init();

unsigned int loc1=0,loc2=0,loc3=0,count;
char pf1='0',pf2='0',pf3='0';
int str_i;


int main(void)
{
	
		
	system_init();
	sei();
	
    while(1)
    {
			
		if(!LDR)
			
		{
			
			
			
							if( PIR1 )
							
							{
								loc1 = 0;
								pf1 = '1';
							}
							else
							{								
							   pf1 = '0';
											
							}
							
			
							/*		
							if(PIR2)
							{
								loc2 = 0;
								pf2 = '1';
							}
			
									
							if(PIR3)
							{
								loc3 = 0;
								pf3 = '1';
							}
							
							
							*/
							
							
							if  ( (pf1 == '1') && (loc1 <1000)) 
							{
								RLY1_ON;
																		
							}
							
							else
							{
								pf1 = '0';
								RLY1_OFF;
							}
							
							
							/*
							if( (pf2 == '1') && (loc2 <1000 ) )
							RLY2_ON;
							
							if( (pf3 == '1') && (loc3 <1000 ) )
							RLY3_ON;
							
				*/
									//loc1++;
									
									/*
									if(loc1 > 1000)
									{
										pf1 = '0';
										RLY1_OFF;
									}
									*/
	
	/*
									loc2++;
									if(loc2 > 1000)
									{
										pf2 = '0';
										RLY2_OFF;
									}
	
	
									loc3++;
									if(loc3 > 1000)
									{
										pf3 = '0';
										RLY3_OFF;
									}
							
						*/	
						
							
							/*
							if (  (!PIR2) && (!PIR1) )
							str_i = 0;
							
							if ( (!PIR2) && (PIR1) )
							str_i = 1;
							
							if ( (PIR2) && (!PIR1) )
							str_i = 2;
							
							if ( (PIR2) && (PIR1) )
							str_i = 3;
							*/	
							
							
							
		
		}
		
		else
		{
			
			 pf1='0';
			 pf2='0';
			 pf3='0';
			 
			 loc1=0;
			 loc2=0;
			 loc3=0;
			 
			RLY1_OFF;
			RLY2_OFF;
			RLY3_OFF;
			
		}
		
		
		
		
		
	
		
		
       if(PS1)
	   send_data('1');                          // LIGHT1 ON
	   else
	   send_data('2');							// LIGHT1 OFF
	   
	    
	   if(PS2)
	   send_data('3');							// LIGHT2 ON 
	   else
	   send_data('4');							// LIGHT2 OFF
	   
	  
	   if(PS3)
	   send_data('5');							//LIGHT3 ON
	   else
	   send_data('6');							//LIGHT3 OFF
	   
	 
	   
	   if(CT1)
	   send_data('7');                          // RL1 ON
	   else
	   send_data('8');							// RL1 OFF
	   
	   
	   if(CT2)
	   send_data('9');							// RL2 ON
	   else
	   send_data('a');							// RL2 OFF
	   
	   
	   if(CT3)
	   send_data('b');							// RL3 ON
	   else
	   send_data('c');							// RL3 OFF
	   
	   
	}  
	


}


void system_init()
{
	DDRC &= ( ~( (1<<DDC0) | (1<<DDC1) | (1<<DDC2)  ) );              // POST 1
	PORTC |= ((1<<PC0) | (1<<PC1) | (1<<PC2) );
	DDRC |= ( (1<<DDC3));
	
	DDRB &= ( ~( (1<<DDB1) | (1<<DDB2) | (1<<DDB3)  ) );              // POST2
	PORTB |= ((1<<PB1) | (1<<PB2) | (1<<PB3) );
	DDRB |= ((1<<DDB4));
	
	DDRD &= (~( (1<<DDD5) | (1<<DDD6) | (1<<DDD7)));
	PORTD |= ( (1<<PD5) | (1<<PD6) | (1<<PD7) );                   // POST3
	DDRB |= (1<<DDB0);
	
	DDRC &= (~(1<<DDC4));
	PORTC |= (1<<PC4);
	
	
	timer0_init();
	
	USART_Init();

}	





void USART_Init()
{
			
		UCSRB |= ((1<<RXEN)|(1<<TXEN)|(1<<RXCIE));								   // Enable receiver and transmitter 
		
        UBRRL = 38;
		
		UCSRC |= ((1 <<URSEL) | (1 << UCSZ0 ) | (1 << UCSZ1 ));					   // Use 8- bit character sizes
		
}





void send_data( unsigned char data )
{
	                                                 
while (!( UCSRA & (1 << UDRE )));												   // Wait for empty transmit buffer
                                                      
 UDR = data;																       // Put data into buffer, sends the data 

}

void send_string ( char *str )
{
	while(*str != '\0')
	{
		send_data( *str);
		
		str	++;

	}
	
	//send_data('\0');
}





/////////////////////////////////////////////// TIMER 0 //////////////////////////////////////////////////////////////////

void timer0_init()
{
	TIMSK |= ( 1<<TOV0 );			    										// timer0 overflow interrupt is enabled
	
	TCNT0 = 256-59;															    //  time = (59* ( 1024/(6*10^6))) = 10069  us
		
	TCCR0 |= ( (1<<CS02) | (1<<CS00) ) ;										//     prescalar = 1024
}	



ISR(TIMER0_OVF_vect)
{

	count++;
	
	if( count == 99 )              // 99*10496 ~= 1 sec
	{
		count=0;
		/*
		switch(str_i)
		{
			case 0:
			{
				RLY1_OFF;
				RLY2_OFF;
				break;
			}
			
			case 1:
			{
				RLY1_ON;
				RLY2_OFF;
				break;
			}
			
			case 2:
			{
				RLY1_OFF;
				RLY2_ON;
				break;				
			}
			
			case 3:
			{
				
				RLY1_ON;
				RLY2_ON;
				break;
			}
		}
		*/
		
	}  
	

	TCNT0 = (256-41);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////