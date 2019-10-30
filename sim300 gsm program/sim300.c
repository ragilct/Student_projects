

/*
Project : GSM Module Driver
Model	: SIM300 series 
Version : 1.0 
Date   	: 30/07/2012
Author  : Rejith cv
Location: Thiruvananthapuram, India
E-mail  : rejithcvcv@gmail.com


Processor           : AVR
Clock frequency     : 8.000000 MHz 

*/




#include "usart0.c"


#define S300_BUFF_SIZE 300
char sim300_buffer[S300_BUFF_SIZE];


void sim300_filter_strings(char begin, char end, char *str);


/*
#define S300_FILTER_A 		sim300_filter_strings('\n','\r')
#define S300_FILTER_B 		sim300_filter_strings('\n',':')
#define S300_FILTER_C 		sim300_filter_strings('"','"')

*/



//SIM300 SEND COMMANDS
	
const char S300_COMM_MFCR[]  	 					PROGMEM = "ATI\r\n";  				//manufacture   
const char S300_COMM_TEST[]  	 					PROGMEM = "AT\r\n";					//Normal test
const char S300_COMM_ENABLE_INCOMING_NO[]  		 	PROGMEM = "AT+CLIP=1\r\n";  		//enable incoming call

const char S300_COMM_CALL_STATUS[]  	 			PROGMEM = "AT+COLP=1\r\n";  		//call progress status
const char S300_COMM_CALL_STATUS_D[]  	 			PROGMEM = "AT+COLP=0\r\n";  		//call progress status disable

const char S300_COMM_AUD_MAIN[]  	 				PROGMEM = "AT+CHFA=0\r\n";   		//normal audio
const char S300_COMM_AUD_AUX[]  		 			PROGMEM = "AT+CHFA=1\r\n";   		//auxiliary audio
const char S300_COMM_SPK_VOL[]  		 			PROGMEM = "AT+CLVL=";   			//speaker volume
const char S300_COMM_MIC_VOL[]  		 			PROGMEM = "AT+CMIC=";   		 	//mic volume

const char S300_COMM_SMS_TEXT[]  	 				PROGMEM = "AT+CMGF=1\r\n";	 		// sms
const char S300_COMM_SMS_GSM[]  	 				PROGMEM = "AT+CSCS=\"GSM\"\r\n";	// sms
const char S300_COMM_SMS_SENT[]  	 				PROGMEM = "AT+CMGS=";	 			// sms
const char S300_COMM_SMS_DELETE[]  	 				PROGMEM = "AT+CMGD=";	 			// sms


const char S300_COMM_DIAL[]  		 				PROGMEM = "ATD";  					//dial
const char S300_COMM_OFF_HOOK[]  		 			PROGMEM = "ATA\r\n";  				//pick phone
const char S300_COMM_ON_HOOK[]  		 			PROGMEM = "ATH\r\n";  				//terminate connection


const char S300_COMM_SIGNAL_STRENGTH[]  	 		PROGMEM = "AT+CSQ\r\n" ;  	 		//checking range   
const char S300_COMM_GSM_NW[]  	 					PROGMEM = "AT+CREG?\r\n"; 	 		//checking gsm network ok
const char S300_COMM_COM9[]  	 					PROGMEM = "AT+CMGF=1\r\n"; 	 		//reading sms start
const char S300_COMM_DISCONNECT[]  					PROGMEM = "ATH\r\n";	     		//dial disconnect
const char S300_COMM_SIM_CARD[]  					PROGMEM = "AT+CSMINS?\r\n";		 		//check sim card
const char S300_COMM_IMEI[]  	 					PROGMEM = "AT+GSN\r\n";      		//get imei
const char S300_COMM_END[]  	 					PROGMEM = "\r\n";     		 		//end command

const char S300_COMM_SERV_PROVIDE[]  	 			PROGMEM = "AT+CSPN?\r\n";  			//get service provider	 
		








//SIM300 STATUS 

#define S_OKAY				1
#define S_ERROR				2
#define S_INVALID			3
#define S_TIMEOUT			4
#define S_DISCONNECT		5
#define S_RING				6
#define S_BUSY				7
#define S_NOTONE			8
#define S_PICKED			9

#define S_SIM_PRESENT		10
#define S_SIM_ABSENT		11
#define S_SIM_ERROR			12

#define S_REGISTERED		13
#define S_ROAMING			14
#define S_SEARCHING			15

#define S_MSG_EMPTY			16





#define S_AUD_NORMAL		0
#define S_AUD_AUX	     	1





