#include <avr/io.h>

#define ADC_VREF_TYPE 0x43 // Vref = Vcc ;AVCC with external capacitor at AREF pin

//-------------------------------------------
// PROTOTYPES
void ADC_Init(void);
unsigned int ADC_Read(unsigned char	adc_input);

void ADC_Init(void)
{
	ADMUX=ADC_VREF_TYPE; // Vref = Vcc, ADC0
	ADCSRA = 0X86; // PRESCALE = 64; 6000000/64=93 kHz
}
//----------------------------------------------
unsigned int ADC_Read(unsigned char	adc_input)
{
	ADMUX=adc_input|ADC_VREF_TYPE;
	ADCSRA|=(1<<ADSC); // start conversion
	while ((ADCSRA & 0x10)==0);
	ADCSRA&=~(0x10);
	return ADC;
}
//----------------------------------------------

