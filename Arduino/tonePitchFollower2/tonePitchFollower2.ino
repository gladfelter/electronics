/*
  Pitch follower
 
 Plays a pitch that changes based on a changing analog input
 
 circuit:
 * 8-ohm speaker on digital pin 8
 * photoresistor on analog 0 to 5V
 * 4.7K resistor on analog 0 to ground
 
 created 21 Jan 2010
 modified 31 May 2012
 by Tom Igoe, with suggestion from Michael Flynn

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone2
 
 */

int tones[] = {
  123,
  131,
  139,
  147,
  156,
  165,
  175,
  185,
  196,
  208,
  220,
  233,
  247,
  262,
  277,
  294,
  311,
  330,
  349,
  370,
  392,
  415,
  440,
  466,
  494,
  523,
  554,
  587,
  622,
  659,
  698,
  740,
  784,
  831,
  880,
  932,
  988,
  1047,
  1109,
  1175,
  1245,
  1319,
  1397,
  1480,
  1568,
  1661,
  1760,
  1865,
  1976,
  2093,
  2217,
  2349,
  2489,
  2637,
  2794,
  2960,
  3136,
  3322,
  3520,
  3729,
  3951,
  4186,
  4435,
  4699,
  4978};

int toneCount = sizeof(tones) / sizeof(int);
int startTone = toneCount / 2;
int lastIndex = startTone;
int closestTone(int startIdx, int tn) {
  int recursionCount = 0;
  while(true) {
    if (startIdx <= 0) {
      return 0;
    } else if (startIdx >= toneCount - 1) {
      return toneCount - 1;
    } else if (tones[startIdx] <= tn && tones[startIdx + 1] >= tn) {
      return tn - tones[startIdx] < tones[startIdx + 1] - tn ? startIdx : startIdx + 1;
    } else if (tones[startIdx] > tn) {
      if (++recursionCount > 7) {
        return startIdx;
      }
      startIdx = startIdx / 2;
      continue;
    } else if (tones[startIdx] < tn) {
      if (++recursionCount > 7) {
        return startIdx;
      }
      startIdx = (startIdx + toneCount) / 2;
      continue;
    }
  }
    return startIdx;
}
  
void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(9600);
}



void loop() {
  // read the sensor:
  int sensorReading = analogRead(A0);
  // print the sensor reading so you know its range
  Serial.println(sensorReading);
  // map the analog input range (in this case, 400 - 1000 from the photoresistor)
  // to the output pitch range (120 - 1500Hz)
  // change the minimum and maximum input numbers below
  // depending on the range your sensor's giving:
  int thisPitch = map(sensorReading, /*400*/ 400, 1023, 123, 1500);
  int requestedIndex = closestTone(startTone, thisPitch);
  int nextDelay = 500;
  if (requestedIndex > lastIndex) {
    nextDelay = nextDelay * 2 / (requestedIndex - lastIndex);
    lastIndex += 1;
  } else if (requestedIndex < lastIndex) {
    nextDelay = nextDelay * 2 / (lastIndex - requestedIndex);
   lastIndex -= 1;
  }
  // play the pitch:
  tone(9, tones[lastIndex], nextDelay);
  delay(nextDelay * 3 / 4);        // delay in between reads for stability
}
