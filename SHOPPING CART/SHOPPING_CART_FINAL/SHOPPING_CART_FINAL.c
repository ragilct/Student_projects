/*
 * SHOPPING_CART_FINAL.c
 *
 * Created: 01-Mar-15 3:32:51 PM
 *  Author: ADMIN
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#include "UART.h"
#include "lcd.h"
#include "PIN_CONFIG.h"



////////////// LINE FOLLOWER SECTION ///////////////////////////////////////////////////////

// UNCOMMENT ONLY THE REQUIRED ONE
//#define BLACK_LINE_ON_WHITE_SURFACE
#define WHITE_LINE_ON_BLACK_SURFACE


#define IR_PIN_0	PinC0

#define IR_PIN_1	PinC1

#define IR_PIN_2	PinC2

#define IR_PIN_3	PinC3

////////////////////////////////////////////////////


/// MOTOR CONTROL ////////////////////////////////////////////////////////

#define MOTOR_1_EN	output_pin (PortB,PB0,HIGH)
#define MOTOR_2_EN	output_pin (PortB,PB3,HIGH)


#define MOTOR_1_IN_1		PinB1

#define MOTOR_1_IN_2		PinB2

#define MOTOR_2_IN_1		PinB4

#define MOTOR_2_IN_2		PinB5


#define MOTOR_CTRL(x)		output_pin_group(4,x,MOTOR_1_IN_1,MOTOR_1_IN_2,MOTOR_2_IN_1,MOTOR_2_IN_2)


#define ROBOT_STOP			MOTOR_CTRL(0B0000)

#define ROBOT_FORWARD		MOTOR_CTRL(0B0101)

#define ROBOT_RIGHT		    MOTOR_CTRL(0B1001)

#define ROBOT_LEFT			MOTOR_CTRL(0B0110)

#define ROBOT_BACKWARD		MOTOR_CTRL(0B1010)

//////////////////////////////////////////////////////////////////////

// IR READING

#define IR_READ			read_pin_group_wp(4,IR_PIN_3,IR_PIN_2,IR_PIN_1,IR_PIN_0)

#define ON_TRACK		0B0110
#define RIGHT_DEV		0B0011
#define LEFT_DEV		0B1100
#define RIGHT_MARK		0B0111
#define LEFT_MARK		0B1110
#define RIGHT_SING		0B0001
#define LEFT_SING		0B1000
#define ON_JUNCTION		0B1111
#define OFF_TRACK		0B0000
#define ILLEGAL			99

/////////////////////////////////////////////////////////////////////////


#define RFID_CARD_LEN	6

#define RFID_MODULE_ON		output_pin_group(2,0B01,PinD5,PinD6)

#define ZIGBEE_ON			output_pin_group(2,0B00,PinD5,PinD6);

////////////////////////////////////////////////////////////////////////////

#define TOTAL_CARD_NO	10

#define RF_CARD_10	"CARD0"   // 1

#define RF_CARD_11	"7600193D1F4D"	 // 2
#define RF_CARD_12	"7600193D1C4E"	 // 3
#define RF_CARD_13	"7600193D1B49"	 // 4

#define RF_CARD_20	"7600193D3664"	 // 5
#define RF_CARD_21	"7600193D2173"	 // 6
#define RF_CARD_22	"7600193D2270"	 // 7
#define RF_CARD_23	"7600193D2371"

#define RF_CARD_30	"CARD30"
#define RF_CARD_31	"7600193D2476"
#define RF_CARD_32	"7600193D2577"   // 1
#define RF_CARD_33	"7600193D3567"


////////////////////////////////////////////////////////////////////



#define RF_10		0x10
#define RF_11		0x11
#define RF_12		0x12
#define RF_13		0x13

#define RF_20		0x20
#define RF_21		0x21
#define RF_22		0x22
#define RF_23		0x23

#define RF_30		0x30
#define RF_31		0x31
#define RF_32		0x32
#define RF_33		0x33


#define ID_1	RF_11
#define ID_2	RF_12
#define ID_3	RF_13
#define ID_4	RF_31
#define ID_5	RF_32
#define ID_6	RF_33


#define POS_RAW_EXT(x)		( (x & 0xF0) >> 4 )
#define POS_COL_EXT(x)		( x & 0x0F)


#define DEST_ROBOT_RAW		(  0x0f & ( POS_RAW_EXT(rx_code ) ) )
#define DEST_ROBOT_COL		(  0x0f & ( POS_COL_EXT(rx_code ) ) )

#define CT_ROBOT_RAW		(  0x0f & ( POS_RAW_EXT(rfid_val) ) )
#define CT_ROBOT_COL		(  0x0f & ( POS_COL_EXT(rfid_val) ) )

////////////////////////////////////////////////////////////////////////////////////////////////////

#define TD_F	1
#define TD_B	0
#define TD_N	2

#define BASE	RF_20



uint8_t RFID_read(void);
void Robot_move_rfid_read_right(void);
void Robot_move_rfid_read_left(void);
void Robot_move_rfid_read_return_upto_junction(void);
//void robot_reach_track_2(void);

uint8_t LFS_flag_set_fun(void);
void Robot_move(void);
//void robot_reverse(uint8_t rx_code);

uint8_t Move_robot_forward( void );
uint8_t Move_robot_backward ();

void goto_destination( uint8_t rx_code);




uint16_t prod_price[7] = {10,20,50,15,30,80,0 };
uint8_t prod_count[7] = {0};
volatile uint8_t robot_working_flag = 0;
volatile uint8_t rx_codes_len=0;
int rx_codes[10]={ID_1,ID_2};
int search_str[30];
volatile uint8_t robot_wake_up_flag = 0;
volatile uint8_t rfid_detect_flag = 0;
volatile uint8_t rfid_val=0;
uint8_t robot_dir_flag = 1; // forward
void packet_conter_fun( uint8_t prod_no );
char temp[10];
volatile uint8_t disp_1_flag = 0;

//volatile uint8_t rx_code = 0;


////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
	
	LCDInit (LS_NONE);
	
	USART_Init();
	
    sei();
	
	
		
	MOTOR_1_EN;
	MOTOR_2_EN;
	
	
    while(1)
    {
		
		
				
				
				if (disp_1_flag == 0)
				{
					LCDClear();
					LCDWriteString("waiting for msg ");
					
					disp_1_flag = 1;
				}
	
	
			if (robot_wake_up_flag == 1)
			{
								
						LCDClear();
						LCDWriteString("ROBOT WAKE UP ");
		
						_delay_ms(1000);
		
						//robot_working_flag = 1;
		
						robot_wake_up_flag = 0;
		
						uint8_t i=0;
						
						while(  i < rx_codes_len	 )
						{
								
							
							
					 
						    RFID_MODULE_ON;
					 
							LCDClear();
							LCDWriteStringXY(0,0,"DEST");
							itoa(rx_codes[i],temp,16);
						    LCDWriteStringXY(0,1,temp);
					 
					 
							LCDWriteStringXY(7,0,"POS");
					 
							LCDWriteStringXY(7,1,"00");
					 					 
					 
					 
				
					 
							goto_destination(rx_codes[i]);
					 
					 
							LCDClear();
							LCDWriteString( "Destination ");
					 
			 
							ROBOT_STOP;
							
							///////////////////////////////////////////////////////////////
							
							
							
							
							//////////////////////////////////////////////////////////////
							
							while (LFS_flag_set_fun () != OFF_TRACK )
							{
								ROBOT_FORWARD;
							}
							
							ROBOT_STOP;
							
							_delay_ms (1000);
							
							/*
							
							while (LFS_flag_set_fun () != ON_TRACK )
							{
								ROBOT_LEFT;
								
								if (LFS_flag_set_fun () != OFF_TRACK )
								{
									
									//	Robot_move_rfid_read_return_upto_junction ();
									while( LFS_flag_set_fun () != ON_TRACK )
									Robot_move_rfid_read_left();
									
									ROBOT_STOP;
									
									_delay_ms (2000);
									
									break;
								}
								
								
							}
							*/
							
							
							while(LFS_flag_set_fun () == OFF_TRACK )
							ROBOT_LEFT;
							
							while( LFS_flag_set_fun () != ON_TRACK )
							Robot_move_rfid_read_left();
							
														
							while ( LFS_flag_set_fun () != ON_JUNCTION )
							Robot_move_rfid_read_return_upto_junction();
							
							
							ROBOT_STOP;
							
						 ////////////////////////////////////////////////////////////////////////////////
						 
						 
						  i++;
						  
						  
						/*  if (i<rx_codes_len)
						  {
							  
							  if (  ( POS_RAW_EXT(rx_codes[i]) )   > POS_RAW_EXT( rx_codes[i-1] ) )
							  {
								  
							  }
							  
							  
							  
						  }*/
						 
						 					 
						 _delay_ms (3000);
						 
						 			 					
		
						}
						
						
						
						LCDClear();
						
						LCDWriteStringXY(0,0,"shopping completed");
						
						
						_delay_ms (3000);
		
		
				
		
		}
        
    }
	
	
	
	
}










