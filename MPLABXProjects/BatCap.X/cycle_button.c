#include <xc.h>
#include "cycle_button.h"

void (*__cycleNotify)(unsigned char doubleClick) = 0;
void (*__wakeupNotify)() = 0;
int (*__getMillis)() = 0;

volatile int now;
int __lastEdge;

void CycleButtonInit(void (*cycleNotify)(unsigned char),
    void (*wakeupNotify)(), int (*getMillis)()) {
  __cycleNotify = cycleNotify;
  __wakeupNotify = wakeupNotify;
  __getMillis = getMillis;
  __lastEdge = -1;
  INTCONbits.IOCIE = 1;
  IOCAN3 = 1;
}

void CycleButtonISR() {
  if (INTCONbits.IOCIF) {
    if (IOCAF3) {
      IOCAF &= ~0x8;
      now = __getMillis();
      if (now - __lastEdge > 100) {
        if (now - __lastEdge > 1000) {
          __cycleNotify(0);
        } else {
          __cycleNotify(1);
        }
      } else {
        __wakeupNotify();
      }
      __lastEdge = now;
    }
  }
}