#include <stdint.h>
#include <xc.h>

#include "serial_pic16.h"
#include "cycle_button.h"
#include "timing.h"

void interrupt ISR(void)
{
  TimingISR();
  SerialRxISR();
  CycleButtonISR();
}