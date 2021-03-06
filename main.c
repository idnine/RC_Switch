/*
 * RC Switch / MSP430
 * by Koo Jonghoe (idnine@gmail.com)
 *
 * == Hardware Setup ==
 * P1.0 LED OUT (toggle)
 * P1.3 RC Signal Input (PWM)
 *
 * == RC Receiver Output is 50Hz PWM for Servo Motor ==
 * PWM is 50Hz = 20ms
 * Signal High 1.0ms = Full Left Position
 * Signal High 1.5ms = Middle Position
 * Signal High 2.0ms = Full Right Position
 *
 */

#include <msp430.h>

#define		RCin			BIT3
#define		LEDout			BIT0
#define		LED2out			BIT6
#define		DELAY100us		6
#define		RChighTimes		20
#define		RCthreshold		15
#define		TactSwDelay		4

volatile int isChange = 0;
volatile int oldValue = 0;

int main(void)
{
	//
	// WatchDogClock Disable
	//
	WDTCTL = WDTPW + WDTHOLD;
	//
	// Main Clock Set 1MHz
	//
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	//
	// IO Set
	//
	P1DIR |= (LEDout + LED2out);	// Output P1.0, P1.6
	P1OUT &= ~(LEDout + LED2out);	// P1.0, P1.6 Clear
	P1DIR &= ~RCin;					// Input P1.3
	P1OUT |= RCin;					// Input P1.3 Pull-Up Set
	P1REN |= RCin;					// P1.3 Internal Resister
	P1IE |= RCin;					// P1.3 interrupt enabled
	P1IES &= ~RCin;					// P1.3 Low -> High Interrupt Edge Select
	P1IFG &= ~RCin;					// P1.3 Interrupt Flag clear
	//
	// LPM4 Mode & Wait Interrupt
	//
	_BIS_SR(LPM4_bits + GIE);		// Enter LPM4 w/interrupt
}

//
// delay 100us
//
void delay100us(void)
{
	volatile int i;
	for(i=0; i < DELAY100us; i++);
}

//
// Port 1 interrupt service routine
//
#pragma vector=PORT1_VECTOR
__interrupt void RCswitchInt(void)
{
	int newValue;
	int i;
	int cntRC = 0;

	for(i=0; i < RChighTimes; i++)
	{
		if(P1IN & RCin) cntRC++;
		delay100us();
	}
	if(cntRC > RCthreshold)
	{
		P1OUT |= LEDout;
		newValue = 1;
	} else {
		P1OUT &= ~LEDout;
		newValue = 0;
	}
	if(newValue == oldValue)
	{
		if(isChange > TactSwDelay)
		{
			P1OUT &= ~LED2out;
			isChange = 0;
		} else {
			isChange++;
		}
	} else {
		isChange = 1;
		oldValue = newValue;
		P1OUT |= LED2out;
	}
	//
	// P1 RCin IFG Cleared
	//
	P1IFG &= ~RCin;
}
