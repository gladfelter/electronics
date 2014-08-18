/* 
 * File:   main.c
 * Author: David
 *
 * Created on August 16, 2014, 3:33 PM
 */

// PIC16F1825 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "serial_pic16.h"
#include "lcd_hd44780_pic16.h"

#define IRCF_OSC_16MHZ 0b1111
#define INPUT 1
#define OUTPUT 0

volatile char serialBuffer[64];
volatile char *serialBufferPtr = serialBuffer;

void waitOnOscillatorStable() {
  while (!HFIOFS) {
  }
}

/*
 * Port Assignments:
 *
 *   LCD:
 *     C0-C3: 4-bit data bus
 *     A4   : Register Select
 *     A5   : Read/Write
 *     C4   : Backlight Control
 *
 *   Serial:
 *     A0   : TX
 *     A1   : RX
 *
 *   Discharge MOSFET Gate: C5
 *
 *   Reset: A3
 *
 *   Voltage Measurement: A2/AN2
 */
void main() {
  OSCCONbits.IRCF = IRCF_OSC_16MHZ;
  
  // Configure Serial port directions, type
  TRISA0 = OUTPUT;
  TRISA1 = INPUT;
  ANSELA &= ~0b11;

  // Configure backlight control
  TRISC4 = OUTPUT;
  ANSELC &= ~(1<<4);
  RC4 = 1;

  waitOnOscillatorStable();
  SerialInit(9600, &serialBufferPtr, 64);
  SerialWriteLine("Awake!");

  LCDInit(LS_NONE);
  LCDClear();
  LCDWriteString("Awake!");
  while (1) {
  }
}
