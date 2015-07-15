#ifndef button_array_h
#define button_array_h

#include <Arduino.h>
#include <EEPROM.h>

class ButtonArray {
public:
  /**
   * Initializes the calibration values from Eeprom memory
   */
  void init(int analogInPin);

  /**
   * Reads the next button.
   *
   * @return -2 for unknown button, -1 for no button, 0-15 for button.
   */
  int readButton();
private:
  int _lastVal;
  int _newVal;
  int _misses;
  int _repeats;
  int _analogInPin;
  int _window;
  int _idx;
  int _val;
  int _low[16];
  int _high[16];
};
#endif

