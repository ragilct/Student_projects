/*
 * PACE_MAKER.c
 *
 * Created: 26-Oct-14 12:09:46 PM
 *  Author: MAHMOOD
 */ 

#define F_CPU 6000000UL
#define BAUD_RATE 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <velectro/adc.h>
#include <velectro/PIN_CONFIG.c>
#include <velectro/uart.h>
#include <stdlib.h>
#include <string.h>

#define BLOOD_PRESSURE 0
#define HEART_RATE 1
#define INTRA_IMP 2
#define VENTRI_RATE 3

#define BP_Port PortB
#define IND1_E PB0
#define IND11 PB1
#define IND12 PB2

#define HR_Port PortB
#define IND2_E PB3
#define IND21 PB4
#define IND22 PB5

#define IMP_Port PortD
#define IND3_E PD2
#define IND31 PD3
#define IND32 PD4

#define VR_Port PortD
#define IND4_E PD5
#define IND41 PD6
#define IND42 PD7

#define HR_Normal 72
#define HR_Interval HR_Normal/4
#define BP_Normal 120
#define BP_Interval BP_Normal/4
#define VR_Normal 80
#define VR_Interval VR_Normal/4
#define IMP_Normal 200
#define IMP_Interval IMP_Normal/4  

uint16_t HeartRate,BloodPressure,IntraImp,VentricularRate;
uint32_t BP_value,HR_value,IMP_value,VR_value;
char s[10];
uint8_t BP_Risk,HR_Risk,IMP_Risk,VR_Risk,TotalRisk;

/*
uint8_t uart_buffer[50],temp_buffer[50],temp[10];
uint8_t HR_Normal,BP_Normal,IMP_Normal,VR_Normal;
uint8_t HR_Interval,BP_Interval,VR_Interval,IMP_Interval;
*/


void read_pacemaker();
void display_in_PC();
void heart_rate_indicator();
void blood_pressure_indicator();
void ventricaular_rate_indicator();
void itra_impedance_indicator();
//==========================================



