/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
#define _XTAL_FREQ 4000000
#define ADC_SETTLE_US 20
/* i.e. uint8_t <variable_name>; */

void configureOscillator() {
  OSCCONbits.IRCF = 0x06; // 4 MHz
  OSCCONbits.SCS = 1; // Use IRCF to choose internal clock rate
}

void configureGpio4Analog() {
  ADCON0bits.VCFG = 0; // Use VDD for analog compare
  ADCON0bits.ADFM = 1; // Analog result is right-justified
  ADCON0bits.CHS = 0x03; // Use AN3 for analog measurement
  // Analog In on AN3
  TRISIObits.TRISIO4 = 1; // Input mode on AN3/GP4
  ANSELbits.ANS3 = 0; // Set AN3/GP4 Pin to Digital (Simulator wants this)
  ANSELbits.ANS3 = 1; // Set AN3/GP4 Pin to Analog
  ADCON0bits.ADON = 1; // Enable ADC
  WPU = 0;
  //WPU &= ~0x16;
}

short analogRead() {
  __delay_us(ADC_SETTLE_US); // let ADC settle
  ADCON0bits.GO = 1; // begin measurement
  while(ADCON0bits.GO_DONE) {
  }
  short _analogMeasurement = ADRESH;
  _analogMeasurement = _analogMeasurement << 8;
  _analogMeasurement += ADRESL;
  return _analogMeasurement;
}
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    configureOscillator();
    CMCON0bits.CM = 0x07; // Set all Comparator outputs to GPIO
    configureGpio4Analog();
    TRISIO = 0x39;
    GPIObits.GP1 = 0x0;
    GPIObits.GP2 = 0x1;
    short reading;
    bool isHigh = 0;
    while(1) {
      GPIO = 0x04;
      reading = analogRead();
      if (reading > 100) {
        GPIO = 0x06;
      }
      __delay_ms(100);
    }
}

