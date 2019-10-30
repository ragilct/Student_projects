


#define USART0_BUFF_SIZE 64



//Varriables
volatile char _usart0Buff[USART0_BUFF_SIZE];	//USART Receive Buffer
volatile int8_t UQFront;
volatile int8_t UQEnd;



#define USART0_BAUDRATE 9600
#define UBRR_VALUE0 (((F_CPU / (USART0_BAUDRATE * 16UL))) - 1)




volatile int buff0Index;
volatile char _rx0Flag;




#define CLR_RX0_FLAG 	_rx0Flag=0
#define SET_RX0_FLAG	_rx0Flag=1	

#define RX0_FLAG        (_rx0Flag==1) 




void clear_usart0_buffer(){

	int i;
	_rx0Flag=0;
	for(i=0;i<USART0_BUFF_SIZE;i++)
	{
		_usart0Buff[i]='\0';
	}
	 _rx0Flag=0;
	 buff0Index=0; 

}




void usart0_init(void)
{
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE0>>8);
    UBRR0L = (uint8_t)UBRR_VALUE0;

    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE0>>8);
    UBRR0L = (uint8_t)UBRR_VALUE0;

	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

	sei(); 	
 		
}









//The USART ISR
ISR((USART0_RX_vect))
{
	//Read the data
	char data=UDR0;

	//Now add it to q

	if(((UQEnd==USART0_BUFF_SIZE-1) && UQFront==0) || ((UQEnd+1)==UQFront))
	{
		//Q Full
		UQFront++;
		if(UQFront==USART0_BUFF_SIZE) UQFront=0;
	}
	

	if(UQEnd==USART0_BUFF_SIZE-1)
		UQEnd=0;
	else
		UQEnd++;


	_usart0Buff[UQEnd]=data;

	if(UQFront==-1) UQFront=0;

}







char UReadData()
{
	char data;
	
	//Check if q is empty
	if(UQFront==-1)
		return 0;
	
	data=_usart0Buff[UQFront];
	
	if(UQFront==UQEnd)
	{
	//If single data is left
	//So empty q
	UQFront=UQEnd=-1;
	}
	else
	{
		UQFront++;

		if(UQFront==USART0_BUFF_SIZE)
		UQFront=0;
	}

	return data;
}



void usart0_send_byte(char data)
{
	//Wait For Transmitter to become ready
	while((UCSR0A & (1<<UDRE0)) == 0);

	//Now write
	UDR0=data;
}




uint8_t UDataAvailable()
{
	if(UQFront==-1) return 0;
	if(UQFront<UQEnd)
		return(UQEnd-UQFront+1);
	else if(UQFront>UQEnd)
		return (USART0_BUFF_SIZE-UQFront+UQEnd+1);
	else
		return 1;
}





void usart0_send_string(const char *str)
{
	while((*str)!='\0')
	{
		usart0_send_byte(*str);
		str++;
	}
}


void usart0_send_string_p( const char *prg_s )
{
	char c;

	while ( ( c = pgm_read_byte( prg_s++ ) ) ) {
		usart0_send_byte(c);
	}
}






void UReadBuffer(void *buff,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		((char*)buff)[i]=UReadData();
	}
}






void UFlushBuffer()
{
	while(UDataAvailable()>0)
	{
		UReadData();
	}
}









