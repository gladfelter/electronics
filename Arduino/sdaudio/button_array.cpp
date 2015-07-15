#include "button_array.h"

const int factor = 0.00;
const int min__window = 5;

void ButtonArray::init(int analogInPin) {
  _lastVal = -1;
  _repeats = 0;
  this->_analogInPin = analogInPin;
  analogReference(EXTERNAL); 
  for (_idx = 0; _idx < 17; _idx++) {
    Serial.print("idx = ");
    Serial.print(_idx);
    _val = int(EEPROM.read(_idx * 2));
    _val += int(EEPROM.read(_idx * 2 + 1)) << 8;
    Serial.print("; cal val = ");
    Serial.print(_val);
    _window = max(int(factor * float(_val)), min__window);
    Serial.print("; window = ");
    Serial.println(_window);
    _low[_idx] = max(_val - _window, 0);
    _high[_idx] = _val + _window;
    if (_idx > 1) {
      int avg = (_low[_idx - 1] + _high[_idx]) / 2;
      _low[_idx - 1] = avg + 1;
      _high[_idx] = avg;
    } 
  }
  for (_idx = 0; _idx < 17; _idx++) {
    Serial.print("idx = ");
    Serial.print(_idx);
    Serial.print("; low = ");
    Serial.print(_low[_idx]);
    Serial.print("; high = ");
    Serial.println(_high[_idx]);
  }
}

int ButtonArray::readButton() {
  delay(2);
  _val = analogRead(_analogInPin);
  Serial.println(_val);
  _newVal = -1;
  for (_idx = 0; _idx < 16; _idx++) {
    if (_val >= _low[_idx] && _val <= _high[_idx]) {
      _newVal = _idx;
      //Serial.print(_idx);
      //Serial.print("; ");
      //Serial.print(_lastVal);
      //Serial.print("; ");
      //Serial.print(_repeats);
      //Serial.print("; ");
      //Serial.println(_misses);
      if (_lastVal == _idx) {
        _repeats += 1;
      } else {
        //Serial.println("Missed");
        _misses++;
        if (_misses >=  3 || _lastVal < 0) {
          //Serial.println("Resetting");
          _repeats = 1;
          _misses = 0;
          _lastVal = _idx;
        }
      }
      break;
    }
  }
  if (_newVal == -1) {
    //Serial.println("Increasing misses");
    if (++_misses >= 3) {
      _lastVal = -1;
    }
  }
  if (_lastVal > 0) {
    return _repeats == 8 ? _lastVal : -2;
  } 
  else {
    return _lastVal - 1;
  }
}

