/* 
 * File:   timing.h
 * Author: David
 *
 * Created on August 31, 2014, 6:48 PM
 */

#ifndef TIMING_H
#define	TIMING_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "frequency.h"

volatile int timingBaseMilliseconds = 0;


#define TIMING_FREQ = 1000
// Number of machine instructions per ISR before timer is reset
#define INTERRUPT_INSTRUCTIONS_EST = 10
// 4 clock cycles per instruction
#define INTERRUPT_OVERHEAD_EST ((4 * INTERRUPT_INSTRUCTIONS_EST) / (float) _XTAL_FREQ)
#define LFINTOSC_FREQ 31000
#define WDT_PRESCALER 131072
#define WDT_PRESCALER_BITS 0b1100
#define WDT_INTERVAL_MS ((WDT_PRESCALER * 1000) / LFINTOSC_FREQ)

void TimingInit() {
    // Set 4s watchdog interval, turned off
    WDTCON = WDT_PRESCALER_BITS << 1;
    TMR6IE = 1;

    int postscaler = (int) floor(1 / (float)TIMING_FREQ * (float)_XTAL_FREQ) / 256;
    int prescaler = 1;
    if (postscaler / 16 > 0) {
        if (postscaler / 64 > 0) {
            T6CON = 0b11;
            prescaler = 64;
            postscaler /= 64;
        } else {
            T6CON = 0b10;
            prescaler = 16;
            postscaler /= 16;
        }
    }
    // ensure there's headroom to down-tune the PR register.
    postscaler++;
    if (postscaler > 16) {
        postscaler = 16;
    }
    T6CON |= (postscaler - 1) << 3;

    double timingFrequency = (float) _XTAL_FREQ / (float) (postscaler * prescaler) / 256;
    timingFrequency -= INTERRUPT_OVERHEAD_EST * (float) postscaler * (float) prescaler * 256;
    PR6 = (unsigned char) floor(256.0 / 1000.0 * timingFrequency);
    TMR6IF = 0;
    TMR6ON = 1;
}

void TimingSleep() {
    WDTCON = WDTCON | 0x01;
    SLEEP();
}

int TimingGetMilliseconds() {
    TMR6IE = 0;
    int result = timingBaseMilliseconds;
    TMR6IE = 1;
    return result;
}

void TimingISR() {
    if (TMR6IF == 1) {
        timingBaseMilliseconds++;
        TMR6IF = 0;
    } else if (STATUSbits.nTO == 0 && STATUSbits.nPD == 0) {
        WDTCON = WDTCON & ~0x01;
        timingBaseMilliseconds += WDT_INTERVAL_MS;
    }
}

#ifdef	__cplusplus
}
#endif

#endif	/* TIMING_H */

