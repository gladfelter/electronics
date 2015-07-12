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

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
boolean looped = false;
int readings[100];
int nextReading = -1;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int minval, maxval, sum, x;


void setup() {
  analogReference(EXTERNAL);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // read the analog in value:
  if (!digitalRead(2)) {
    if (nextReading == -1) {
      nextReading = 0;
      looped = false;
    } else {
      sensorValue = analogRead(analogInPin);            
      readings[nextReading] = sensorValue;
      if (nextReading == sizeof(readings) / sizeof(int) - 1) {
        looped = true;
      }
      nextReading = (nextReading + 1) % (sizeof(readings) / sizeof(int));
      // print the results to the serial monitor:
      //Serial.print("sensor = " );                       
      //Serial.println(sensorValue);      
    }
  } else {
    if (nextReading != -1) {
      minval = maxval = readings[0];
      sum = 0;
      for (x = 0; looped ? x < sizeof(readings) / sizeof(int) : nextReading; x++) {
        sum += readings[x];
        minval = minval > readings[x] ? readings[x] : minval;
        maxval = maxval < readings[x] ? readings[x] : maxval;
      }
      Serial.print("min = ");
      Serial.print(minval);
      Serial.print("; max = ");
      Serial.print(maxval);
      Serial.print("; avg = ");
      Serial.println(sum / (looped ? sizeof(readings) / sizeof(int) : nextReading));
      nextReading = -1;
    }
  }
  delay(2);                     
}