volatile int _simData;




#define SIM_MIN_RANGE   1
#define SIM_MAX_RANGE	31

#define SIM_STEP_RANGE (SIM_MAX_RANGE-SIM_MIN_RANGE)
#define SIM_STEP_RANGE_LEVEL   4
#define SIM_RANGE_LEVEL _simData/(SIM_STEP_RANGE/SIM_STEP_RANGE_LEVEL)


#define SIM300_DELAY  _delay_ms(200)








void clear_sim300_buffer()
{
	int i;
	for(i=0;i<S300_BUFF_SIZE;i++)
		sim300_buffer[i]='\0';
}






void sim300_filter_strings(char begin, char end, char *str)

{

	char *start; 
	char *stop;
	char len;

	clear_sim300_buffer();

	start=strchr((const char *)_usart0Buff,begin);
	//*start='\0';		
		
	stop=strchr(++start,end);
	len=stop-start;
	len++;
	//*stop='\0';
	strlcpy(str,start,len);

}






unsigned char sim300_get_cmd_string(const char *cmd)
{

	int i=0;
	char c;

	clear_sim300_buffer();

	while ( ( c = pgm_read_byte( cmd++ ) ) ) {
		sim300_buffer[i++]=c;
	}
	sim300_buffer[i]='\0';
	return i;

}







unsigned char sim300_cmd(const char *cmd)
{

	
	char len=strlen(cmd);

	usart0_send_string(cmd);

			
	len++;	//Add 1 for trailing CR added to all commands
	
	int i=0;
	
	//Wait for echo
	while(i<10*len)
	{
		if(UDataAvailable()<len)
		{
			i++;
			
			_delay_ms(10);
			
			continue;
		}
		else
		{

			UReadBuffer(sim300_buffer,len);	//Read serial Data
			
			return S_OKAY;
			
		}
	}
	return S_TIMEOUT;

}






unsigned char sim300_cmd_p(const char *cmd)
{
	char ret;
	ret=sim300_get_cmd_string(cmd);
	ret=sim300_cmd(sim300_buffer);

	return ret;

}








unsigned char SIM300CheckResponse(const char *response,const char *check,uint8_t len)
{
	len-=2;
	
	//Check leading CR LF
	if(response[0]!=0x0D || response[1]!=0x0A)
		return	S_INVALID;
	
	//Check trailing CR LF
	if(response[len]!=0x0D || response[len+1]!=0x0A)
		return	S_INVALID;
		
	
	//Compare the response
	for(uint8_t i=2;i<len;i++)
	{
		if(response[i]!=check[i-2])
			return S_ERROR;
	}
	
	return S_OKAY;		
}





unsigned char sim300_wait_response(uint16_t timeout)
{
	uint8_t i=0;
	uint16_t n=0;
	
	while(1)
	{
		while (UDataAvailable()==0 && n<timeout){n++; _delay_ms(1);}
	
		if(n==timeout)
			return 0;
		else
		{
			sim300_buffer[i]=UReadData();
		
			if(sim300_buffer[i]==0x0D && i!=0)
			{
				UFlushBuffer();
				return i+1;
			}				
			else
				i++;
		}
	}	
}











char sim300_status(){


	
	clear_sim300_buffer();
	return 0;

}








void sim300_offhook()
{
	char ret=0;
	char cmd[10];

	strcpy(cmd,"ATA\r\n");
	ret=sim300_cmd(cmd);
}


void sim300_onhook()
{
	char ret=0;
	char cmd[10];

	strcpy(cmd,"ATH\r\n");
	ret=sim300_cmd(cmd);
}




unsigned char sim300_rx_status()
{

	if(!buff0Index)return 0;
	else return 1;
	//clear_usart0_buffer();

}






unsigned char sim300_incoming_no()
{
	char ret=0;
	//ret=sim300_cmd();


	if(ret==S_RING){
		clear_sim300_buffer();
	//	S300_FILTER_C;
		
		clear_usart0_buffer();
	return 1;
	}
	else return 0;
	
}





unsigned char sim300_disconnect()
{

	char ret=0;
	char cmd[10];

	strcpy(cmd,"ATH\r\n");
	ret=sim300_cmd(cmd);
	return ret;

}






unsigned char sim300_dial(char *number)
{

	char ret=0;
	char cmd[20];

	strcpy(cmd,"ATD");
	strcat(cmd,number);
	strcat(cmd,";\r\n");

	ret=sim300_cmd(cmd);
	return ret;
		
}