uint8_t LFS_flag_set_fun(void)
{
	
	uint16_t LFS_val;
	
	LFS_val = read_pin_group_wp(4,IR_PIN_3,IR_PIN_2,IR_PIN_1,IR_PIN_0); // reads pins with pull up
	
	
	
	#ifdef WHITE_LINE_ON_BLACK_SURFACE
	
	LFS_val = ( 0x000F & ( ~ LFS_val));
	
	#endif
	
	
	#ifdef BLACK_LINE_ON_WHITE_SURFACE
	
	LFS_val = ( 0X000F & LFS_val );
	
	#endif
	
	
	
	switch ( LFS_val )
	{
		
		
		case 0B0110: {		//  on track
			return( ON_TRACK);
			break;
		}
		
		
		case 0B0011:{ //  right deviation
			
			return(RIGHT_DEV);
			break;
		}
		
		
		
		case 0B1100: { // left deviation
			
			return (LEFT_DEV);
			break;
		}
		
		
		case 0B0111:{ //  right side mark detected
			
			return (RIGHT_MARK);
			
			break;
		}
		
		case 0B1110:{ // left side mark detected
			
			return( LEFT_MARK);
			
			break;
		}
		
		
		
		case 0B0000:{ // off track
			
			return( OFF_TRACK);
			
			break;
		}
		
		case 0B1111:{
			
			return( ON_JUNCTION );
			
			break;
		}
		
		case 0B0001 : {
			
			return( RIGHT_SING );
			
			break;
		}
		
		
		case 0B1000: {
			
			return ( LEFT_SING );
			
			break;
		}
		
		default:{
			
			return(ILLEGAL);
		}
		
		
	}
	
	
	
	
}






