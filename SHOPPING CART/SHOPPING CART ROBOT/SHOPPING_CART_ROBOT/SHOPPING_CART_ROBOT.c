/*
 * SHOPPING_CART_ROBOT.c
 *
 * Created: 02-Feb-15 12:52:55 PM
 *  Author: ADMIN
 */ 


#define F_CPU	6000000

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
#define BLACK_LINE_ON_WHITE_SURFACE
//#define WHITE_LINE_ON_BLACK_SURFACE


#define IR_PIN_0	PinC0

#define IR_PIN_1	PinC1

#define IR_PIN_2	PinC2

#define IR_PIN_3	PinC3

////////////////////////////////////////////////////


/// MOTOR CONTROL ////////////////////////////////////////////////////////


#define MOTOR_1_IN_1		PinB0

#define MOTOR_1_IN_2		PinB1

#define MOTOR_2_IN_1		PinB2

#define MOTOR_2_IN_2		PinB3


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

#define RFID_MODULE_ON		output_pin(PortB,4,HIGH)
#define RFID_MODULE_OFF		output_pin(PortB,4,LOW)


#define TD_F	1
#define TD_B	2
#define RD_F	3
#define RD_B	4

#define TOTAL_CARD_NO	10

#define RF_CARD_10	"CARD10"
#define RF_CARD_11	"CARD11"
#define RF_CARD_12	"CARD12"
#define RF_CARD_13	"CARD13"

#define RF_CARD_20	"CARD20"
#define RF_CARD_21	"CARD21"
#define RF_CARD_22	"CARD22"
#define RF_CARD_23	"CARD23"

#define RF_CARD_30	"CARD30"
#define RF_CARD_31	"CARD31"
#define RF_CARD_32	"CARD32"
#define RF_CARD_33	"CARD33"


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




#define POS_NUM_EXT(x)		( (x & 0xF0) >> 4 )
#define POS_ID_EXT(x)		( x & 0x0F)
#define DEST_CART_RAW		POS_NUM_EXT(rx_codes[i])
#define DEST_CART_COL		POS_ID_EXT(rx_codes[i])
#define CT_CART_RAW			POS_NUM_EXT(rfid_val)
#define CT_CART_COL			POS_ID_EXT(rfid_val)



#define TRACK	0x01
#define ROOM	0x00



#define SHOPPING_COMPLETE_BUTTON		read_pin_wp(PortD,PD7)
#define PACKET_COUNTER_PIN				read_pin_wp(PortD,PD6)





void rx_codes_testing(uint8_t len);
uint8_t RFID_read(void);
void Robot_move_rfid_read(void);
void robot_reach_track_2(void);

uint8_t LFS_flag_set_fun(void);
void Robot_move(void);
void robot_reverse(void);


uint16_t prod_price[7] = {10,20,50,15,30,80,0 };
uint8_t prod_count[7] = {0};



volatile uint8_t robot_working_flag = 0;
volatile uint8_t rx_codes_len=0;

int rx_codes[10];
int search_str[30];

volatile uint8_t robot_wake_up_flag = 0;
volatile uint8_t rfid_detect_flag = 0;
volatile uint8_t rfid_val=0;
uint8_t robot_dir_flag = 1; // forward

void packet_conter_fun( uint8_t prod_no );

char temp[10];

uint8_t disp_1_flag = 0;



int main(void)
{
	USART_Init();
	LCDInit(LS_NONE);
		
	sei();
		
    while(1)
    {
		
		LCDClear();
		
		if (disp_1_flag == 0)
		{
			LCDWriteString("waiting for msg ");
			
			disp_1_flag = 1;
		}
		
		
		
		
		
        if (robot_wake_up_flag == 1)
        {
			LCDClear();
			LCDWriteString("ROBOT WAKE UP ");
			
			_delay_ms(1000);
			
			robot_working_flag = 1;
			
			robot_wake_up_flag = 0;
			
			uint8_t i=0;
			
			while(i<rx_codes_len)
			{
				
				LCDClear();
				LCDWriteString("DEST:");
				
				itoa(rx_codes[i],temp,16);
				
				LCDWriteStringXY(0,1,temp);
				
				
				RFID_read();
				
				
				
				_delay_ms(500);
				
								
				if (CT_CART_RAW == 0x02 )
				{
																		
					
					
							while(CT_CART_COL != DEST_CART_COL)
							{
								Robot_move();
						
								if (robot_dir_flag != 1 ) // not forward
								{
									/*
									ROBOT_RIGHT;
									_delay_ms(1000);
							
									while(LFS_flag_set_fun() == OFF_TRACK );
												*/
									
									robot_reverse();		
								}
								
								
						
							}
							
							_delay_ms(1000);
					
										
					
					
							if (CT_CART_RAW == 0x03 )
							{
								ROBOT_RIGHT;
						
								_delay_ms(1000);
						
								while( LFS_flag_set_fun() == OFF_TRACK );
							}
							else
							{
								ROBOT_LEFT;
						
								_delay_ms(1000);
						
								while( LFS_flag_set_fun() == OFF_TRACK );
							}
					
					
							Robot_move();
					
							do 
							{
								RFID_read();
						
							} while(DEST_CART_COL != CT_CART_COL );
							
							ROBOT_STOP;
							
							
							LCDClear();
							LCDWriteString( "Destination ");
							
							_delay_ms(1000);
							
							 packet_conter_fun( i );
												
							
							//robot_reach_track_2();
							
																				
							
							//_delay_ms(1000);
							
							i++;
									
					
					
				}
				else 
				{
					robot_reverse();
					
					RFID_read();
					
				}
							
				
				
			}
			
			
			
        }
		
		
		
				
    }
	
	
}



