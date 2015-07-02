
int greenPin = 9;
int redPin = 10;
int bluePin = 11;
int pins[] = {greenPin, redPin, bluePin};

void setup()  { 
  // nothing happens in setup 
} 

void loop()  {
 
  for(int fadeValue = 0 ; fadeValue <= 510; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(greenPin, fadeValue > 255 ? 510 - fadeValue : fadeValue);         
    delay(30);
    for(int fadeValue = 0 ; fadeValue <= 510; fadeValue +=5) { 
      analogWrite(bluePin, fadeValue > 255 ? 510 - fadeValue : fadeValue);         
      delay(30);
      for(int fadeValue = 0 ; fadeValue <= 510; fadeValue +=5) { 
        analogWrite(redPin, fadeValue > 255 ? 510 - fadeValue : fadeValue);         
        delay(30);                           
      }
    }
  } 
}


