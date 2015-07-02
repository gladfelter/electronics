/* Unipolar Stepper control with an L293D
 * 
 * The L293D chip has 16 pins. Here is how each of the pins should
 * be connected:
 * 
 * Pin 1, 9 Enable pins. Enable pin DIO on controller
 * Pin 3: Coil 1A (Blue wire)
 * Pin 6: Coil 1B (Pink wire)
 * Pin 11: Coil 2A (Yellow wire)
 * Pin 14: Coil 2B (Orange wire)
 * Pin 4, 5, 12, 13: Ground
 * Pin 8 Motor voltage: 5-12V.
 * Pin 16: +5V, isolated from motor power.
 * Pin 2: Phase 1A control (motor pin 1)
 * Pin 7: Phase 1B control (motor pin 2)
 * Pin 10: Phase 2A control (motor pin 3)
 * Pin 15: Phase 2B control (motor pin 4)
 */

int motorPin1 = 3;
int motorPin2 = 4;
int motorPin3 = 5;
int motorPin4 = 6;
int enablePin = 7;
int ledPin = 13;
int delayTime = 5;

int cycle[] = { 3, 6, 12, 9 };

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(13, OUTPUT);
}

int x = 0;
int cyclepos = 0;

void loop() {
  if (x++ % 2036 == 0) {
    x = 1;
    digitalWrite(enablePin, LOW);
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }
  digitalWrite(enablePin, HIGH);
  digitalWrite(motorPin1, cycle[cyclepos] & 1);
  digitalWrite(motorPin2, cycle[cyclepos] & 2);
  digitalWrite(motorPin3, cycle[cyclepos] & 4);
  digitalWrite(motorPin4, cycle[cyclepos] & 8);
  delay(delayTime);
  cyclepos = (cyclepos + 1) % 4;
}
