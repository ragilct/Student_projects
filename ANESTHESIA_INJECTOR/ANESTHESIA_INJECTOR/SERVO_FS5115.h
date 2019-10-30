/*
 * SERVO_FS5115.h
 *
 * Created: 13-Mar-15 3:10:51 PM
 *  Author: ADMIN
 */ 



/*

use 6mhz crystal

angle value		80 to 260  represents 0 to 180

*/



#ifndef SERVO_FS5115_H_
#define SERVO_FS5115_H_

#include <avr/io.h>
#include <util/delay.h>


#define  SERVO_X_OFF	TCCR1A	&= ( ~(1<<COM1A1))
#define  SERVO_Y_OFF	TCCR1A	&= ( ~(1<<COM1B1))


// should connect 2200uf capacitor to servo supply






void servo_angle_set_x_axis( float);
void servo_angle_set_y_axis(float );
void servo_init_function (void);


#endif /* SERVO_FS5115_H_ */