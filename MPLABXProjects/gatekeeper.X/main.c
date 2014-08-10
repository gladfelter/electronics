#include <xc.h>

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <math.h>

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

// #define MYDEBUG

#define OPEN_IR_VOLTS 1
#define VDD 5
#ifdef MYDEBUG
#define GATE_TRIGGER_MEAS 100
#else
#define GATE_TRIGGER_MEAS OPEN_IR_VOLTS * 1024 / VDD
#endif
#define _XTAL_FREQ 4000000
#define WARNING_ON_MS 4000
#ifdef MYDEBUG
#define ADC_SETTLE_US 20
#else
#define ADC_SETTLE_US 20
#endif // MYDEBUG
#define INDICATOR_STATE(state) GPIObits.GP2 = state;
#define DETECTOR_PWR(state) GPIObits.GP5 = state;
#define ON 1
#define OFF 0

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
}

void configureGpio5Digital() {
  CMCON0bits.CM = 0x07; // Set all Comparator outputs to GPIO
  TRISIObits.TRISIO5 = 0; // GPIO5 is output
}

void configureGpio2Digital() {
  CMCON0bits.CM = 0x07; // Set all Comparator outputs to GPIO
  ANSELbits.ANS2 = 0; // AN2/GPIO2 set to digital
  TRISIObits.TRISIO2 = 0; // GPIO2 is output
}

void beginAsyncAn3Measurement() {
  PIR1bits.ADIF = 0; // turn off interrupt bit
  PIE1bits.ADIE = 1; // enable A/D interrupt
  INTCONbits.GIE = 1; // enable global interrupts
}

void configureWatchdog() {
  WDTCONbits.WDTPS = 0x09; // 16184 / 32*10^4 seconds
}

void turnOffComparatorReference() {
  VRCONbits.VREN = 0;
}

short _analogMeasurement;

short analogRead() {
  __delay_us(ADC_SETTLE_US); // let ADC settle
  ADCON0bits.GO = 1; // begin measurement
  while(ADCON0bits.GO_DONE) {
  }
  _analogMeasurement = ADRESH;
  _analogMeasurement = _analogMeasurement << 8;
  _analogMeasurement += ADRESL;
  return _analogMeasurement;
}

short detectorVoltage;
unsigned char highLedState = OFF;

void main() {
  configureOscillator();
  configureWatchdog();
  configureGpio4Analog();
  configureGpio2Digital();
  configureGpio5Digital();
  turnOffComparatorReference();
  while (1) {
    INDICATOR_STATE(OFF);
    DETECTOR_PWR(ON);
    detectorVoltage = analogRead();
    DETECTOR_PWR(OFF)
    if (detectorVoltage > GATE_TRIGGER_MEAS) {
      highLedState = !highLedState;
      INDICATOR_STATE(highLedState);
      CLRWDT();
      OPTION_REGbits.PSA = 1;
      // Prescale WDT by additional 4x for ~4 sec while off
      OPTION_REGbits.PS = highLedState ? 0 : 2;
    } else {
      highLedState = OFF;
      CLRWDT();
      OPTION_REGbits.PSA = 1;
#ifdef MYDEBUG
      OPTION_REGbits.PS = 1;
#else
      OPTION_REGbits.PS = 6; // Prescale WDT by additional 64x for ~32 sec
#endif
    }
    SLEEP();
    NOP();
  }
}
