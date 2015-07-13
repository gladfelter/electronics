/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 */
#include "Statistic.h"
#include <EEPROM.h>

Statistic stats;
boolean isReading = false;

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int nextEepromIdx = 0;

void setup() {
  analogReference(EXTERNAL);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // read the analog in value:
  if (!digitalRead(2)) {
    if (!isReading) {
      isReading = true;
    } else if (stats.count() < 100){
      stats.add(analogRead(analogInPin));
    }
  } else {
    if (isReading) {
      Serial.print("min = ");
      Serial.print(stats.minimum());
      Serial.print("; max = ");
      Serial.print(stats.maximum());
      Serial.print("; avg = ");
      Serial.print(stats.average());
      Serial.print("; intavg = ");
      Serial.print(int(stats.average()));
      Serial.print("; low = ");
      Serial.print(0x00ff & int(stats.average()));
      Serial.print("; high = ");
      Serial.print(0x00ff & (int(stats.average()) >> 8));
      Serial.print("; stddev = ");
      Serial.println(stats.pop_stdev());
      EEPROM.write(nextEepromIdx++, 0x00ff & int(stats.average()));
      EEPROM.write(nextEepromIdx++, 0x00ff & (int(stats.average()) >> 8));
      stats.clear();
      isReading = false;
      delay(500);
    }
  }
  delay(2);                     
}