void packet_conter_fun( uint8_t prod_no )
{
	static uint16_t total =0 ;
	
	uint8_t disp_flag = 0;
	
	
	
	while(! SHOPPING_COMPLETE_BUTTON )
	{
		
		if (disp_flag == 0)
		{
			LCDClear();
			
			LCDWriteString("COUNT   TOTAL");
			
			disp_flag = 1;
		}
		
		
		
		LCDWriteIntXY(0,1,prod_count[prod_no],3);
		
		LCDWriteIntXY(8,1,total,3);
		
		
			if (PACKET_COUNTER_PIN)
			{
				
				prod_count[prod_no] ++ ;
				
				total += ( prod_price[prod_no]  );
				
				_delay_ms(500);
						
				
			}
			
				
	}
	
			
	}
	
	
	









void robot_reach_track_2(void)
{
	
		RFID_read();
		
		if (CT_CART_RAW != 0x02)
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
		
	} while(CT_CART_RAW != 0x02 );
	
	
		
	
	
	
}




void robot_reverse(void)
{
	ROBOT_RIGHT;
	
	_delay_ms(500);
	
	while( LFS_flag_set_fun() == OFF_TRACK );
	
	//Robot_move();
}




uint8_t RFID_read()
{
	
	RFID_MODULE_ON;
	
	robot_working_flag = 1;
	
		
	while( rfid_detect_flag == 0 )
	{
		
		if(LFS_flag_set_fun() == OFF_TRACK )
		{
			ROBOT_RIGHT;
			
			_delay_ms(1000);
			
			ROBOT_FORWARD;
			
			_delay_ms(200);
		}
		else
		{
			 Robot_move_rfid_read();
		}
		
		
	}
	
	rfid_detect_flag = 0;
	
	return(rfid_val);
	
	
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
		
		
		case 0B1100:{ //  right deviation
			
						return(RIGHT_DEV);
						break;
		}
		
		
		
		case 0B0011: { // left deviation
						
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
			
		case 0B1000 : {
			
						return( RIGHT_SING );
			
						break;
		}
		
				
		case 0B0001: {
			
						return ( LEFT_SING );
			
						break;
		}
					
		default:{
			
						return(ILLEGAL);
		}
		
		
	}
	
	
}







void Robot_move_rfid_read(void)
{
	
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD ; break; }
		
		case RIGHT_DEV :	{ ROBOT_LEFT; break; }
		
		case RIGHT_MARK :	{ ROBOT_RIGHT;break;}
		
		case RIGHT_SING :	{ ROBOT_RIGHT; break; }
		
		case LEFT_DEV :		{ ROBOT_RIGHT; break; }
		
		case LEFT_MARK :	{ ROBOT_LEFT;break;	}
		
		case LEFT_SING :	{ ROBOT_LEFT;break; }
		
		case OFF_TRACK :	{ ROBOT_FORWARD; break; }
		
		case ON_JUNCTION :	{ ROBOT_RIGHT; break; }
		
		case ILLEGAL :		{ ROBOT_FORWARD; break; }
		
		default:			{ ROBOT_FORWARD; break; }
		
				
	}
	
}




void Robot_move(void)
{
	uint8_t flag_val;
	
	flag_val = LFS_flag_set_fun();
	
	switch( flag_val )
	{
		
		case ON_TRACK :		{ ROBOT_FORWARD ; break; }
		
		case RIGHT_DEV :	{ ROBOT_LEFT; break; }
		
		case RIGHT_MARK :	{
			
								ROBOT_FORWARD;
			
								_delay_ms(500);
			
								if ( LFS_flag_set_fun() == ON_TRACK )
								{
				
									robot_dir_flag = 1 ;// moving forward
				
									break;
								}
								else
								{
									ROBOT_RIGHT;
				
									break;
								}
			
			
			
			}
			
			case RIGHT_SING :	{ ROBOT_RIGHT; break; }
			
			case LEFT_DEV :		{ ROBOT_RIGHT; break; }
			
			case LEFT_MARK :	{
			
										ROBOT_FORWARD;
			
										_delay_ms(500);
			
										if ( LFS_flag_set_fun() == ON_TRACK )
										{
												robot_dir_flag = 0 ;// backward
			
												break;
										}
										else
										{
												ROBOT_LEFT;
			
												break;
										}
			
			
			}
			
			case LEFT_SING :	{ ROBOT_LEFT;break; }
			
			case OFF_TRACK :	{ ROBOT_FORWARD; break; }
			
			case ON_JUNCTION :	{ ROBOT_RIGHT; break; }
			
			case ILLEGAL :		{ ROBOT_FORWARD; break; }
			
			default:			{ ROBOT_FORWARD; break; }
			
						
			}
			
				
}











ISR(USART_RXC_vect)
{
	
	static char str_buffer[50];
	static uint8_t count;
	
		
	if (robot_working_flag == 0 )
	{
		
		str_buffer[count] = UDR;
		
		count++;
		
		if ( (str_buffer[count-1] == 0x0D) ||  ( count >= 50 ) )
		{
			
																		
			if ( strstr(str_buffer,"ENT") )
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
														
				robot_wake_up_flag = 1;	
				robot_working_flag = 1;				
				
				disp_1_flag = 0;
				
			}
				
			
			count = 0;
						
		}
				
	}
	else
	{
		str_buffer[count] = UDR;
		
		count++;
		
		
				
			if(   count >= RFID_CARD_LEN )
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
						
										
					count = 0;
					
									
					
					
			}
				
				
				
	}
			
}
		
		
	







void rx_codes_testing(uint8_t len)
{
	
	char test_str[20];
	uint8_t i;
	
	for (i=0;i<len;i++)
	{
		itoa(rx_codes[i],test_str,10);
		
		send_string(test_str);
		
		send_string("\r");
	}
	
	
	
}





