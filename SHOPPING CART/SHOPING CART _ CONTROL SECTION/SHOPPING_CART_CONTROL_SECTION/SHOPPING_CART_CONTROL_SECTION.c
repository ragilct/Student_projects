/*
 * SHOPPING_CART_CONTROL_SECTION.c
 *
 * Created: 31-Jan-15 7:25:54 AM
 *  Author: ADMIN
 */ 


#define F_CPU 6000000

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#include "lcd.h"
#include "PIN_CONFIG.h"
#include "UART.h"
#include "KEYPAD.h"





#define CLEAR			read_pin_wp(PortC,PC0)
#define SEND			read_pin_wp(PortC,PC1)
#define PREVIOUS		read_pin_wp(PortC,PC3)
#define ENTER			read_pin_wp(PortC,PC4)
#define NEXT			read_pin_wp(PortC,PC5)

					  






////////////////////////////////////////////////////////


typedef struct
{
	char item_name[20];
	
	uint16_t price;
	
	uint8_t loc;
	
	char item_id[5];
	
}goods;

goods item[10];

goods stored_item[7] = {
	
	{ "ENTER NEW LIST ",10,20,"ENT"},
		
	{"NO:1",10,11," 1"},
		
	{"NO:2",15,12," 2"},
	
	{"NO:3",25,13," 3"},
	
	{"NO:4",20,31," 4"},
	
	{"NO:5",30,32," 5"},
	
	{"NO:6",50,33," 6"}
	
};




void system_init(void)
{
	
	LCDInit(LS_NONE);
	USART_Init();
	
}


void Display_stored( uint8_t x );
void scroll_switch_control(void);




volatile uint8_t scroll_count=0 ;
uint8_t status_flag = 0;
char str_send[20];
char cart_str[20]="ENT";
uint8_t cart_count = 1;

int main(void)
{
	
	system_init();
	LCDWriteString(" SHOPPING CART ");
	_delay_ms(500);
	LCDClear();
	LCDWriteString(" ENTER NEW LIST ");
		
    while(1)
    {
       scroll_switch_control();
    }
}




void Display_stored( uint8_t x )
{
	LCDClear();
	LCDWriteStringXY(0,0,"ITEM");
	LCDWriteStringXY(0,1,(stored_item[x].item_name));
	LCDWriteStringXY(8,0,"RS");
	LCDWriteIntXY(8,1,stored_item[x].price,3);
	LCDWriteStringXY(13,0,"LOC");
	LCDWriteIntXY(13,1,stored_item[x].loc,3);
	
}





void scroll_switch_control(void)
{
	
	
				
				
				if (PREVIOUS)
				{
					
							if (scroll_count < 1 )
							{
								scroll_count = 6;
							}
							else
							{
								scroll_count--;
								if (scroll_count <= 0)
								scroll_count = 6;
								Display_stored(scroll_count);
								_delay_ms(250);
							}
							
					
				}
				else if (ENTER)
				{
					
							if ( cart_count < 6 )
							{
								uint8_t id_pre=0;
						
								if ( strstr( cart_str,stored_item[scroll_count].item_id) )
								id_pre = 1;
								else
								id_pre = 0;
																
								if ( id_pre == 0 )
								{
																	
									strcat(cart_str,stored_item[scroll_count].item_id );
									LCDClear();
									LCDWriteString("ADD TO CART");
									LCDWriteStringXY(0,1,"NOS:");
									LCDWriteIntXY(4,1,cart_count,2);
									send_string(cart_str);
									_delay_ms(500);
									cart_count++;
											
								}
								else
								{
							
									LCDClear();
									LCDWriteString(" ADDED TO CART ");
									LCDWriteStringXY(0,1,"ENTER NEW LIST ");
							
								}
						
						
							}
							else
							{
									LCDClear();
									LCDWriteString("CART FULL");
						   		    _delay_ms(500);
							}
					
				}
				else if ( NEXT )
				{
					
							if (scroll_count > 6 )
							{
						    	scroll_count = 1;
							}
							else
							{
								scroll_count++;
								
								if (scroll_count > 6)
								scroll_count = 1;
								Display_stored(scroll_count);
								_delay_ms(250);
							}
					
				}
		
		
			
				if(CLEAR)
				{
					LCDClear();
					LCDWriteString("CART IS CLEAR ");
					LCDWriteStringXY(0,1,"ENTER NEW LIST");
					cart_count = 0;
					scroll_count = 0;
					*cart_str = '\0';
					strcat(cart_str,stored_item[scroll_count].item_id );
					_delay_ms(500);
			
				}
				
				
				
				if (SEND)
				{
					if ( cart_str != '\0')
					{
						send_string(cart_str);
						send_data ('\n');
						cart_count = 0;
						scroll_count = 0;
						*cart_str = '\0';
						strcat(cart_str,stored_item[scroll_count].item_id );
						_delay_ms(500);
						
						LCDClear();
						LCDWriteString("CHECK OUT ");
						LCDWriteStringXY(0,1,"ENTER NEW LIST");
					}
					else
					{
						LCDClear();
						LCDWriteString("SELECT ITEMS ");
						_delay_ms(500);
					}
				}
	
	
				
					
					
	
}









