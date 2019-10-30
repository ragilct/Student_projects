

#define F_CPU 6000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "lcd.c"

#define LCR_ON (PORTB|=(1<<PB1))
#define LCR_OFF (PORTB&=(~(1<<PB1)))

#define HCR_ON   (PORTB|=(1<<PB2))
#define HCR_OFF  (PORTB&=(~(1<<PB2))) 

#define SRR_ON (PORTB|=(1<<PB3))
#define SRR_OFF (PORTB&=(~(1<<PB3))) 


void USART_Init();
void send_data( unsigned char );
void send_string ( char *);
void gsm_send_msg(char *,char *);
void gsm_read_msg ();
void USART_Flush( void );
void delete_msg();
void int0_init();
void int1_init();
unsigned int adc_read(char);
void timer0_init();
int string_to_int(char*);


void system_init()

{
	DDRB |= ( (1<<PB1) |(1<<PB2) | (1<<PB3) ) ;
	USART_Init();
	LCDInit(LS_NONE);
	 int0_init();
	 int1_init();
	
	 timer0_init();
	
	
}





unsigned char cmti,cmgr,creg,ceng;   
int i=0;
char num,rx_msg[200],temp_data[100],msg_data[100],*p,id,j,k,c_flag=0;
char lac[10],ci[10],mcc[20],p_w[10],check='0',act='0',pas_c ='0',loc_id[50];

unsigned int count;
char time_string[10];

 char tb1='0';
 unsigned int l_adc,h_adc,l_rate,h_rate,t_unit,l_unit,h_unit,l_load,h_load,t_load,t_unit_c;
 int ke=0,ht,t_rate ,ls_low=30,ls_high=90;
char hcr_flag = '1',req_flag='0';

char *tpw,*rls,power_flag='1',disp_pow_flag='1',lspw_str[10],tr_str[5],time_high[5],time_low[5] ;
int tr_value=2,th_value,tl_value;

 


int main(void)
{
		  system_init();
		  
		  LCDClear();
		  LCDWriteString("Initializing");
		 for(i=0;i<30;i++){_delay_ms(1000);}
			
						
		  id = '1';	 
		  delete_msg();
		  
		  id='2';
		  delete_msg();
		  
		  SRR_ON;
		  LCR_ON;
		  HCR_ON;
		 
		  LCDClear();
		  
		  t_rate = 2;
		  
		 		  
		  sei();
		
		  
    while(1)
    {
		tb1='0';
							
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
												
												
										
										
										
									 if ( (strncmp ( msg_data,"BRE",3 ) == 0 ) )
									
									{
																					
														strncpy(p_w,msg_data,8);
			
														*(p_w +8) = '\0';
																																													
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"BRE1234",7 )) == 0  )
			
														{											
																SRR_OFF;	
																
																power_flag = '0';																
																					
														}
								
															
																								
										
									}
									
									else if ( (strncmp ( msg_data,"ACT",3 ) == 0 ) )
									
									{
																					
														strncpy(p_w,msg_data,8);
			
														*(p_w +8) = '\0';
																																													
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"ACT1234",7 )) == 0  )
			
														{											
																SRR_ON;																	
																			
																power_flag='1';		
														}
								
															
																								
										
									}
									else if ( (strncmp ( msg_data,"RLS",3 ) == 0 ) )
									
									{
																					
														strncpy(p_w,msg_data,16);
			
														*(p_w +15) = '\0';
																																													
														strcpy(msg_data,"dummy");
			
														if ((strncmp ( p_w,"RLS1234",7 )) == 0  )
			
														{		
															cli();									
																tpw=strrchr(p_w,':');
																
																strcpy(lspw_str,(tpw+1));
																
																
																
																tpw = strrchr(lspw_str,',');
																
																strcpy(tr_str,(tpw+1));
																
																*tpw ='\0';
																
																tr_value=atoi(tr_str);
																
																
																
																tpw = strchr(lspw_str,',');
																
																strcpy(time_high,(tpw+1));
																
																*tpw='\0';
																
																ls_high=atoi(time_high);
																
																																
																strcpy(time_low,lspw_str);
																
																ls_low=atoi(time_low);
																
																sei();
																
																
																
																
																							
														}
								
															
																								
										
									}
									
									
	   
	                                    
									
									/////////////////////////////////////////////////////
									
									
									
									
															
						 
			}	
							
												
          

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
				
			else
				{
					i=0;	
					c_flag = 0;	
					USART_Flush();
	
				}
				
				
				///////////////////////////////////////////////////////////
				
				
	
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void int0_init ()
{
	DDRD   &=  (~( 1<<DDD2 ));
	
	PORTD  |=  ( 1 <<PD2 );
	
	GICR &= (~(1<<INT0));
	
	GIFR &= (~(1<<INTF0));
		
	MCUCR  |=  ( (1<<ISC01) );
	
	GICR   |=  ( 1<<INT0 );
}

