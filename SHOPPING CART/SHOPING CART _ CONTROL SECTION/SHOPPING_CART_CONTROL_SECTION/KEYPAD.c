/*
 * KEYPAD.c
 *
 * Created: 31-Jan-15 8:19:58 AM
 *  Author: ADMIN
 */ 


#include "KEYPAD.h"





// KEYPAD FUNCTIONS


uint8_t GetKeyPressed()
{
	uint8_t r,c;
	
	KEYPAD_PORT|= 0X0F;
	
	for(c=0;c<3;c++)
	{
		KEYPAD_DDR&=~(0X7F);
		
		KEYPAD_DDR|=(0X40>>c);
		
		for(r=0;r<4;r++)
		{
			if(!(KEYPAD_PIN & (0X08>>r)))
			{
				return (r*3+c);
			}
		}
	}
	
	return 0XFF;//Indicate No key pressed
	
	
}



char read_keypad_char()
{
	uint8_t val_temp;
	
	char str[2];
	
	
	val_temp = GetKeyPressed();
	
	
	while( val_temp == 0xff )
	
	val_temp = GetKeyPressed();
	
	_delay_ms(250);
	
	switch(val_temp)
	{
		case 9 :	{
			return('*');
			break;
		}
		
		case 10:	{
			return('0');
			break;
		}
		
		case 11:	{
			return('#');
			break;
		}
		
		default:	{
			itoa((val_temp+1),str,10);
			
			return(*str);
		}
		
	}
	
	
}




void read_keypad_str( char *str,uint8_t count )
{
	uint8_t i;
	
	for(i=0;i<count;i++)
	{
		*( str + i ) = read_keypad_char();
		
		// display of each digit entered
		
		//send_data( *( str + i ) );
	}
	
}



uint16_t read_keypad_int(uint8_t digit_no )
{
	char str[digit_no];
	
	read_keypad_str(str,digit_no);
	
	return( atoi(str));
}



