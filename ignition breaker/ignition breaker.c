/*
 * ignition_breaker.c
 *
 * Created: 3/21/2014 3:25:14 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000

	#include <avr/io.h>
    #include <util/delay.h>
	#include <avr/interrupt.h>
	#include <string.h>
	


	#include "lcd.c"


#define RLY_ON (PORTB|=(1<<PB2))
#define RLY_OFF (PORTB&=(~(1<<PB2)))

#define SW (!(PINB&(1<<PB1)))


void system_init();
void USART_Init();
void send_data( unsigned char );
void send_string ( char *);
void gsm_send_msg(char *,char *);
void gsm_read_msg ();
void USART_Flush( void );
void delete_msg();
void mcc_mnc_lac_ci ();



unsigned char cmti,cmgr,creg,ceng;   
int i=0;
char num,rx_msg[200],temp_data[100],msg_data[100],*p,id,j,k,c_flag=0;
char lac[10],ci[10],mcc[20],p_w[10],check='0',act='0',pas_c ='0',loc_id[50];


unsigned char tb1='1',tb2='1';



int main(void)
{
	
	     system_init();
		  sei(); 
		  
		  
		  LCDClear();
		  LCDWriteString("Initializing");
		  for(i=0;i<20;i++){_delay_ms(1000);}
						
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
			
		
				
			
									if ( (strncmp ( msg_data,"ACT",3 ) == 0 )  )
		
										{
			
													strncpy(p_w,msg_data,8);
			
													*(p_w +8) = '\0';
			
																						
													strcpy(msg_data,"dummy");
			
													if ((strncmp ( p_w,"ACT1234",7 ) == 0 ) )
			
														{
													    
																tb1='1';
														
														
																act = '0';
																																																		
																					
														}
								
																					
                                        }
										
										
									else if ( (strncmp ( msg_data,"BRE",3 ) == 0 ) )
									
									{
										
											{
			
														strncpy(p_w,msg_data,8);
			
														*(p_w +8) = '\0';
														
														
																						
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"BRE1234",7 )) == 0  )
			
															{
																
																												    
																	tb1='0';
														
														
																	act = '0';
																	pas_c = '0';
																																			
																					
															}
								
																					
											}
										
																			
										
									}
									
									
									
									else if ( (strncmp ( msg_data,"LOC",3 ) == 0 ) )
									
									{
										
											{
			
														strncpy(p_w,msg_data,8);
			
														*(p_w +8) = '\0';
			
																						
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"LOC1234",7 ) == 0 ) )
			
															{
													    
																	mcc_mnc_lac_ci();
																																
															}
								
																					
											}
										
																			
										
									}
									
									
									
								if ( tb1 == '1'  )
								
								{
									if( act == '0')
									{
										act = '1';
										
										LCDClear();		
									    LCDWriteStringXY(0,0,"ACTIVATED");
									}
									
									
									if(SW)
									{
										RLY_ON;
										
										LCDWriteStringXY(0,1,"IGNITION ON ");
																		
									}
									else
									{
										RLY_OFF;
										LCDWriteStringXY(0,1,"IGNITION OFF");										
									}
									
									
								}		
															
									
								else if (tb1 == '0')
								{
									if( pas_c == '0')
									{
										pas_c = '1';
										
										LCDClear();		
									    LCDWriteStringXY(0,0,"DEACTIVATED");
										
										
										RLY_OFF;
										LCDWriteStringXY(0,1,"IGNITION OFF");
										
									}
									
																									
								}																	
									
								
								
									
									
									
									
									
									
									
	
	
	
         }

}

void system_init()
{
	DDRB |= (1<<PB2);              //  RELAY OUTPUT
	
	DDRB &= ( ~(1<<PB1));          // BUTTON INPUT
	PORTB |= (1<<PB1);
	
	
	DDRB |= (1<<PB0);             // LCD OUTPUT
	DDRD |= ( (1<<PD6) | (1<<PD7) );
	DDRC |= ( (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5) );
	
	 LCDInit(LS_NONE);
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
				
				else if( (strncmp(rx_msg,"+CREG:",6) == 0) )
				{
										
					 creg = '1';
					 
					 p = strchr ( rx_msg,'"');
					 *(p + 12 ) = '\0';
					 strcpy (ci, (p+8) );
					
								
					*(p + 5 ) = '\0';
					strcpy( lac,(p+1));
					
									
					i=0;
						
				}
				
			 else if( (strncmp(rx_msg,"+CENG: 0",8) == 0) ) 
				{
					rx_msg[27]='\0';
					
															
						ceng = '0';
					    p = strchr ( rx_msg,':');
						strcpy(mcc,(p+15) );
					 			/*		 
								 LCDClear();
								 LCDWriteString(temp_data);
								 while(1);
					            */
						//strcat(mcc,"MCC,MNC");	
									
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



void mcc_mnc_lac_ci ()
{
	
	
	USART_Flush();
	
	cli();
	
	send_string("AT+CREG=2");
	send_data('\r');
	send_data('\n');
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	send_string("AT+CREG?");
	send_data('\r');
	send_data('\n');
	
	sei();
	
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);

		
	cli();
	
	send_string("AT+CENG=1");
	send_data('\r');
	send_data('\n');
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	
	ceng = '1';
	send_string("AT+CENG?");
	send_data('\r');
	send_data('\n');
	
	sei();
	
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	
	cli();
	
	strcat(loc_id,"MCC,MNC,LAC,CI : ");
	
	strcat(loc_id,mcc);
	strcat(loc_id,lac);
	strcat(loc_id,",");
	strcat(loc_id,ci);
	
	LCDClear();
	LCDWriteString("SENDING LOC ID");
	gsm_send_msg(loc_id,"9567126559");
	
	if(tb1 == '1')
	{
		act = '0';
	}
	else
	{
		pas_c = '0';
	}
	
	USART_Flush();

	sei();
	
	
	
	
		
}