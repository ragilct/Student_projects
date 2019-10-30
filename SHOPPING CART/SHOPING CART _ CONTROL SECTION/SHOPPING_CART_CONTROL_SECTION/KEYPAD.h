/*
 * KEYPAD.h
 *
 * Created: 31-Jan-15 8:17:05 AM
 *  Author: ADMIN
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>




#define KEYPAD	A



#ifndef MYUTILS_H

#define CONCAT(a,b)	a##b
#define PORT(x)	CONCAT(PORT,x)
#define DDR(x)	CONCAT(DDR,x)
#define PIN(x)	CONCAT(PIN,x)

#endif

#define KEYPAD_PORT		PORT(KEYPAD)
#define KEYPAD_DDR		DDR(KEYPAD)
#define KEYPAD_PIN		PIN(KEYPAD)


//  KEYPAD READ FUNCTIONS

uint8_t GetKeyPressed();
char read_keypad_char();
void read_keypad_str( char *str, uint8_t count );
uint16_t read_keypad_int(uint8_t digit_no );




#endif /* KEYPAD_H_ */