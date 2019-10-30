/*
 * password_based_circuit_breaker.c
 *
 * Created: 3/9/2014 5:16:54 PM
 *  Author: ADMIN
 */

	#define F_CPU 6000000

	#include <avr/io.h>
    #include <util/delay.h>
	#include <avr/interrupt.h>
	#include <string.h>

	#include "lcd.c"
	
		
	
void delete_msg();
void gsm_read_msg ();
void USART_Init();
void send_data( unsigned char  );
void send_string ( char * );
void gsm_send_msg(char *,char*);
void USART_Flush( void );
  
 
 unsigned char Read_key(void); 
 void key_init(void);
 void flush_kp(void);
 void lcd_disp(void);
 
 void relay_init(void);
 
 #define B1_ON (PORTD|=(1<<PD2))
 #define B1_OFF (PORTD&=(~(1<<PD2)))
 
 #define B2_ON (PORTD|=(1<<PD3))
 #define B2_OFF (PORTD&=(~(1<<PD3))) 
 
 #define B3_ON (PORTD|=(1<<PD4))
 #define B3_OFF (PORTD&=(~(1<<PD4)))
 
    
   char flag=0,p_flag = '0';
   char kp[10];
   char password[10] = "#1234";
   int pk = 0;
   char tb1='0',tb2='0',tb3='0';
   
 
 
unsigned char cmti,cmgr,creg;   
int i=0;
char num,rx_msg[100],temp_data[100],msg_data[100],*p,id,j,k,c_flag=0;
char lac[10],ci[10],p_w[10],check='0',act='0',pas_c =0;
   
   int main(void)
{
          uint8_t key;
		  LCDInit(LS_NONE);
		  key_init();
		  USART_Init();
		  relay_init();
		  sei(); 
		  LCDClear();
		  LCDWriteString("Initializing");
		  
	   	  for(i=0;i<20;i++){_delay_ms(1000);}
				 
							
			id = '1';	 
			delete_msg();
		
			LCDClear();
		  
		  LCDWriteString("ENTER PASSWORD");

		 
				
		  pk=0;
			
		  while(1)
		  
		  {
			 
					  key=Read_key(); 
					  if(key != 'f')	
					  {
						 
								  if(key == '#')
								  {
									   p_flag = '1';
									   LCDClear();
									   pk=0;
								  }	
				  
				  
								  if( p_flag == '1')
								  
								  {
									   LCDData(key);
									   kp[pk] = key;
									   pk++;	
				 			
					 
										 if( ( kp[pk] == '*')|| (pk == 5) )
										 
										 {
													 kp[5] = '\0';
									 
													 if( (strncmp(kp,"#1111",4)) == 0 )
													 {
														 															
														if( tb1 == '1')
														tb1='0';
														else
														tb1='1';
														LCDClear();
														LCDWriteString("ENTER PASSWORD");
														lcd_disp();								
														
														
													 }
									 
													else if( (strncmp(kp,"#2222",4)) == 0 )
													 {
														 
														if( tb2 == '1')
														tb2='0';
														else
														tb2='1';
														LCDClear();
														LCDWriteString("ENTER PASSWORD");
														lcd_disp();											
																 
													 }
													else if( (strncmp(kp,"#3333",4)) == 0 )	
													 {
														if( tb3 == '1')
														tb3='0';
														else
														tb3='1';
														LCDClear();
														LCDWriteString("ENTER PASSWORD");
														lcd_disp();			
														 
													 }
													else
													{
														LCDClear();
														LCDWriteString("WRONG");
														_delay_ms(500);
														LCDWriteString("ENTER PASSWORD");
														lcd_disp();
														
													}																												 
											 
												        p_flag = '0';
													    pk=0;
														
										 }	
								 }									 
				 
				}	
					 
				else
							
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
			
		
				
			
									if ( (strncmp ( msg_data,"PW",2 ) == 0 )  )
		
										{
			
											strncpy(p_w,msg_data,8);
			
											*(p_w +6) = '\0';
			
																						
											strcpy(msg_data,"dummy");
			
									if ((strncmp ( p_w,"PW1111",6 ) == 0 ) )
			
												{
													    
														if( tb1 == '1')
														tb1='0';
														else
														tb1='1';
														
													    act = '0';
													    pas_c = 0;
																									
						
												}
			
									else if ((strncmp ( p_w,"PW2222",6 ) == 0 ) )
												{
													    if( tb2 == '1')
														tb2='0';
														else
														tb2='1';
														
													    act = '0';
													    pas_c = 0;
														
														
														
														
													
												}
												
									 else if ((strncmp ( p_w,"PW3333",6 ) == 0 ) )
												{
													    if( tb3 == '1')
													    tb3='0';
													    else
													    tb3='1';
														
													    act = '0';
													    pas_c = 0;
														
														
													
												}
												
											/*	
											else
												{
													LCDClear();
													LCDWriteString("wrong");
													_delay_ms(2000);
													
													LCDClear();
													LCDWriteString("ENTER PASSWORD");
													lcd_disp();
												}
			
												*/
												
										
													LCDClear();
													LCDWriteString("ENTER PASSWORD");
													lcd_disp();		
							    
								
								 }	
		
							
													
								
					}								 
							
		    }	   
	   
    
}





