#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Pins for led segments for the digits 0-9. 0 Pin means skip.
byte DIGIT_SEGMENTS[][7] = {{4, 5, 6, 7, 8, 9, 0}, {7, 4, 0, 0, 0, 0, 0}, {3, 5, 6, 7, 9, 0, 0},
                   {3, 4, 5, 7, 9, 0, 0}, {3, 4, 7, 8, 0, 0, 0}, {3, 4, 5, 8, 9, 0, 0},
                   {3, 4, 5, 6, 8, 9, 0}, {4, 7, 9, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 8, 9},
                   {3, 4, 7, 8, 9, 0, 0}};

// IR codes for digits 0-9 for AR-MP3 remote control
unsigned long DIGIT_IR_CODES[] = {0xff6897, 0xff30cf, 0xff18e7, 0xff7a85, 0xff10ef,
                         0xff38c7, 0xff5aa5, 0xff42bd, 0xff4ab5, 0xff52ad};

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

byte lastPin = 0;
byte digit = -1;

void loop()
{
  if (digit >= 0) {
    pinMode(DIGIT_SEGMENTS[digit][lastPin], INPUT);
    lastPin = (lastPin + 1) % 7;
    while(!DIGIT_SEGMENTS[digit][lastPin]) {
      lastPin = (lastPin + 1) % 7;
    }
    pinMode(DIGIT_SEGMENTS[digit][lastPin], OUTPUT);
    digitalWrite(DIGIT_SEGMENTS[digit][lastPin], LOW);
  }
  if (irrecv.decode(&results)) {
     Serial.println(results.value, HEX);
     irrecv.resume(); // Receive the next value
     for (int x = 0; x < 10; x++) {
       if (results.value == DIGIT_IR_CODES[x]) {
         digit = x;
         for (int x = 0; x < 8; x++) {
           pinMode(x + 2, INPUT);
         }
         break;
       }
     }
  }
}