unsigned char sim300_check_network()
{

	char ret=0;
	char cmd[10];

	strcpy(cmd,"AT+CREG?\r\n");
	ret=sim300_cmd(cmd);

		//Now wait for response
	uint16_t i=0;
	
	while(i<10)
	{
		if(UDataAvailable()<20)
		{
			i++;
			
			_delay_ms(10);
			
			continue;
		}
		else
		{

			UReadBuffer(sim300_buffer,20);	
			
			if(sim300_buffer[11]=='1')
				return S_REGISTERED;
			else if(sim300_buffer[11]=='2')
				return S_SEARCHING;
			else if(sim300_buffer[11]=='5')
				return S_ROAMING;
			else
				return S_ERROR;			
		}
	}
	
	return S_TIMEOUT;

}










unsigned int sim300_signal_strength()
{
	char str[3];

	char ret=0;
	char cmd[10];

	strcpy(cmd,"AT+CSQ\r\n");
	ret=sim300_cmd(cmd);

	uint8_t len=sim300_wait_response(250);
	
	if(len==0)
		return S_TIMEOUT;
		
	sim300_buffer[len-1]='\0';

	str[0]=sim300_buffer[6];
	str[1]=sim300_buffer[7];
	str[2]='\0';

	return atoi (str);
}






unsigned char sim300_imei(char *imei)
{	
	UFlushBuffer();

	char ret=0;
	char cmd[10];

	strcpy(cmd,"AT+GSN\r\n");
	ret=sim300_cmd(cmd);
	
	uint8_t len=sim300_wait_response(1000);
	
	if(len==0)
		return S_TIMEOUT;
		
	sim300_buffer[len-1]='\0';
	
	strcpy(imei,sim300_buffer+2);
	
	return S_OKAY;	

}






//mic volume 0-15db
//spk volume 0-99


unsigned char sim300_audio(char channel, char mic_vol, char spk_vol)
{
	int len;
  	char str[3];
	char ret=0;
	char cmd[10];

	if(channel==S_AUD_NORMAL){
		
		strcpy(cmd,"AT+CHFA=0\r\n");
		ret=sim300_cmd(cmd);

		sprintf(cmd,"AT+CMIC=%d,",channel);
		len=sprintf(str,"%d",mic_vol);
		strcat(cmd,str);
		strcat(cmd,"\r\n");
		ret=sim300_cmd(cmd);

	}


	else if(channel==S_AUD_AUX){

		strcpy(cmd,"AT+CHFA=1\r\n");
		ret=sim300_cmd(cmd);

 		sprintf(cmd,"AT+CMIC=%d,",channel);
		len=sprintf(str,"%d",mic_vol);
		strcat(cmd,str);
		strcat(cmd,"\r\n");
		ret=sim300_cmd(cmd);

	}

		len=sprintf(str,"%d",spk_vol);
		sprintf(cmd,"AT+CLVL=%d,",channel);
		strcat(cmd,str);
		strcat(cmd,"\r\n");
		ret=sim300_cmd(cmd);

	return ret;

		
}







unsigned char sim300_service_provider(char *name)
{

	UFlushBuffer();
	
	char cmd[10];
	char ret;
	
	strcpy(cmd,"AT+CSPN?\r\n");
	ret=sim300_cmd(cmd);

	uint8_t len=sim300_wait_response(1000);
	
	if(len==0)
		return S_TIMEOUT;
	

	sim300_filter_strings('"','"',name);
	
	return strlen(name);

}








unsigned char sim300_sim_status()
{


	UFlushBuffer();
	char cmd[10];
	char ret;
	
	strcpy(cmd,"AT+CSMINS?\r\n");
	ret=sim300_cmd(cmd);	


	//Now wait for response
	uint16_t i=0;
	
	//correct response is 22 byte long
	//So wait until we have got 22 bytes
	//in buffer.
	while(i<30)
	{
		if(UDataAvailable()<22)
		{
			i++;
			
			_delay_ms(10);
			
			continue;
		}
		else
		{
			//We got a response that is 22 bytes long
			//Now check it
			UReadBuffer(sim300_buffer,22);	//Read serial Data
			
			if(sim300_buffer[13]=='1')
				return S_SIM_PRESENT;
			else
				return S_SIM_ABSENT;
		}
	}
	
	return S_TIMEOUT;

}












