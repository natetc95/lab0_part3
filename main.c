#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"

typedef enum stateEnum {
    led1, led2, led3
}state;

typedef enum boolean {
    true, false
}bool;

volatile state stat = led1;
volatile bool next = true;

int main(void) {
    enableInterrupts();
    //LED Initialization
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATDbits.LATD2 = 0;
    //Timer Initialization
    PR1 = 2441;
    TMR1 = 0;
    T1CONbits.TCKPS = 3;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 5;
    T1CONbits.ON = 1;
    //Switch Initialization
    TRISDbits.TRISD6 = 1;
    CNCONDbits.ON = 1;
    CNENDbits.CNIED6 = 1;
    IFS1bits.CNDIF = 0;
    CNPUDbits.CNPUD6 = 1;
    while(1) {
        switch(stat) {
            case led1:
                LATDbits.LATD0 = 1;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                break;
            case led2:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 1;
                LATDbits.LATD2 = 0;
                break;
            case led3:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 1;
                break;
            default:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                break;
        }
    }
    return 0;
}

void __ISR(_TIMER_1_VECTOR, IPL5SRS) T1Interrupt() {
    IFS0bits.T1IF = 0;
    if(stat == led1) stat = led3;
    else if(stat == led2) stat = led1;
    else if(stat == led3) stat = led2;
}