void Robot_move_rfid_read_right(void)
{
	
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD;_delay_ms (100); break; }
		
		case RIGHT_DEV :	{ROBOT_LEFT;_delay_ms (100); break; }
		
		case RIGHT_MARK :	{ROBOT_FORWARD;_delay_ms(100);break;}
		
		case RIGHT_SING :	{ROBOT_LEFT;_delay_ms(100); break; }
		
		case LEFT_DEV :		{ ROBOT_RIGHT;_delay_ms (100); break; }
		
		case LEFT_MARK :	{ROBOT_FORWARD;_delay_ms(100);break;	}
		
		case LEFT_SING :	{ROBOT_RIGHT;_delay_ms(100);break; }
		
		case OFF_TRACK :	{ROBOT_RIGHT;_delay_ms(100);break; }
		
		case ON_JUNCTION :	{ROBOT_RIGHT;_delay_ms (100); break; }
		
		case ILLEGAL :		{ROBOT_FORWARD;_delay_ms (100); break; }
		
		default:			{ROBOT_FORWARD;_delay_ms(100); break; }
		
		
	}

}






void Robot_move_rfid_read_left(void)
{
	
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD;_delay_ms (75); break; }
		
		case RIGHT_DEV :	{ROBOT_LEFT;_delay_ms (75); break; }
		
		case RIGHT_MARK :	{ROBOT_FORWARD;_delay_ms(75);break;}
		
		case RIGHT_SING :	{ROBOT_RIGHT;_delay_ms (20);
			
								if (LFS_flag_set_fun () != RIGHT_SING)
								{
									break;
								}
								
								ROBOT_LEFT;_delay_ms (40); break; }
		
		case LEFT_DEV :		{ ROBOT_RIGHT;_delay_ms (10); break; }
		
		case LEFT_MARK :	{ROBOT_FORWARD;_delay_ms(10);break;	}
		
		case LEFT_SING :	{ROBOT_LEFT;_delay_ms(30);
			
								if (LFS_flag_set_fun () != LEFT_SING)
								{
									break;
								}
								
								
								ROBOT_RIGHT;_delay_ms (40); break; }
		
		case OFF_TRACK :	{ROBOT_LEFT;_delay_ms(10);ROBOT_FORWARD;_delay_ms(10);break; }
		
		case ON_JUNCTION :	{ROBOT_RIGHT;_delay_ms (10); break; }
		
		case ILLEGAL :		{ROBOT_FORWARD;_delay_ms (10); break; }
		
		default:			{ROBOT_FORWARD;_delay_ms(10); break; }
		
		
	}

}






