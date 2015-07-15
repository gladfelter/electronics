#ifndef button_array_h
#define button_array_h
#include <Arduino.h>
#include <EEPROM.h>
class ButtonArray {
public:
  /**
   * Initializes the calibration values from Eeprom memory
   */
  void init(int analogIn);
  
  /**
   * Reads the next button.
   *
   * @return -2 for unknown button, -1 for no button, 0-15 for button.
   */
  int readButton();
private:
  int analogIn;
  int window;
  int idx;
  int val;
  int low[16];
  int high[16];
  int lastVal = -1;
  int newVal;
  int misses;
  int repeats = 0;
};
#endif