int main(void)
{
	ADC_Init();
	uart_init();
	
	
	
    while(1)
    {  
		/*
		HR_Interval=HR_Normal/4;
		BP_Interval=BP_Normal/4;
		VR_Interval=VR_Normal/4;
		IMP_Interval=IMP_Normal/4;
		*/
		
		read_pacemaker();
		
		HeartRate=(HR_value*HR_Normal)/512;
		BloodPressure=(BP_value*BP_Normal)/512;
		IntraImp=(IMP_value*IMP_Normal)/512;
		VentricularRate=(VR_value*VR_Normal)/512;		
		
		
		if (HeartRate==HR_Normal)
			HR_Risk=0;
		else if( ( (HeartRate>HR_Normal) & (HeartRate<=(HR_Normal+HR_Interval)) )| ( (HeartRate<HR_Normal) & (HeartRate>=(HR_Normal-HR_Interval)) ) )
			HR_Risk=1;
		else if ( ( (HeartRate>(HR_Normal+HR_Interval)) & (HeartRate<=(HR_Normal+2*HR_Interval)) )| ( (HeartRate<(HR_Normal-HR_Interval)) & (HeartRate>=(HR_Normal-2*HR_Interval)) ) ) 
			HR_Risk=2;
		else if ( (HeartRate>(HR_Normal+2*HR_Interval)) | (HeartRate<(HR_Normal-2*HR_Interval)) )
			HR_Risk=3;
		else 
			HR_Risk=4;
		
		heart_rate_indicator();
		
		
		
		if(BloodPressure==BP_Normal)
			BP_Risk=0;
		else if( ( (BloodPressure>BP_Normal) & (BloodPressure<=(BP_Normal+BP_Interval)) ) | ((BloodPressure<BP_Normal) & (BloodPressure>=(BP_Normal-BP_Interval)) ) )
			BP_Risk=1;
		else if( ( (BloodPressure>(BP_Normal+BP_Interval)) & (BloodPressure<=(BP_Normal+2*BP_Interval)) ) | ((BloodPressure<(BP_Normal-BP_Interval)) & (BloodPressure>=(BP_Normal-2*BP_Interval)) ) )
			BP_Risk=2;
		else if( (BloodPressure>(BP_Normal+2*BP_Interval)) | (BloodPressure<(BP_Normal-2*BP_Interval)) )
			BP_Risk=3;
		else
			BP_Risk=4;
		
		blood_pressure_indicator();
		
		
		
		if(VentricularRate==VR_Normal)
			VR_Risk=0;
		else if( ( (VentricularRate>VR_Normal) & (VentricularRate<=(VR_Normal+VR_Interval)) ) | ( (VentricularRate<VR_Normal) & (VentricularRate>=(VR_Normal-VR_Interval)) ) )
			VR_Risk=1;
		else if( ( (VentricularRate>(VR_Normal+VR_Interval)) & (VentricularRate<=(VR_Normal+2*VR_Interval)) ) | ( (VentricularRate<(VR_Normal-VR_Interval)) & (VentricularRate>=(VR_Normal-2*VR_Interval)) ) )
			VR_Risk=2;
		else if( (VentricularRate>(VR_Normal+2*VR_Interval)) | (VentricularRate<(VR_Normal-2*VR_Interval)) )
			VR_Risk=3;
		else
			VR_Risk=4;
		
		ventricaular_rate_indicator();
		
		
		if(IntraImp==IMP_Normal)
			IMP_Risk=0;
		else if( ( (IntraImp>IMP_Normal) & (IntraImp<=(IMP_Normal+IMP_Interval)) ) | ( (IntraImp<IMP_Normal) & (IntraImp>=(IMP_Normal-IMP_Interval))  ) )
			IMP_Risk=1;
		else if( ( (IntraImp>(IMP_Normal+IMP_Interval)) & (IntraImp<=(IMP_Normal+2*IMP_Interval)) ) | ( (IntraImp<(IMP_Normal-IMP_Interval)) & (IntraImp>=(IMP_Normal-2*IMP_Interval)) ) )
			IMP_Risk=2;
		else if( (IntraImp>(IMP_Normal+2*IMP_Interval)) | (IntraImp<(IMP_Normal-2*IMP_Interval) ) )
			IMP_Risk=3;
		else
			IMP_Risk=4;
			
		itra_impedance_indicator();
		
		
		TotalRisk=BP_Risk + HR_Risk + VR_Risk + IMP_Risk;
		display_in_PC();
			
    }
}

//===========================================================


ISR(USART_RXC_vect)
{
	
}

void read_pacemaker()
{
	BP_value=ADC_Read(BLOOD_PRESSURE);
	HR_value=ADC_Read(HEART_RATE);
	IMP_value=ADC_Read(INTRA_IMP);
	VR_value=ADC_Read(VENTRI_RATE);
}

//-----------------------------------------

void display_in_PC()
{
	itoa(BloodPressure,s,10);
	uart_putc('b');
	uart_puts(s);
	uart_puts("   ");
	
	itoa(HeartRate,s,10);
	uart_putc('h');
	uart_puts(s);
	uart_puts("   ");
	
	itoa(IntraImp,s,10);
	uart_putc('i');
	uart_puts(s);
	uart_puts("   ");
	
	itoa(VentricularRate,s,10);
	uart_putc('v');
	uart_puts(s);
	uart_puts("   ");
	
	itoa(BP_Risk,s,10);
	uart_putc('p');
	uart_puts(s);
	uart_putc(' ');
	
	itoa(HR_Risk,s,10);
	uart_putc('q');
	uart_puts(s);
	uart_putc(' ');
	
	itoa(IMP_Risk,s,10);
	uart_putc('r');
	uart_puts(s);
	uart_putc(' ');
	
	itoa(VR_Risk,s,10);
	uart_putc('s');
	uart_puts(s);
	uart_putc(' ');
	
	itoa(TotalRisk,s,10);
	uart_putc('t');
	uart_puts(s);
	uart_putc(' ');
	uart_putc('\n');
	
}

