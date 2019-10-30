/*
 * anti1.c
 *
 * Created: 2/25/2014 6:23:10 PM
 *  Author: ADMIN
 */ 

#define F_CPU 6000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

//#include "lcd.c"

 unsigned char cmti,cmgr; 

void gsm_read_msg ();
void USART_Init();
void send_data( unsigned char  );
void send_string ( char * );
void gsm_send_msg(char *,char*);
void USART_Flush( void );


int i=0;
char num,rx_msg[50],temp_data[50],msg_data[50],*p,id;


int main(void)
{
	cmti = '0';
	cmgr = '0'; 
	
	 USART_Init();
	 sei();
	 
		gsm_send_msg("hai","9493574647");	
		
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
			
			cmgr = '0';
			}			
			
		//	LCDWriteString(msg_data);
			
			
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
	send_string("AT+CMGF=1");      // text mode
	send_data('\n');
	send_data('\r');
	_delay_ms(1000);
	_delay_ms(1000);
	
	
	
	send_string("AT+CMGS=");      // send
	send_data('"');
	send_string(num);
	send_data('"');
	send_data('\n');
	send_data('\r');
	_delay_ms(1000);
	_delay_ms(1000);
	
	send_string(msg);
	
	send_data(0x1A);
	
	
}



///////////////////////////////////////////////////////////////////////////////////////////////


	

	


void gsm_read_msg ()
{
	cli();
	
	
	send_string("AT+CMGF=1");      // text mode
	send_data('\n');
	send_data('\r');
	_delay_ms(1000);
	_delay_ms(1000);
	
	send_string("AT+CMGR=");
	send_data(id);
	send_data('\n');
	send_data('\r');
	
	sei();
	
	_delay_ms(1000);
	_delay_ms(1000);
	
	
}	




ISR(USART_RXC_vect)

{
	rx_msg[i] = UDR;
	
	rx_msg[i+1] ='\0';
	i++;
	
	if( (strncmp(rx_msg,"+CMTI:",6) == 0) && (rx_msg[i-1]==0x0d) )
	{
		
		strcpy(temp_data,rx_msg);
		p = strrchr(temp_data,',');
		id=*(p+1);
		cmti = '1' ;
		//cmgr = '0';
		i=0;
		
	}	
	
	
	
	else if( (strncmp(rx_msg,"+CMGR:",6) == 0) && (rx_msg[i-1]==0x0d) )
	{
		
		strcpy(temp_data,rx_msg);
		p = strrchr(temp_data,'"');
		strcpy(msg_data,(p+1));
		//cmti = '0';
		cmgr ='1';
		
		i=0;
	}
	
	
	else
	{
		//cmti = '0';
		//cmgr = '0';
	USART_Flush();
		
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
