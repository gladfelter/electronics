
int greenPin = 9;
int redPin = 10;
int bluePin = 11;
int pins[] = {greenPin, redPin, bluePin};

void setup()  { 
  // nothing happens in setup 
} 

void loop()  { 
  for (int currentPin = 0; currentPin < sizeof(pins) / sizeof(int); currentPin++) {
    // fade in from min to max in increments of 5 points:
    for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
      // sets the value (range from 0 to 255):
      analogWrite(pins[currentPin], fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                           
    } 
    for(int fadeValue = 255; fadeValue >= 0; fadeValue -=5) { 
      // sets the value (range from 0 to 255):
      analogWrite(pins[currentPin], fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                           
    } 
  }
}


