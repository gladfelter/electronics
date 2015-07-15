#include "button_array.h"

const int factor = 0.00;
const int min_window = 3;

void ButtonArray::init(int analogIn) {
  this->analogIn = analogIn;
  analogReference(EXTERNAL); 
  for (idx = 0; idx < 17; idx++) {
    val = int(EEPROM.read(idx * 2));
    val += int(EEPROM.read(idx * 2 + 1)) << 8;
    window = max(int(factor * float(val)), min_window);
    low[idx] = val - window;
    high[idx] = val + window;
  }
}

int ButtonArray::readButton() {
  delay(2);
  val = analogRead(analogIn);
  newVal = -1;
  for (idx = 0; idx < 16; idx++) {
    if (val >= low[idx] && val <= high[idx]) {
      newVal = idx;
      if (lastVal == idx) {
        repeats += 1;
      } else if (repeats / (++misses) < 3 || lastVal < 0) {
        repeats = 1;
        misses = 0;
        lastVal = idx;  
      }
      break;
    }
  }
  if (newVal == -1) {
    if (++misses >= 3) {
      lastVal = -1;
    }
  }
  if (lastVal > 0) {
    return repeats == 10 ? lastVal : -2;
  } else {
    return lastVal - 1;
  }
}