//-----------------------------------------------

void heart_rate_indicator()
{
	switch(HR_Risk)
	{
		case 0: output_pin(HR_Port,IND2_E,LOW);
				output_pin(HR_Port,IND21,LOW);
				output_pin(HR_Port,IND22,LOW);
				break;
		case 1: output_pin(HR_Port,IND2_E,LOW);
				output_pin(HR_Port,IND21,HIGH);
				output_pin(HR_Port,IND22,LOW);
				break;
		case 2: output_pin(HR_Port,IND2_E,LOW);
				output_pin(HR_Port,IND21,LOW);
				output_pin(HR_Port,IND22,HIGH);
				break;
		case 3: output_pin(HR_Port,IND2_E,LOW);
				output_pin(HR_Port,IND21,HIGH);
				output_pin(HR_Port,IND22,HIGH);
				break;
		default: output_pin(HR_Port,IND2_E,HIGH);
				break;
	}
	 
}

//-----------------------------------------------
	
void blood_pressure_indicator()
{
	switch(BP_Risk)
	{
		case 0: output_pin(BP_Port,IND1_E,LOW);
				output_pin(BP_Port,IND11,LOW);
				output_pin(BP_Port,IND12,LOW);
				break;
		case 1: output_pin(BP_Port,IND1_E,LOW);
				output_pin(BP_Port,IND11,HIGH);
				output_pin(BP_Port,IND12,LOW);
				break;
		case 2: output_pin(BP_Port,IND1_E,LOW);
				output_pin(BP_Port,IND11,LOW);
				output_pin(BP_Port,IND12,HIGH);
				break;
		case 3: output_pin(BP_Port,IND1_E,LOW);
				output_pin(BP_Port,IND11,HIGH);
				output_pin(BP_Port,IND12,HIGH);
				break;
		default: output_pin(BP_Port,IND1_E,HIGH);
				break;
	}
	
}

//-----------------------------------------------------------

void ventricaular_rate_indicator()
{
	switch(VR_Risk)
	{
		case 0: output_pin(VR_Port,IND4_E,LOW);
				output_pin(VR_Port,IND41,LOW);
				output_pin(VR_Port,IND42,LOW);
				break;
		case 1: output_pin(VR_Port,IND4_E,LOW);
				output_pin(VR_Port,IND41,HIGH);
				output_pin(VR_Port,IND42,LOW);
				break;
		case 2: output_pin(VR_Port,IND4_E,LOW);
				output_pin(VR_Port,IND41,LOW);
				output_pin(VR_Port,IND42,HIGH);
				break;
		case 3: output_pin(VR_Port,IND4_E,LOW);
				output_pin(VR_Port,IND41,HIGH);
				output_pin(VR_Port,IND42,HIGH);
				break;
		default:output_pin(VR_Port,IND4_E,HIGH);
				break;
	}
}

//-----------------------------------------------------------

void itra_impedance_indicator()
{
	switch(IMP_Risk)
	{
		case 0: output_pin(IMP_Port,IND3_E,LOW);
				output_pin(IMP_Port,IND31,LOW);
				output_pin(IMP_Port,IND32,LOW);
				break;
		case 1: output_pin(IMP_Port,IND3_E,LOW);
				output_pin(IMP_Port,IND31,HIGH);
				output_pin(IMP_Port,IND32,LOW);
				break;
		case 2: output_pin(IMP_Port,IND3_E,LOW);
				output_pin(IMP_Port,IND31,LOW);
				output_pin(IMP_Port,IND32,HIGH);
				break;
		case 3: output_pin(IMP_Port,IND3_E,LOW);
				output_pin(IMP_Port,IND31,HIGH);
				output_pin(IMP_Port,IND32,HIGH);
				break;
		default: output_pin(IMP_Port,IND3_E,HIGH);
				break;
	}
}