ISR(INT0_vect)
{
	
	
	if((ke > ls_low) && (ke < ls_high)&& (req_flag == '0'))
	{
		GICR &= (~(1<<INT1));
		TCCR0 &= (~((1<<CS02) | (1<<CS00)));
		LCDClear();
		LCDWriteString("SENDING REQUEST");
		gsm_send_msg("HCR REQ 1234","7736245395");
		cli();
		LCDClear();
		LCDWriteString("WAITING ");
		
		while(tb1=='0')
		{
			
			sei();
			 if( (cmti == '1'))
									  
				{
					gsm_read_msg();
						cli();
					cmti ='0' ;
			
				}
			else if(cmgr == '1')
			 
				{
												
					delete_msg();
					cmgr = '0';
			
				}
				
				if ( (strncmp ( msg_data,"HCRON",5 ) == 0 )  )
		
										{
			
												strncpy(p_w,msg_data,10);
												*(p_w +10) = '\0';
																						
												strcpy(msg_data,"dummy");
			
												if ((strncmp ( p_w,"HCRON1234",9 ) == 0 ) )
			
												{
													    
													HCR_ON;
													tb1='1';
													req_flag = '1';
													TCCR0 |= ( (1<<CS02) | (1<<CS00) ) ;
													GICR   |=  ( 1<<INT1 );
																											
												}
								
																					
                                        }
				
				
		}
		cli();
		LCDClear();
		disp_pow_flag='1';
	}
	
	
	
	
	
	
}


////////////////////////////////////////////////////////////////////////

void int1_init ()
{
	DDRD   &=  (~( 1<<DDD3 ));
	
	PORTD  |=  ( 1 <<PD3 );
	
	GICR &= (~(1<<INT1));
	
	GIFR &= (~(1<<INTF1));
	
	MCUCR  |=  ( (1<<ISC11));
	
	GICR   |=  ( 1<<INT1 );

	
}

