//**********************************************************************//
// AC LAB KIT BUCK SINGLE OUTPUT CONFIGURATION                          //
//**********************************************************************//

//include files
#include "p30f6010A.h"
#include "main.h"
//

//
char PID_Vsample =0;         //voltage PID sampling counter

unsigned int softStart = 0;  //soft start counter

int Ir = 0;

static int Voffset = 0;

static long int Vr_PREerror = 0;
static long int Integral = 0;

static int VrREF = -(55200/(int)V_Pgain); //set initial voltage ref
//

//
void PID_Vr(void); //Voltage PI function
//

int main(void)
{

init();

//precharging init
PWMenable = 0; //PWM is off
delay(300); //delay 300ms
RL1_ON = 1; //precharging enable
delay(1000); //delay 1000ms
RL1_ON = 0; //precharging disable
delay(30); //delay 50ms
RL2_ON = 1; //bypass precharging
delay(30); //delay 30ms
PWMenable = 1; //PWM is on
delay(1); //delay 1ms

PWM1 = 0;
PWM2 = 0;
PWM3 = 0;
//precharging init ends

Voffset = adc(5); //read voltage offset


    while(1)
	{

//Ir peak detect every 0 match of PWM
if(Ipeak_flag) //if PWM 0 match
{
IrCOUNT = PWM1; //T2 period value
IrCOUNT = IrCOUNT>>1; //set timer for half of the PWM on time
IrTIMER_ON = 1; //T2 on
Ipeak_flag = 0; //reset PWM1 0 match flag
}
//peak detect end

//Ir peak control
if(T2_Flag) //sample inductor current
{
IrTIMER_ON = 0; //T2 off
T2_Flag = 0;
Ir = adc(11); //sample Ir
if(Ir >= (int)Irpeak) PWMenable = 0; //if peak detect pwm is off
} //peak control ends

//loop counters
if(T1us_Flag) 
   {
PID_Vsample++; 

softStart++;

T1us_Flag = 0;
   }
//loop counters end

if(PID_Vsample >= (char)PID_V_count) //voltage sample
   {
    PID_Vr(); //call voltage PI
    PID_Vsample = 0;
   } //voltage sample end


if(softStart >= (unsigned int)softCount) //soft start
   {
    VrREF++; //increment output voltage ref with soft start
    
    if(VrREF >= (int)Vr_ref) VrREF = (int)Vr_ref; // clamp to voltage ref
    
    softStart = 0; //reset flag
   } //soft start end

     } //while end

} //main end

//

//Converter ouptut voltage Vr PI routine
void PID_Vr()
{

int currentError = 0;
long int Pterm = 0;
long int Iterm = 0;

int voltage;
long int PI_value = 0;

voltage = adc(5); // read output voltage using adc channel 5
voltage = (voltage - (Voffset+2))<<1; //subtract 2.5 V offset in feedback

if(voltage <= 0) voltage = 0;

currentError = VrREF - voltage; // calculate error

Pterm = (long int)currentError*(long int)V_Pgain; //calculate Proportional term = error*Pgain
if(Pterm >= 55200) Pterm = 55200;
if(Pterm <= -55200) Pterm = -55200;

Integral = Vr_PREerror + (long int)currentError; //calculated integrated error term = current error+previous error
if(Integral >= 55200) Integral = 55200;
if(Integral <= -55200) Integral = -55200;
Vr_PREerror = Integral;

Iterm = Integral*(long int)V_Igain; //calculate Integral term = integarted error*Igain
if(Iterm >= 55200) Iterm = 55200;
if(Iterm <= -55200) Iterm = -55200;

PI_value = Pterm + Iterm; // PI output = Pterm+Iterm
if(PI_value >= 55200) PI_value = 55200;
if(PI_value <= -55200) PI_value = -55200;

PI_value = PI_value>>5;

asm("disi #0x3FFF");
PWM1 = (int)PI_value; //program duty cycle 1
asm("disi #0x0000");

}
//Vr PI routine ends

