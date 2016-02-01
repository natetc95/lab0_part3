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
    PR1 = 100000;
    TMR1 = 0;
    T1CONbits.TCKPS = 3;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 2;
    T1CONbits.ON = 1;
    //Switch Initialization
    TRISDbits.TRISD6 = 1;
    CNCONDbits.ON = 1;
    CNENDbits.CNIED6 = 1;
    CNPUDbits.CNPUD6 = 1;
    IFS1bits.CNDIF = 0;
    IPC8bits.CNIP = 4;
    IPC8bits.CNIS = 3;
    IEC1bits.CNDIE = 1;
    while(1) {
        switch(stat) {
            case led1:
                LATDbits.LATD0 = 1;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                break;
            case led2:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 1;
                break;
            case led3:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 1;
                LATDbits.LATD2 = 0;
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

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4SRS) _CNInterrupt( void ){
    IFS1bits.CNDIF = 0;
    if(PORTDbits.RD6 == 0) {
        TMR1 = 0;
    }
    if(PORTDbits.RD6 == 1) {
        if(TMR1 < 1228) {
            if(stat == led1) stat = led2;
            else if(stat == led2) stat = led3;
            else if(stat == led3) stat = led1;
        }
        else {
            if(stat == led1) stat = led3;
            else if(stat == led2) stat = led1;
            else if(stat == led3) stat = led2;
        }
    }
}