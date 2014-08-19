#include <xc.h>

#include "frequency.h"
#include "serial_pic16.h"

void _SerialClearBuffer();
char volatile **RxBuff; // USART Receive Buffer
volatile int8_t RxBuffNext;
volatile int8_t RxBuffEnd;
uint8_t RxBufferSize;

void SerialInit(uint16_t baud_rate, char volatile ** rxBuffer,
    uint8_t rxBufferSize) {
  RxBuff = rxBuffer;
  RxBufferSize = rxBufferSize;
  _SerialClearBuffer();
  // 16-bit, high baud rate
  BRG16 = 1;
  BRGH = 1;
  unsigned short coef = (uint8_t)((_XTAL_FREQ
      / ((float)baud_rate * 4.0) - 1) + 0.5);
  SPBRG = coef;

  //TXSTA
  TXSTAbits.TX9 = 0; //8 bit transmission
  TXSTAbits.TXEN = 1; //Transmit enable
  TXSTAbits.SYNC = 0; //Async mode
  TXSTAbits.BRGH = 1; //High speed baud rate

  //RCSTA
  RCSTAbits.SPEN = 1; //Serial port enabled
  RCSTAbits.RX9 = 0; //8 bit mode
  RCSTAbits.CREN = 1; //Enable receive
  RCSTAbits.ADDEN = 0; //Disable address detection

  //Receive interrupt
  RCIE = 1;
  PEIE = 1;

  ei();
}

void SerialPutCh(char ch) {
  while (!PIR1bits.TXIF) {
  }
  TXREG = ch;
}

void SerialPuts(const char *str) {
  while (*str != '\0') {
    SerialPutCh(*str);
    str++;
  }
}

void SerialWriteLine(const char *str) {
  SerialPuts(str);
  SerialPutCh('\r'); //CR
  SerialPutCh('\n'); //LF
}

volatile char rxChar;

bool SerialRxISR() {
  if (RCIE && RCIF) {
    //Read the data
    rxChar = RCREG;

    // Drop a char if out of room
    if (RxBuffEnd == RxBuffNext) {
      RxBuffNext = (RxBuffNext + 1) % RxBufferSize;
    }
    RxBuffEnd = (RxBuffEnd + 1) % RxBufferSize;
    (*RxBuff)[RxBuffEnd] = rxChar;
    if (RxBuffNext == -1) {
      RxBuffNext = 0;
    }
    // Swallow and clear any receive overrun errors
    if (RCSTAbits.CREN) {
      RCSTAbits.CREN = 0;
    }
    return true;
  }
  return false;
}

void _SerialClearBuffer() {
  RxBuffEnd = 0;
  RxBuffNext = -1;
}

char SerialGetCh() {
  // Disable Receive Interrupt to prevent a race condition via ISR re-entrancy
  RCIE = 0;

  //Check if buffer is empty
  if (RxBuffNext == -1) {
    RCIE = 1;
    return 0;
  }

  char nextChar = (*RxBuff)[RxBuffNext];
  
  RxBuffNext = (RxBuffNext + 1) % RxBufferSize;
  if (RxBuffNext == RxBuffEnd) {
    _SerialClearBuffer();
  }
  // Enable Receive Interrupt. I believe RCIF could be set by now (in practice
  // this is unlikely.) If I read the datasheet correctly that would cause
  // the ISR to be almost immediately executed.
  RCIE = 1;
  return nextChar;
}

uint8_t SerialDataAvailable() {
  uint8_t result;
  // Disable Receive Interrupt to prevent a race condition via ISR re-entrancy
  RCIE = 0;

  if (RxBuffNext == -1) {
    result = 0;
  }
  else if (RxBuffNext < RxBuffEnd) {
    result = (RxBuffEnd - RxBuffNext + 1);
  }
  else if (RxBuffNext > RxBuffEnd) {
    result = (RxBufferSize - RxBuffNext + RxBuffEnd + 1);
  } else {
    result = 1;
  }
  
  RCIE = 1;
  
  return result;
}

void SerialRead(char *buff, uint16_t len) {
  uint16_t i;
  for (i = 0; i < len; i++) {
    buff[i] = SerialGetCh();
  }
}

void SerialFlushBuffer() {
  // Disable Receive Interrupt to prevent a race condition via ISR re-entrancy
  RCIE = 0;
  _SerialClearBuffer();
  // Re-enable interrupts
  RCIE = 1;
}