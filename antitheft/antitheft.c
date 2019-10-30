/*
 * antitheft.c
 *
 * Created: 2/23/2014 3:32:05 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "lcd.c"

 unsigned char cmti,cmgr,creg; 

void delete_msg();
void gsm_read_msg ();
void USART_Init();
void send_data( unsigned char  );
void send_string ( char * );
void gsm_send_msg(char *,char*);
void USART_Flush( void );
void mcc_mnc_lac_ci (void);


int i=0;
char num,rx_msg[100],temp_data[100],msg_data[100],*p,id,j,k,c_flag=0;
char lac[10],ci[10],p_w[10],check='0',act='0',pas_c =0;


int main(void)
{
	cmti = '0';
	cmgr = '0'; 
	creg = '0';
	
	
	 LCDInit(LS_NONE);
	 USART_Init();
	 sei();
	 
	 id='1';
	 delete_msg();
	 
	 LCDWriteString("initializing");
	//for(t=0;t<60;t++)_delay_ms(1000);
	 
		//gsm_send_msg("send message ","12355545");	
		
		LCDClear();
		LCDWriteString("message send");
		
		
		
    while(1)
	{
			
		if( (cmti == '1'))
		{
			gsm_read_msg();
			
			cmti ='0' ;
			
		}
			
			 
		else if(cmgr == '1')
			 
			{
				send_string(msg_data);
				
				send_data(check);
						
				delete_msg();
			
				cmgr = '0';
			
			}
			
		else if (creg == '1')
		
			{
		
		
					send_string("LAC\n");
					send_string(lac);
		
					send_string("CID");
					send_string(ci);
	
					creg = '0';
		
		
			}	
			
			
		///////////////////////////////////////////////////////////////////////////////////////////////	
			
		
			if( act == '0')
			{
					gsm_send_msg("ENTER PASSWORD","9496115504");
					
					act = '1';
					
					pas_c = 0;
			} 	
		
			
				
		
		
	 if ( (strncmp ( msg_data,"PW",2 ) == 0 )  )
		
		{
			
			strncpy(p_w,msg_data,8);
			
			*(p_w +6) = '\0';
			
				LCDClear();
			LCDWriteString(p_w);
			
					if ((strncmp ( p_w,"PW1234",6 ) == 0 ) )
			
					{
						LCDWriteString("Correct");
						act = '0';
						pas_c = 0;
						
					}
			
					else
					{
						LCDWriteString("wrong");
						pas_c++;
				
						if(pas_c > 3)
						{
							gsm_send_msg("SOS","9496115504");
					
							pas_c = 0;
							
							strcpy(msg_data,"garbage");
					
							//act ='0';
							
							
						}
					}
			
			
		}	
		
		
		
		
		
	else if( strncmp ( msg_data,"LOCATION",4 ) == 0 )	
		{
			mcc_mnc_lac_ci();
		}			
			
			
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
	
	
	if ((rx_msg[i-1]== 0x0d )  )
		
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
							strcpy(msg_data,(p+2));
									
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
					LCDClear();
					LCDWriteString(lac);
	
					
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
					
				LCDClear();
				
					
					USART_Flush();
					send_string("AT+CMGD=");
					send_data(id);
					send_data('\r');
					send_data('\n');
					_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
					
					USART_Flush();
					
					LCDWriteString("msg deleted");
	sei();
	
}



void mcc_mnc_lac_ci ()
{
	//char lac[10],ci[10],hex_str[45],*temp_string;
	
	USART_Flush();
	
	cli();
	
	send_string("AT+CREG=2");
	send_data('\r');
	send_data('\n');
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	send_string("AT+CREG?");
	
	sei();
	
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
//	_delay_ms(1000);
//	_delay_ms(1000);
	
	
	
	cli();
	
	send_string("AT+CREG=0");
	send_data('\r');
	send_data('\n');
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	/*
	if(creg == '1')
	{
		
		
		send_string("LAC\n");
		send_string(lac);
		
		send_string("CID");
		send_string(ci);
	
		creg = '0';
		
		
	}
	
	else
	{
		send_string("ERROR");
		
		creg =0;
	}
	*/
	USART_Flush();

	sei();
	
	
	
	
		
}