unsigned char sim300_del_sms(int sms)
{
	UFlushBuffer();
	

	char cmd[10];
	char ret;
	
	sprintf(cmd,"AT+CMGD=%d\r\n",sms);
	ret=sim300_cmd(cmd);


	uint8_t len=sim300_wait_response(1000);
	
	if(len==0)
		return S_TIMEOUT;
	
	sim300_buffer[len-1]='\0';
	
	//Check if the response is OK
	if(strcasecmp(sim300_buffer+2,"OK")==0)
		return S_OKAY;
	else
		return S_ERROR;
}






unsigned char sim300_wait_sms_call()
{	
	uint8_t len=sim300_wait_response(250);
	
	if(len==0)
		return S_TIMEOUT;
	
	sim300_buffer[len-1]='\0';
	
	if(strncasecmp(sim300_buffer+2,"+CMTI",6)==0)//check for sms
	{
		char str_id[4];	
		char *start;	
		start=strchr(sim300_buffer,',');
		start++;
	
		strcpy(str_id,start);
		
		tel.sms_no=atoi(str_id);

		tel.sms=TRUE;
		
		return S_OKAY;
	}		
	else if(strncasecmp(sim300_buffer+2,"+CLIP",6)==0)//Check for call
	{
		sim300_filter_strings('"','"',tel.buffer);
		tel.rx=TRUE;
		return S_OKAY;

	}

	else return S_ERROR;

}










unsigned char sim300_read_sms(uint8_t sms, char *msg, char *oa)
{
	//Clear pending data in queue
	UFlushBuffer();
	
	char cmd[16];
	char ret;

	sprintf(cmd,"AT+CMGR=%d",sms);
	ret=sim300_cmd(cmd);

	
	uint8_t len=sim300_wait_response(1000);
	
	if(len==0)
		return S_TIMEOUT;
	
	sim300_buffer[len-1]='\0';
	
	if(strcasecmp(sim300_buffer+2,"+CMS ERROR: 517")==0)
	{
		//SIM ERROR
		return S_SIM_ERROR;
	}
	
	//MSG Slot Empty
	if(strcasecmp(sim300_buffer+2,"OK")==0)
	{
		return S_MSG_EMPTY;
	}
	
	//Check if OK
	if(strncasecmp(sim300_buffer+2,"+CMGR:",6)==0)
	{
		char *start,*end;
		
		//Find the 3rd "
		start=strchr(sim300_buffer,'"');
		start++;
		
		start=strchr(start,'"');
		start++;
		
		start=strchr(start,'"');
		start++;
		
		end=strchr(start,'"');
		
		*end='\0';
		
		strcpy(oa,start);
	}
		
	//Now read the actual msg text
	len=sim300_wait_response(1000);
	
	if(len==0)
		return S_TIMEOUT;
	
	sim300_buffer[len-1]='\0';
	strcpy(msg,sim300_buffer+1);//+1 for removing trailing LF of prev line
	
	return S_OKAY;
}







unsigned char	sim300_send_sms(const char *num, const char *msg,uint8_t *msg_ref)
{
	UFlushBuffer();
	
	
	char cmd[25];
	char ret;

	sprintf(cmd,"AT+CMGF=1\r\n");
	ret=sim300_cmd(cmd);
	_delay_ms(100);
	cmd[0]='\0';

	sprintf(cmd,"AT+CSCS=\"GSM\"\r\n");
	ret=sim300_cmd(cmd);
	_delay_ms(100);
	cmd[0]='\0';
	sprintf(cmd,"AT+CMGS= %s",num);	
	cmd[8]=0x22; 
	
	uint8_t n=strlen(cmd);
	
	cmd[n]=0x22; 
	cmd[n+1]='\0';
	
	ret=sim300_cmd(cmd);

	
	while(UDataAvailable()<(strlen(msg)+5));
	
	//Remove Echo
	UReadBuffer(sim300_buffer,strlen(msg)+5);
	
	uint8_t len=sim300_wait_response(6000);
	
	if(len==0)
		return S_TIMEOUT;
	
	sim300_buffer[len-1]='\0';
	
	if(strncasecmp(sim300_buffer+2,"CMGS:",5)==0)
	{
		
		*msg_ref=atoi(sim300_buffer+8);
		
		UFlushBuffer();
		
		return S_OKAY;
	}
	else
	{
		UFlushBuffer();
		return S_ERROR;	
	}		
}







/*
Check for any new sms or incoming calls
*/


unsigned char sim300_wait_status()
{
	char ret;

	if(UDataAvailable()==0)
		return FALSE;
	
	ret=sim300_wait_sms_call();
	
	return TRUE;


}




