ISR(INT1_vect)
{
	LCDClear();
	
	LCDWriteStringXY(0,0,"UNIT = ");
	
	itoa(t_unit,time_string,10);
	
	LCDWriteStringXY(8,0,time_string);
	
	
	
	LCDWriteStringXY(0,1,"RS ");
	
	if((ke > ls_low) && (ke < ls_high))
	{
		itoa(tr_value,time_string,10);
		LCDWriteStringXY(3,1,time_string);
	}
	else
	{
		itoa(t_rate,time_string,10);
		LCDWriteStringXY(3,1,time_string);
	}
	
	
	
	_delay_ms(2000);
	
	disp_pow_flag='1';
	
	LCDClear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////




unsigned int adc_read(char nadc)
{
	unsigned int a,b;
	
	ADMUX = 0x00;     // initializing ADMUX and ADCSRA
	
	ADCSRA = 0x00;
	
	ADMUX = nadc;         // Selection of ADC channel 
	
	ADMUX |= ( (1<<REFS0) | (1<<REFS1));
	
	ADCSRA |= ( (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) ); // Enabling ADC
	
	ADCSRA |= (1<<ADSC);  //Start conversion
	
	while (  !(ADCSRA & (1<<ADIF))  ); // wait for conversion complete ADIF= 0--->1
	
	a = ADCL;  
	
	b = ADCH ;
	
	ADCSRA |= (1<<ADSC);  //Start conversion
	
	while (  !(ADCSRA & (1<<ADIF))  ); // wait for conversion complete ADIF= 0--->1
	
	a = ADCL;  
	
	b = ADCH ;
	
	ADCSRA &= (~( 1<<ADEN ));   // ADC off to reduce power consumption
	
	return ( a += (256*b) );
		
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void timer0_init()
{
	TIMSK |= ( 1<<TOV0 );			    										// timer0 overflow interrupt is enabled
	
	TCNT0 = 256-59;															    //  time = (59* ( 1024/(6*10^6))) = 10069  us
		
	TCCR0 |= ( (1<<CS02) | (1<<CS00) ) ;										//     prescalar = 1024
}	



ISR(TIMER0_OVF_vect)
{

	count++;
	
	if( count == (99) )              // 99*10496 ~= 1 sec
	{
		count=0;
		/////////////////////////////////////////////////////////////////
		
				if( power_flag == '1')
				
				{
					
				
		
										l_adc = adc_read(1);
										h_adc = adc_read(0);
										
										/*
										LCDClear();
										LCDWriteInt(l_adc,4);
										LCDWriteInt(h_adc,4);
										//while(1);
										_delay_ms(2000);
										_delay_ms(2000);
										*/	
										if( (140 < l_adc) && (l_adc <= 220))
										l_load =  3;
		
										else if( (100 < l_adc) && (l_adc <= 140 ))
										l_load =  2;
		
										else if ( (20 < l_adc) && (l_adc <= 100 ))
										l_load =  1;
										
										else
										l_load = 0;
		
		
			
										if( (155 < h_adc) && (h_adc <= 200))
										h_load =  3;
		
										else if(  (110 < h_adc) && (h_adc <= 155 ) )
										h_load =  2;
		
										else if ((25 < h_adc) && (h_adc <= 110 ) )
										h_load =  1;
										
										else
										h_load=0;
																						
										t_load = l_load + (h_load * 2);		
										
										LCDWriteStringXY(0,0,"LC ON  ");
										
										if(hcr_flag == '1')
										{
											LCDWriteStringXY(7,0,"HC ON ");
										}
										
										else
										{
											LCDWriteStringXY(7,0,"HC OFF");
										}
										
										LCDWriteStringXY(0,1,"LS ");
										itoa(ls_low,time_string,10);
										LCDWriteStringXY(3,1,time_string);
										LCDWriteStringXY(5,1,",");
										itoa(ls_high,time_string,10);
										LCDWriteStringXY(6,1,time_string);
																													
										
										itoa(ke,time_string,10);
										LCDWriteStringXY(13,1,time_string);
		
										ke++;
										
										if( (ke > ls_low) && (ke < ls_high)&& (req_flag == '0'))
										{
											HCR_OFF;
											hcr_flag='0';
										}
										
										else
										{
											HCR_ON;
											hcr_flag='1';
											if(ke > ls_high)
											req_flag='0';
										}
										
										
										if( ke >= 100 )
										{
											ke=0;
											ht = 0;
											t_load = 0;
											t_unit = 0;
											l_rate=0;
											h_rate=0;
											t_unit_c = 0;
											
											LCDClear();
										}
		/////////////////////////////////////////////////////////////////
											ht++;
	
											if( ht <= 10 )
											{
												t_unit_c = t_load + t_unit_c;
											}
		
											else
											{
												ht = 0;
			
												t_unit = (t_unit_c/10);
			
											}
											
											disp_pow_flag='1';
											
											
				}
				else
				{
					if(disp_pow_flag == '1')
					{
						LCDClear();
						LCDWriteString("POWER OFF");
					}
					
					disp_pow_flag ='0';
					
				}				
	
		
		}  
	

	TCNT0 = (256-41);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

