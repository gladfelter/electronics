#include <stdint.h>
#include <xc.h>

#include "serial_pic16.h"

void interrupt ISR(void)
{
  SerialRxISR();
}