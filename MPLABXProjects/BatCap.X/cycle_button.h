/* 
 * File:   cycle_button.h
 * Author: David
 *
 * Created on August 24, 2014, 11:36 PM
 */

#ifndef CYCLE_BUTTON_H
#define	CYCLE_BUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Call this from the system ISR to handle cycle button interupts.
 */
extern void CycleButtonISR();

/*
 * Initializes the Cycle button's interrupts, etc.
 *
 * @param cycleNotify callback when a cycle has been requested, with boolean
 *     indicating if it was a double-click. Called in the context of an ISR.
 * @param wakeupNotify callback when a spurious click has been registered.
 *     Called in the context of an ISR.
 * @param getMillis callback requesting milliseconds since system some fixed
 *     date. Called in the context of an ISR.
 */
void CycleButtonInit(void (*cycleNotify)(unsigned char),
    void (*wakeupNotify)(), int (*getMillis)());

#ifdef	__cplusplus
}
#endif

#endif	/* CYCLE_BUTTON_H */

