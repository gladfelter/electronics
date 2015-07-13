#include <EEPROM.h>

int idx;
int val;
int cal;

void setup() {
  Serial.begin(9600); 
  for (idx = 0; idx < 17; idx++) {
    val = int(EEPROM.read(idx * 2));
    val += int(EEPROM.read(idx * 2 + 1)) << 8;
    Serial.print("idx = ");
    Serial.print(idx);
    Serial.print("; little = ");
    Serial.print(EEPROM.read(idx * 2));
    Serial.print("; big = ");
    Serial.print(EEPROM.read(idx * 2 + 1));

    Serial.print("; val = ");
    Serial.println(val);
  }
}

void loop() {
}
