#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

int digits[][7] = {{4, 5, 6, 7, 8, 9, 0}, {7, 4, 0, 0, 0, 0, 0}, {3, 5, 6, 7, 9, 0, 0},
                   {3, 4, 5, 7, 9, 0, 0}, {3, 4, 7, 8, 0, 0, 0}, {3, 4, 5, 8, 9, 0, 0},
                   {3, 5, 6, 8, 9, 0, 0}, {4, 7, 9, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 8, 9},
                   {3, 4, 7, 8, 9, 0, 0}};

unsigned short codes[] = {0x00ff6897, 0x00ff30cf, 0x00ff18e7, 0x00ff7a85, 0x00ff5aa5,
                          0x00ff10ef, 0x00ff38c7, 0x00ff42bd, 0x00ff4abd, 0x00ff52ad}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(10, OUTPUT);
  for (int x = 1; x < 10; x++) {
    pinMode(x, OUTPUT);
  }
  digitalWrite(10, HIGH);
}
byte lastPin = 0;
byte digit = -1;
void loop()
{
  pinMode(lastPin, INPUT);
  lastPin = ((lastPin + 1) % 8) + 2;
  pinMode(lastPin, OUTPUT);
  digitalWrite(lastPin, HIGH);
  if (irrecv.decode(&results)) {
     Serial.println(results.value, HEX);
     irrecv.resume(); // Receive the next value
  }
}
