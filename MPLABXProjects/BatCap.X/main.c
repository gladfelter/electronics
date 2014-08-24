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
#include <stdlib.h>
#include <stdio.h>

#include "frequency.h"
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
 *     C4   : Enable
 *     A4   : Register Select
 *     A5   : Read/Write
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

  RXDTSEL = 1; // RX is on RA1
  TXCKSEL = 1; // TX is on RA0

  ANSELC &= ~0b011111;
  ANSELA &= ~0b110011; // A0-1 are not analog
  
  // Use TTL signaling on Digital I/O
  //INLVLA = 0;
  //INLVLC = 0;

  waitOnOscillatorStable();
  __delay_ms(40);

  SerialInit(9600, &serialBufferPtr, 64);
//  short x = 0;
//  char buffer[10];
//  for (x = 390; x < 450; x++) {
//    SPBRG = x;
//    SerialPuts("Talking at ");
//    itoa(buffer, x, 10);
//    SerialPuts(buffer);
//    SerialPuts("\r\n");
//  }
  //SPBRG = 421;
  int coeff = SPBRG;
  SPBRG = 421;
  printf("Testing Serial communication at %d\r\nThe algorithm guessed %d\r\n", SPBRG, coeff);
  SerialWriteLine("Awake!");

  LCDInit(LS_NONE);
  //LCDClear();
  //LCDWriteString("Awake!");
  while (1) {
  }
}

extern void putch(char c);

void putch(char c) {
  SerialPutCh(c);
}