void key_init(void)
{
	
	
  DDRB |= ( (1<<PB1) | (1<<PB2) | (1<<PB3) );

  
  DDRB &= ( ~( (1<<PB4) | (1<<PB5) ));
  PORTB |= ((1<<PB4) | (1<<PB5));
  
  DDRC &= (~ ((1<<PC0) | (1<<PC1)));
  PORTC |= ((1<<PC0) | (1<<PC1));
	
}


void relay_init(void)
{
	DDRD |= ( (1<<PD2) | (1<<PD3) | (1<<PD4) );
}




unsigned char Read_key(void)
{  
   unsigned char value;		
  
   flag = 0;
   value = 'f';
  
   PORTB |= ( (1<<PB1) | (1<<PB2) | (1<<PB3) );
   PORTB &= (~(1<<PB1));
   _delay_ms(50);
   if(!(PINB & (1<<PB4)))
   {
	    value ='1';
	    while (!(PINB & (1<<PB4)));
	    flag = 1;
	//	_delay_ms(150);
   }
  
   
   else if(!(PINB & (1<<PB5))) 
   {
	   	value ='4';
		while (!(PINB & (1<<PB5)));
		flag=1;
	//	_delay_ms(150);
   }
   

   
   else if(!(PINC & (1<<PC0)))
   {
	    value ='7';
	    while (!(PINC & (1<<PC0)));
	    flag=1;
	//	_delay_ms(150);
   }
  
   
   else if ((!(PINC & (1<<PC1))))
   {
	    value ='*';
	    while (!(PINC & (1<<PC1)));
		flag=1;
	//	_delay_ms(150);
   }
  
   
     if(value != 'f')
	 {
		  return (value);
		  //_delay_ms(300);
	 }
	
	
     flag =0 ;
  
     value = 'f'; 
     PORTB |= ( (1<<PB1) | (1<<PB2) | (1<<PB3) );
     PORTB &= (~(1<<PB2));
	 _delay_ms(50);
	 
    if(!(PINB & (1<<PB4)))
   {
	    value ='2';
	    while (!(PINB & (1<<PB4)));
	    flag=1;
	//	_delay_ms(150);
   }
   
     else if(!(PINB & (1<<PB5))) 
   {
	   	value ='5';
		while (!(PINB & (1<<PB5)));
		flag=1;
	//	_delay_ms(150);
   }
   
   else if(!(PINC & (1<<PC0)))
   {
	    value ='8';
	    while (!(PINC & (1<<PC0)));
	    flag=1;
	//	_delay_ms(150);
   }
  
   
   else if ((!(PINC & (1<<PC1))))
   {
	    value ='0';
	    while (!(PINC & (1<<PC1)));
		flag=1;
	//	_delay_ms(150);
   }
  
   
	  if(value != 'f')
	 {
		  return (value);
		 // _delay_ms(300);
	 }
	  
	   flag = 0;
	   value ='f';
   
  
   PORTB |= ( (1<<PB1) | (1<<PB2) | (1<<PB3) );
   PORTB &= (~(1<<PB3));
    _delay_ms(50);
   if(!(PINB & (1<<PB4)))
   {
	    value ='3';
	    while (!(PINB & (1<<PB4)));
	    flag=1;
	//	_delay_ms(150);
   }
	
     else if(!(PINB & (1<<PB5))) 
   {
	   	value ='6';
		while (!(PINB & (1<<PB5)));
		flag=1;
	//	_delay_ms(150);
   }
   
   else if(!(PINC & (1<<PC0)))
   {
	   value ='9';
	   while (!(PINC & (1<<PC0)));
	   flag=1;
	//   _delay_ms(150);
   }
  
   
   else if ((!(PINC & (1<<PC1))))
   {
	     value ='#';
	     while (!(PINC & (1<<PC1)));
		 flag=1;
	//	 _delay_ms(150);
   }
  
   
	else
	
	    value = 'f';
	
  		flag = 0;
	
	  // _delay_ms(300);
	    return(value);
	   
  
  
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
					
				
				
					
					USART_Flush();
					send_string("AT+CMGD=");
					send_data(id);
					send_data('\r');
					send_data('\n');
					_delay_ms(1000);
					_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
				//	_delay_ms(1000);
					
					USART_Flush();
					
					
	sei();
	
}


void lcd_disp(void)
{
	
	
	 if(tb1 == '1')
	{
	  B1_ON;
	  LCDWriteStringXY(0,1,"B1");
    }
														
	else
   {
	 B1_OFF;
	 LCDWriteStringXY(0,1," ");
   }
   
   
   
    if(tb2 == '1')
	{
	  B2_ON;
	  LCDWriteStringXY(5,1,"B2");	 
	}
												
	else
	{
	  B2_OFF;  
	  LCDWriteStringXY(5,1," ");
	}
   
   
     if(tb3 == '1')
	{
	  B3_ON;
	  LCDWriteStringXY(10,1,"B3");
	}	  
	 else
	 {
		B3_OFF;
		LCDWriteStringXY(10,1," ");
	 }	
	
}

