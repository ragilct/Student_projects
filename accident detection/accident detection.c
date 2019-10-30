#define F_CPU 6000000

	#include <avr/io.h>
    #include <util/delay.h>
	#include <avr/interrupt.h>
	#include <string.h>

	

#define  RLY_ON (PORTD|=(1<<PD7))        //RLY
#define  RLY_OFF (PORTD&=(~(1<<PD7)))


#define VBS1 (!(PINC&(1<<PC0)))   //VBS
#define VBS2 (!(PINC&(1<<PC1)))   


#define BUZZER_ON (PORTB|=(1<<PB0)) //BUZZER
#define BUZZER_OFF (PORTB&=(~(1<<PB0)))




void system_init();
void USART_Init();
void send_data( unsigned char );
void send_string ( char *);
void gsm_send_msg(char *,char *);
void gsm_read_msg ();
void USART_Flush( void );
void delete_msg();
void timer0_init();
void accident_info();
void int0_init();



unsigned char cmti,cmgr,creg,ceng;   
int i=0;
char num,rx_msg[200],temp_data[100],msg_data[100],*p,id,j,k,c_flag=0;
char lac[50]="EMERGENCY",p_w[10],check='0',act='0',pas_c ='0',loc_id[50];


unsigned char tb1='1',tb2='1';
unsigned int count;



int main(void)
{
	
	     system_init();
		 
		 for(i=0;i<40;i++)
		 {
			BUZZER_OFF;
		    _delay_ms(950);
		    BUZZER_ON;
		    _delay_ms(50);
		 }
		 
		 _delay_ms(500);
		 
		 BUZZER_OFF;
		  sei();		
						
		 id = '1';	 
		 delete_msg();
		 
		 
		  
    while(1)
    {
		
       
	   
	                                     if( (cmti == '1'))
									  
											{
												gsm_read_msg();
			
												cmti ='0' ;
			
											}
			
			 
									    else if(cmgr == '1')
			 
												{
													
													delete_msg();
			
													cmgr = '0';
			
												}
												
												
												
												
									 if ( (strncmp ( msg_data,"LOC",3 ) == 0 ) )
									
									{
										
											{
			
														strncpy(p_w,msg_data,8);
			
														*(p_w +8) = '\0';
			
																						
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"LOC1234",7 ) == 0 ) )
			
															{
													    
															   accident_info();
																																
															}
																													
											}
										
												
									}
				
			
										
									if( (VBS1 || VBS2) && (pas_c=='0'))
									{
										pas_c = '1';
										timer0_init();
										
										BUZZER_ON;
										
									}
																			
									
	   }

}

void system_init()
{
	DDRC &= (~( (1<<PC0) | (1<<PC1) ));    // VBS
	PORTC |= ( (1<<PC0) | (1<<PC1) );
	
	DDRD |= (1<<PD7);   //RLY
	
	DDRB |= ( 1<<PB0);    // EB
	
	 int0_init();
	 USART_Init();
	 TCCR0 &= (~( (1<<CS02) | (1<<CS00) )) ;
	 
	 BUZZER_OFF;
	 
	
	
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

///////////////////////////////////////////////////////////////////////////////////////////




void gsm_send_msg(char *msg,char *num)
{
	cli();
	
	USART_Flush();
	send_string("AT+CMGF=1");      // text mode
	send_data('\r');
	send_data('\n');
	_delay_ms(1000);
	_delay_ms(1000);
	
	
	USART_Flush();
	send_string("AT+CMGS=");      // send
	send_data('"');
	send_string(num);
	send_data('"');
	send_data('\r');
	send_data('\n');
	_delay_ms(1000);
	_delay_ms(1000);
	
	USART_Flush();
	send_string(msg);
	
	_delay_ms(1000);
	_delay_ms(1000);
	send_data(0x1A);
	
	_delay_ms(1000);
	
	
	sei();
	
	USART_Flush();
	
}



///////////////////////////////////////////////////////////////////////////////////////////////


	

void gsm_read_msg ()
{
	cli();
	
	USART_Flush();
	send_string("AT+CMGF=1");      // text mode
	send_data('\r');
	send_data('\n');
	_delay_ms(1000);
	_delay_ms(1000);
	
	USART_Flush();
	send_string("AT+CMGR=");
	send_data(id);
	send_data('\r');
	send_data('\n');
	
	USART_Flush();
	sei();
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	
}	




ISR(USART_RXC_vect)

{

	rx_msg[i] = UDR;
	
	rx_msg[i+1] ='\0';
	
	i++;
	
	
	if ((rx_msg[i-1]== '\n' )  )                                //  /n is used
		
		{
			
		
				if( ( strncmp(rx_msg,"+CMTI:",6) == 0)  )
				{
					
					strcpy(temp_data,rx_msg);
					p = strrchr(temp_data,',');
					id=*(p+1);
					cmti = '1' ;
					i=0;
									
					c_flag = 0;
				}	
	
	
	
				else if( (strncmp(rx_msg,"+CMGR:",6) == 0) )
				{
					    c_flag++;
						if (c_flag == 2)
						{
													
							strcpy(temp_data,rx_msg);
							p = strrchr(temp_data,'"');
							strcpy(msg_data,(p+3));
							
							
							cmgr ='1';
							
							i=0;
							
							c_flag = 0;
							
							check = '1';
						}
						
																
				}
				
				else if( (strncmp(rx_msg,"$GPRMC",6) == 0) )
				{
										
					 creg = '1';
					 
					 p = strchr ( rx_msg,',');
					 p= strchr ((p+1),',');
					 *(p + 27 ) = '\0';
					 strcpy (loc_id, (p+1) );
					
					 i=0;
						
				}
				
				
				else
				{
					i=0;	
					c_flag = 0;	
					USART_Flush();
	
				}
	
		}
	
	}
	
		
	
	

void USART_Flush( void )
{
unsigned char dummy;

while ( UCSRA & (1<<RXC) ) 
{
	dummy = UDR;
}

}



void delete_msg()
{
	cli();
					
				
					USART_Flush();
					send_string("AT+CMGD=");
					send_data(id);
					send_data('\r');
					send_data('\n');
					_delay_ms(1000);
					_delay_ms(1000);
				
					
					USART_Flush();
					
					
	sei();
	
}


void accident_info()
{
	TIMSK &= (~(1<<TOV0));
	
	sei();
	RLY_ON;
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	
	RLY_OFF;
	
	
	
	strcat(lac,loc_id);
	gsm_send_msg(lac,"9633768228");
	gsm_send_msg(lac,"8714301930");
	gsm_send_msg(lac,"9995473067");
	
	
	TIMSK |= ( 1<<TOV0 );	
	
		
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
	
	if( count == (99*20) )              // 99*10496 ~= 1 sec
	{
		count=0;
		
		 accident_info();
		
	}  
	
	TCNT0 = (256-41);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void int0_init ()
{
	MCUCR  |=  ( 1<<ISC01 );
	
	GICR   |=  ( 1<<INT0	);

	DDRD   &=  (~( 1<<DDD2 ));
	
	PORTD  |=  ( 1 <<PD2 );
}

ISR(INT0_vect)
{
	BUZZER_ON;
	
	timer0_init(); 
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////