// user defined controller registor definitions and functions

#include "p30f6010A.h"

#define RL1_ON PORTGbits.RG0
#define RL2_ON PORTGbits.RG1
#define RL3_ON PORTGbits.RG2
#define RL4_ON PORTGbits.RG3
#define RL5_ON PORTGbits.RG6

#define PWM1 PDC1
#define PWM2 PDC2
#define PWM3 PDC3

#define T1us_Flag IFS0bits.T1IF //every 1.5us flag

#define PID_V_count 33 //20.2Khz sampling
#define PID_I_count 12 //55.55Khz sampling

#define PWM_PERIOD PTPER 
#define Converstion_Done ADCON1bits.DONE
#define Start_Converstion ADCON1bits.SAMP

#define CH0_MULA ADCHSbits.CH0SA
#define CH0_MULB ADCHSbits.CH0SB

#define PWMenable PTCONbits.PTEN

#define Ipeak_flag IFS2bits.PWMIF

#define Irpeak 640 //142count == 1A peak current limit

#define V_Pgain 16 // PI gain parameters
#define V_Igain 1  //

#define Vr_ref 412 //1 count == .218volts //max voltage ref 920
                                          //min voltage ref 100
#define softCount 700 //1.05mS //soft start counter

#define IrTIMER_ON T2CONbits.TON //timer2 for peak current detect
#define IrCOUNT PR2 
#define T2_Flag IFS0bits.T2IF    //for Ir peak detect

// functions
int adc(char); // adc function definition

void delay(unsigned int); //delay function definition

void init(void); //processor initialisation function definition



