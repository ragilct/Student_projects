/*
 * ADC_CONFIG.h
 *
 * Created: 22-Feb-15 8:05:31 AM
 *  Author: ADMIN
 */ 


#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_


#include <avr/io.h>

// ADC mode settings 

	//#define ADC_DIFFERENTIAL // uncomment this to enable differential mode
	// if it is commented adc will work in default single ended mode



// ADC reference settings

	#define	AREF	0B00	//AREF, Internal V ref turned off
	#define AVCC	0B01	//AVCC with external capacitor at AREF pin
	#define IV		0B11	//Internal 2.56V Voltage Reference with external capacitor at AREF pin



// ADC channel settings

	#define ADC0 0B00000 
	#define ADC2 0B00010 
	#define ADC3 0B00011  
	#define ADC4 0B00100 
	#define ADC5 0B00101 
	#define ADC6 0B00110 
	#define ADC7 0B00111 


#ifdef  ADC_DIFFERENTIAL
	
	#warning	"ADC in differential mode comment ADC_DIFFERENTIAL in ADC_CONFIG.h for default single ended mode operation"
	
	#ifdef __AVR_ATmega8__
	
		#error " ATmega8 not supports ADC differential mode "
	
	#endif

	#define DIFF_ADC0_ADC0_10x   0B01000	//		ADC0 ADC0 10x
	#define	DIFF_ADC1_ADC0_10x   0B01001	//		ADC1 ADC0 10x
	#define DIFF_ADC0_ADC0_200x  0B01010	//		ADC0 ADC0 200x
	#define DIFF_ADC1_ADC0_200x  0B01011	//		ADC1 ADC0 200x
	#define DIFF_ADC2_ADC2_10x   0B01100	//		ADC2 ADC2 10x
	#define DIFF_ADC3_ADC2_10x   0B01101 	//		ADC3 ADC2 10x
	#define DIFF_ADC2_ADC2_200x  0B01110	//		ADC2 ADC2 200x
	#define DIFF_ADC3_ADC2_200x  0B01111	//		ADC3 ADC2 200x
	#define DIFF_ADC0_ADC1_1x    0B10000	//		ADC0 ADC1 1x
	#define DIFF_ADC1_ADC1_1x    0B10001	//		ADC1 ADC1 1x
	#define DIFF_ADC2_ADC1_1x    0B10010	//		ADC2 ADC1 1x
	#define DIFF_ADC3_ADC1_1x    0B10011	//      ADC3 ADC1 1x
	#define DIFF_ADC4_ADC1_1x    0B10100	//		ADC4 ADC1 1x
	#define DIFF_ADC5_ADC1_1x    0B10101	//		ADC5 ADC1 1x
	#define DIFF_ADC6_ADC1_1x    0B10110    //		ADC6 ADC1 1x
	#define DIFF_ADC7_ADC1_1x    0B10111    //		ADC7 ADC1 1x
	#define DIFF_ADC0_ADC2_1x    0B11000	//		ADC0 ADC2 1x
	#define DIFF_ADC1_ADC2_1x    0B11001	//		ADC1 ADC2 1x
	#define DIFF_ADC2_ADC2_1x    0B11010	//		ADC2 ADC2 1x
	#define DIFF_ADC3_ADC2_1x    0B11011	//		ADC3 ADC2 1x
	#define DIFF_ADC4_ADC2_1x    0B11100	//		ADC4 ADC2 1x
	
#else

	#warning "ADC in default single ended  mode uncomment ADC_DIFFERENTIAL in ADC_CONFIG.h for differential mode operation "

#endif




// FUNCTION PROTOTYPES
void ADC_Init( uint8_t adc_ref );
uint16_t ADC_Read( uint8_t channel);


#endif /* ADC_CONFIG_H_ */