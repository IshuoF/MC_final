#include <xc.h>

void CCP1_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP1CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
    
    // Timer2 -> On, prescaler -> 16
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b11;
    // Internal Oscillator Frequency, Fosc = 4 MHz, Tosc = 8 µs
//    OSCCONbits.IRCF = 0b001;
//    
//    // PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.CCP1M = 0b1100;
//    
//    // CCP1/RC2 -> Output
//    
//    // Set up PR2, CCP to decide PWM period and Duty Cycle
    
    PR2 = 199;
//    
//    //rotate to degree 0
    CCPR1L = 90;
    CCP1CONbits.DC1B = 0b01;
}