void Robot_move_rfid_read_return_upto_junction(void)
{
	
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD; break; }
		
		case RIGHT_DEV :	{ROBOT_LEFT;break; }
		
		case RIGHT_MARK :	{ROBOT_FORWARD;break;}
		
		case RIGHT_SING :	{ROBOT_RIGHT; break; }
		
		case LEFT_DEV :		{ ROBOT_RIGHT;break; }
		
		case LEFT_MARK :	{ROBOT_FORWARD;break;	}
		
		case LEFT_SING :	{ROBOT_LEFT;break; }
		
		
		case OFF_TRACK :	{
								ROBOT_LEFT;
								
								break;	
														
							}
		
		case ON_JUNCTION :	{
								ROBOT_STOP;
								break;
															
							}
							
		
		
		case ILLEGAL :		{ROBOT_FORWARD;break; }
		
		default:			{ROBOT_FORWARD;break; }
		
		
	}

}


void Robot_move(void)
{
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD ;_delay_ms (75); break; }
		
		case RIGHT_DEV :	{ ROBOT_LEFT;_delay_ms (75); break; }
					
		case RIGHT_SING :	{ ROBOT_RIGHT;_delay_ms (75); break; }
		
		case LEFT_DEV :		{ ROBOT_RIGHT;_delay_ms(75); break; }
			
		case LEFT_SING :	{ ROBOT_LEFT;_delay_ms(75);break; }
		
		case OFF_TRACK :	{ ROBOT_FORWARD;_delay_ms (75); break; }
		
		case ON_JUNCTION :	{ ROBOT_FORWARD;_delay_ms (75); break; }
		
		case ILLEGAL :		{ ROBOT_FORWARD;_delay_ms (75); break; }
		
		default:			{ ROBOT_FORWARD;_delay_ms (75); break; }
		
		
	}
	
	
}




	
	
	
	
