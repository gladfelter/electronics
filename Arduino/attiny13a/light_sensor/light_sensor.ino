/**
 * light_sensor
 *
 * Senses light with a photoresistor voltage divider on an anlog input
 * and blinks an LED with intensity and duty cycle relative to light
 * intensity.
 */

// LED on Pin 5 of the Attiny13a
#define LED 0
// Sensor on Pin 7 of the Attiny13a
#define SENSOR A1

int duty = 512;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
  analogReference(DEFAULT);
}


void loop() {
  duty = max(0, 255 - analogRead(SENSOR) / 4 - 20);
  analogWrite(LED, duty);
  delay(500);
  analogWrite(LED, 0);
  delay(1024 - duty * 4);
}
