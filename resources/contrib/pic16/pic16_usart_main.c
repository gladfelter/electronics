/******************************************************************************

 The most basic example program to write a line of text in to the terminal
 using the USART library for pic16 mcus.

 Compiler: Microchip XC8 v1.12 (http://www.microchip.com/xc)
 IDE: Microchip MPLABX

 MCU: PIC16F877A
 Frequency: 20MHz

                                     NOTICE

NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com

*******************************************************************************/
#include <xc.h>

#include "usart_pic16.h"

// Configuration Byte
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void main()
{
  //Initialize USART with baud rate 9600
  USARTInit(9600);

  //Write intro text
  USARTWriteLine("***********************************************");
  USARTWriteLine("USART Test");
  USARTWriteLine("----------");
  USARTWriteLine("Type a character on keyboard");
  USARTWriteLine("it will reach the PIC MCU via the serial line");
  USARTWriteLine("PIC MCU will return the same character but ");
  USARTWriteLine("enclosed in a <>");
  USARTWriteLine("--");
  USARTWriteLine("For example if you type 'a' you will see <a>");
  USARTWriteLine("appear on serial terminal.");
  USARTWriteLine(" ");
  USARTWriteLine("This checks both way serial transfers.");
  USARTWriteLine("");
  USARTWriteLine("Copyright (C) 2008-2013");
  USARTWriteLine("www.eXtremeElectronics.co.in");
  USARTWriteLine("***********************************************");
  USARTGotoNewLine();
  USARTGotoNewLine();

  while(1)
  {
      //Get the amount of data waiting in USART queue
      uint8_t n= USARTDataAvailable();

      //If we have some data
      if(n!=0)
      {
          //Read it
          char data=USARTReadData();

          //And send back
          USARTWriteChar('<');
          USARTWriteChar(data);
          USARTWriteChar('>');
      }

  }
}