ISR(USART_RXC_vect)
{
		
		static char str_buffer[50];
		static uint8_t count;
		
		
		
			
		str_buffer[count] = UDR;
			
		str_buffer[count + 1 ] = '\0';
			
		count++;
		
		
		
			
		if ( (str_buffer[count-1] =='\n') ||  ( count >= 50 ) )
		{
				
				
				
				
				
				if ( strncmp(str_buffer,"ENT",3) == 0 )
				{
					
					
					
					uint8_t i=0;
					
					if (strstr(str_buffer,"1"))
					{
						rx_codes[i] = ID_1;
						
						i++;
						
					}
															
					if (strstr(str_buffer,"2"))
					{
						rx_codes[i] = ID_2;
						i++;
					}
					
					
					if (strstr(str_buffer,"3"))
					{
						rx_codes[i] = ID_3;
						i++;
					}
					
					
					if (strstr(str_buffer,"4"))
					{
						rx_codes[i] = ID_4;
						i++;
					}
					
					
					if (strstr(str_buffer,"5"))
					{
						rx_codes[i] = ID_5;
						i++;
						
					}
					
					
					if (strstr(str_buffer,"6"))
					{
						rx_codes[i] = ID_6;
						i++;
					}
					
					
					rx_codes_len = i;
					
									
					_delay_ms (3000);
					
					robot_wake_up_flag = 1;
					robot_working_flag = 1;
					
					disp_1_flag = 0;
					
				}
				
				
				count = 0;
								
		}
		else 
		{
			
			if ( strstr(str_buffer,RF_CARD_11) )
			{
				rfid_val = RF_11;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_12))
			{
				rfid_val = RF_12;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_13))
			{
				rfid_val = RF_13;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_20))
			{
				rfid_val = RF_20;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_21))
			{
				rfid_val = RF_21;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_22))
			{
				rfid_val = RF_22;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_23))
			{
				rfid_val = RF_23;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_31))
			{
				rfid_val = RF_31;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_32))
			{
				rfid_val = RF_32;
				rfid_detect_flag = 1;
			}
			else if ( strstr(str_buffer,RF_CARD_33))
			{
				rfid_val = RF_33;
				rfid_detect_flag = 1;
			}
			else
			{
				rfid_val = 0;
				
				rfid_detect_flag = 0;
			}
			
			
			
				if (rfid_detect_flag == 1 )
				{
					count = 0;
					
					itoa(rfid_val,temp,16);
					
					LCDWriteStringXY(7,1,temp);
					
					
					
				}
				else
				{
					LCDWriteStringXY(7,1,"00");
				}
			
			
			
			
		}
			
}
		
		
		
			
			
void goto_destination( uint8_t rx_code)
{
	
	if (rfid_val != rx_code )
	{
		
		
		while(CT_ROBOT_COL != DEST_ROBOT_COL)
		{
			
			
			if (CT_ROBOT_COL < DEST_ROBOT_COL )
			{
				Move_robot_forward ();
				
				
			}
			else if ( CT_ROBOT_COL > DEST_ROBOT_COL )
			{
				Move_robot_backward ();
							
			}
			else
			{			
				break;
			}
			
			
		}
		
		
		
		ROBOT_STOP;
		
		
		
		
		
		
		if (CT_ROBOT_RAW < DEST_ROBOT_RAW )
		{
			ROBOT_RIGHT;
			
			_delay_ms(1000);
			
			ROBOT_FORWARD;
			
			_delay_ms (300);
			
			ROBOT_RIGHT;
			
			while( LFS_flag_set_fun() == OFF_TRACK );
			
			Robot_move_rfid_read_right();
			
			
			do
			{
				Robot_move_rfid_read_right();
				
			} //while(DEST_ROBOT_COL != CT_ROBOT_COL );
			
			while(rfid_val != rx_code );
			
			
		}
		else if( CT_ROBOT_RAW > DEST_ROBOT_RAW )
		{
			
			
			ROBOT_LEFT;
			_delay_ms (1000);
			
			ROBOT_FORWARD;
			
			_delay_ms (400);
			
			//  ROBOT_LEFT;
			
			//  _delay_ms (200);
			
			// ROBOT_FORWARD;
			
			// while( LFS_flag_set_fun() == OFF_TRACK );
			
			Robot_move_rfid_read_left();
			
			do
			{
				Robot_move_rfid_read_left();
				
			} //while(DEST_ROBOT_COL != CT_ROBOT_COL );
			
			while(rfid_val != rx_code );
			
		}
		else
		{
			Robot_move_rfid_read_right();
		}
		
		
		
		
		
		do
		{
			Robot_move_rfid_read_right();
			
		} //while(DEST_ROBOT_COL != CT_ROBOT_COL );
		
		while(rfid_val != rx_code );
		
		ROBOT_STOP;
		
	}
	
}	
		
		
		

