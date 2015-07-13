#include <EEPROM.h>

int idx;
int val;
int low[16];
int high[16];
const float factor = .000;
const int min_window = 3;
int window;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL); 
  for (idx = 1; idx < 17; idx++) {
    Serial.print("idx = ");
    Serial.print(idx - 1);
    val = int(EEPROM.read(idx * 2));
    val += int(EEPROM.read(idx * 2 + 1)) << 8;
    Serial.print("; cal val = ");
    Serial.print(val);
    window = max(int(factor * float(val)), min_window);
    Serial.print("; window = ");
    Serial.print(window);
    low[idx - 1] = val - window;
    high[idx - 1] = val + window;

    Serial.print("; low = ");
    Serial.print(low[idx - 1]);
    Serial.print("; high = ");
    Serial.println(high[idx - 1]);
  }
}

int lastVal = -1;
int newVal, misses;
int repeats = 0;

void loop() {
  val = analogRead(analogInPin);
  newVal = -1;
  for (idx = 0; idx < 16; idx++) {
    if (val >= low[idx] && val <= high[idx]) {
      newVal = idx;
      if (lastVal == idx) {
        if (++repeats == 10) {
          Serial.print("Button ");
          Serial.print(idx);
          Serial.println(" pressed.");
        }
      } else if (lastVal >= 0 && ++misses < 3) {
        
      } else {
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
  delay(2);
}
