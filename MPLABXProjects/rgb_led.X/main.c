/* 
 * File:   main.c
 * Author: david
 *
 * Created on February 3, 2015, 7:14 PM
 */
#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)



#define IRCF_OSC_8MHZ 0b1110001

void waitOnOscillatorStable() {
  while (!OSCCONbits.HTS) {
  }
}
char i, v;
void main (void)
{
  OSCCON = IRCF_OSC_8MHZ;
  waitOnOscillatorStable();
  TRISIO = 0b101000;
  WDTCON = 0b10100;
  OPTION_REGbits.PS = 0;
  
  while(1)
  {
      for (i = 0; i < 15; i++) {
          if (i & 0b1000) {
              v = (i & ~0b1000) + 0b10000;
          } else {
              v = i;
          }
          GPIO = v;
          CLRWDT();
          WDTCONbits.SWDTEN = 1;
          SLEEP();
          WDTCONbits.SWDTEN = 0;
      }
  }
}