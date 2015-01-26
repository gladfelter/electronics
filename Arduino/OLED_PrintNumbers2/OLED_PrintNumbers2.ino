#include <Wire.h>
#include <OzOLED.h>


void setup(){

	OzOled.init();  //initialze OLED display

	OzOled.clearDisplay();           //clear the screen and set start position to top left corner
	OzOled.setNormalDisplay();       //Set display to Normal mode
	OzOled.setPageMode();            //Set addressing mode to Page Mode


	OzOled.printNumber((long)-12345, 3, 0);

}

void loop(){
  
}


