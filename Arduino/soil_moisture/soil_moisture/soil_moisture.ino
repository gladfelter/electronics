/**
 * Soil Moisture Sensor Monitor
 *
 * Hardware setup:
 *   - Connect soil sensor Vcc to +5v, GND to GND
 *   - Connect soil sensor AO (analog output) to A0 on Arduino
 *   - Do not connect the soil sensor DO (digital out)
 *
 * Reads the soil sensor analog out and writes the 1024 analog value to the serial port
 *
 * Notes:
 *   - Digital out is probably high when the moisture goes above a level calibrated by the
 *     on-sensor-board potentiometer.
 */

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0); Serial.println(sensorValue);
  delay(100);
}
