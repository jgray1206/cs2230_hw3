#include <msp430.h>

int mode = 0;
int main(void)
{
  	WDTCTL  = WDTPW | WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	P1DIR = BIT0 | BIT1 | BIT6;	
	P1OUT = 0;

	P1REN = BIT3;
	P1OUT |= BIT3;
	P1IE |= BIT3;
	P1IES |= BIT3;
	P1IFG &= ~BIT3;

	TA0CTL   = TASSEL_2 | MC_1 | ID_3;
    	TA0CCR0  = 41666;
    	TA0CCTL1 = OUTMOD_4;



	TA1CTL   = TASSEL_2 | MC_1 | ID_3;
	TA1CCTL1 = OUTMOD_4;

	P2DIR = BIT1;


	__eint();

	for(;;){
		if(mode==0){
			P1OUT ^= BIT0;
			__delay_cycles(500000);
				
		}
	
		if(mode==1){
			P1SEL = BIT6;
			P1OUT ^= BIT0;
			__delay_cycles(500000);
		}

	   	if(mode==2){
			P2SEL = BIT1;
			P1SEL = BIT6;
			TA1CCR0 = 31250;
		}
		if(mode==3){
			P2SEL = BIT1;
			TA1CCR0 = 62500;
		}
	}

}

#pragma vector=PORT1_VECTOR
__interrupt void button(void)
{
	P1SEL = 0;
	P2OUT = 0;
	P2SEL = 0;
	P1OUT = BIT3;

	mode = (mode+1)%5;
	while(!(BIT3 & P1IN)){}
	__delay_cycles(35000);
	P1IFG &= ~BIT3;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void blink(void)
{
	P1OUT ^= BIT6;
}