uint8_t Move_robot_forward( void )
{
	
	if ( robot_dir_flag == TD_B )
	{
		
		robot_dir_flag = TD_N;
		
		Robot_move_rfid_read_right ();
		
		while( robot_dir_flag != TD_N );
		
		if ( robot_dir_flag == TD_B)
		{
			//robot_reverse ();
		}
		
		
	}
	
	return( RFID_read () );
	
	
}







uint8_t Move_robot_backward ()
{
	
	
	
	
	/*
	
	if ( robot_dir_flag == TD_F )
	{
		
		robot_dir_flag = TD_N;
		
		Robot_move ();
		
		while( robot_dir_flag != TD_N );
		
		if ( robot_dir_flag == TD_F)
		{
			robot_reverse ();
		}
		
	}
	
	
	
	return( RFID_read () );
	*/
	
	
	ROBOT_LEFT;
	
	_delay_ms (1000);
	
	ROBOT_FORWARD;
	
	_delay_ms (200);
	
	ROBOT_LEFT;
	
	while(LFS_flag_set_fun () == OFF_TRACK );
	
	
	Robot_move_rfid_read_left ();
	
	return( 0 );
	
	
}	


/*

void robot_reach_track_2(void)
{
	
	RFID_read();
	
	if (CT_ROBOT_RAW != 0x02)
	{
		
		
		while(LFS_flag_set_fun() != OFF_TRACK)
		{
			Robot_move();
			
			if (robot_dir_flag == 0 )
			{
				robot_reverse();
				
				break;
			}
			
			
		}
		
		if (LFS_flag_set_fun() == OFF_TRACK )
		{
			robot_reverse();
		}
		
		
		
	}
	
	
	do
	{
		RFID_read();
		
	} while(CT_ROBOT_RAW != 0x02 );
	
	
	
	
}




void robot_reverse(uint8_t rx_code)
{
	LCDClear();
	LCDWriteString("RETURNS");
	
	while(   (LFS_flag_set_fun() != ON_JUNCTION ))
	ROBOT_FORWARD;
	
	
	ROBOT_RIGHT;
	_delay_ms(500);
	
	while(LFS_flag_set_fun() != ON_TRACK )
	ROBOT_RIGHT;
	
	rx_code = BASE;
	
	
	
	while( CT_ROBOT_COL != DEST_ROBOT_COL )
	{
		RFID_read();
		
		LCDClear();
	}
	
	
	LCDClear();
	
	LCDWriteString("BASE");
	
	ROBOT_LEFT;
	
	_delay_ms(3000);
	
	ROBOT_STOP;
	
	_delay_ms(5000);
	
	
	
}

*/


uint8_t RFID_read()
{
	
	
	while( rfid_detect_flag == 0 )
	{
		
		if(LFS_flag_set_fun() == OFF_TRACK )
		{
			
			Robot_move_rfid_read_right ();
			
		}
		else
		{
			Robot_move_rfid_read_right();
		}
		
		
	}
	
	rfid_detect_flag = 0;
	
	return(rfid_val);
	
	
}

		
		




