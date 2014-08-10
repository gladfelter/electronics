/*
  Battery Discharge Monitor
  
  by David Gladfelter, 9/10/2013
  ---------------------------------------------------------------------
  
  This program uses an LCD & Keypad Shield and a custom circuit that discharges a rechargable
  battery and measures and logs the power dissipated over time. It can then upload the
  logged data as a CSV to a host computer via RS-232.demonstrates button detection, LCD text/number printing,
  and LCD backlight control on the Freetronics LCD & Keypad Shield, connected to an Arduino board.
*/
/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <Wire.h>
#include <LiquidCrystal.h>   // include LCD library
#include <stdio.h>
/*--------------------------------------------------------------------------------------
  Defines
--------------------------------------------------------------------------------------*/
// Pins in use
#define BUTTON_ADC_PIN           A0  // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN         10  // D3 controls LCD backlight
#define BATT_MEAS_PIN            A2
#define MOSFET_CONTROL_PIN        2
// ADC readings expected for the 5 buttons on the ADC input
#define RIGHT_10BIT_ADC          20  // right
#define UP_10BIT_ADC            130  // up
#define DOWN_10BIT_ADC          306  // down
#define LEFT_10BIT_ADC          480  // left
#define SELECT_10BIT_ADC        720  // select
#define BUTTONHYSTERESIS         10  // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE               0  // 
#define BUTTON_RIGHT              1  // 
#define BUTTON_UP                 2  // 
#define BUTTON_DOWN               3  // 
#define BUTTON_LEFT               4  // 
#define BUTTON_SELECT             5  // 
//some example macros with friendly labels for LCD backlight/pin control, tested and can be swapped into the example code as you like
#define LCD_BACKLIGHT_OFF()     digitalWrite(LCD_BACKLIGHT_PIN, LOW)
#define LCD_BACKLIGHT_ON()      digitalWrite(LCD_BACKLIGHT_PIN, HIGH)
#define LCD_BACKLIGHT(state)    { if(state){digitalWrite(LCD_BACKLIGHT_PIN, HIGH);}else{digitalWrite(LCD_BACKLIGHT_PIN, LOW);} }

#define VOLTAGE_OFFSET          0.00
#define RESIST                  2.3 // ohms
#define MINIMUM_NIMH_VOLT       byte(1024 * 0.8 / 5.0 + VOLTAGE_OFFSET * 1024 / 5.0)
#define MILLISECONDS_PER_SAMPLE 10000
#define BACKLIGHT_TMO           30000

#define BL_DISABLED             0
#define BL_ENABLED              1
#define BL_TIMED_OUT            2

#define DEBUGLN(x) { Serial.println(x); Serial.flush(); }
#define DEBUG(x)   Serial.print(x);
/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

byte voltageReadings[100] = { 64 };
int nextReadingPos = 0;
int lastSecs = 0;
unsigned long startTime;
unsigned long nextReadingTime;
unsigned long lastButtonPress = 0;
boolean discharge = false;
int backlight = BL_DISABLED;
/*--------------------------------------------------------------------------------------
  Init the LCD library with the LCD pins to be used
--------------------------------------------------------------------------------------*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);   //Pins for the freetronics 16x2 LCD shield. LCD: (RS, E, LCD-D4, LCD-D5, LCD-D6, LCD-D7)
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
--------------------------------------------------------------------------------------*/
void setup() {
  //button adc input
   Serial.begin(115200);

   pinMode(BUTTON_ADC_PIN, INPUT);         //ensure A0 is an input
   pinMode(LCD_BACKLIGHT_PIN, OUTPUT);     //D3 is an output
   pinMode(MOSFET_CONTROL_PIN, OUTPUT);
   digitalWrite(BUTTON_ADC_PIN, LOW);      //ensure pullup is off on A0
   SetBacklight(BL_ENABLED);
   //set up the LCD number of columns and rows: 
   lcd.begin(16, 2);
   PrintLcdHeaders();
   SetStandby();
   startTime = millis();
}

void PrintLcdHeaders() {
   lcd.setCursor(4, 0);
   lcd.print("V");
   lcd.setCursor(5, 0);
   lcd.setCursor(10, 0);
   lcd.print("A");
   lcd.setCursor(12, 0);
   lcd.print("Mode");
   lcd.setCursor(6, 1);
   lcd.print("Secs");
}  

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
--------------------------------------------------------------------------------------*/
void loop() {
   byte button;
   byte timestamp;
   
   unsigned int battVoltage;
   unsigned long now = millis() - startTime;
   int currSecs = now / 1000;
   battVoltage = analogRead(BATT_MEAS_PIN);
   button = ReadButtons();
   if (discharge) {
     LogDischarge(now, battVoltage);
   }

   if (currSecs != lastSecs) {
     char buf[12]; // "-2147483648\0"
     lcd.setCursor(0, 1);
     lcd.print(itoa(currSecs, buf, 10));   
     lcd.setCursor(0, 0);
     float fVoltage = CalculateVoltage(battVoltage);
     lcd.print(dtostrf(fVoltage, 1, 2, buf));
     lcd.setCursor(6, 0);
     if (discharge) {
       lcd.print(dtostrf(fVoltage / RESIST, 1, 2, buf));
     } else {
       lcd.print("0.00");
     }
     lcd.setCursor(0, 1);
     lcd.print(itoa(currSecs, buf, 10));   
   }
   lastSecs = currSecs;
   if (buttonJustPressed) {
     switch(button) {
        case BUTTON_NONE: {
           break;
        }
        case BUTTON_RIGHT: {
          SetDischarge();
          break;
        }
        case BUTTON_UP: {
          UploadReadings();
          break;
        }
        case BUTTON_DOWN: {
          SetBacklight(backlight == BL_ENABLED ? LOW : HIGH);
          break;
        }
        case BUTTON_LEFT: {
          SetStandby();
          break;
        }
        case BUTTON_SELECT: {
           break;
        }
        default: {
           break;
        }
     }
     lastButtonPress = now;
     if (backlight == BL_TIMED_OUT) {
       SetBacklight(BL_ENABLED);
     }
   } else if (lastButtonPress + BACKLIGHT_TMO < now) {
     SetBacklight(BL_TIMED_OUT);
   }
   buttonJustPressed = false;
   buttonJustReleased = false;
}

void SetBacklight(int state) {
  digitalWrite(LCD_BACKLIGHT_PIN, state == BL_ENABLED ? HIGH : LOW);
  backlight = state;
}

float CalculateVoltage(int voltage) {
  return ((float)voltage / (float) 1024) * 5.0 - VOLTAGE_OFFSET;
}

void SetDischarge() {
  DEBUGLN("Entering discharge");

  if (!discharge) {
    startTime = millis();
    DEBUG("Start time is ");
    DEBUGLN(startTime);
    nextReadingPos = 0;
    nextReadingTime = startTime;
  }
  discharge = true;
  lcd.setCursor(11, 1);
  lcd.print("Dscrg");
  digitalWrite(MOSFET_CONTROL_PIN, HIGH);
}

void SetStandby() {
  DEBUGLN("Entering standby");

  if (discharge) {
    voltageReadings[nextReadingPos++] = 0;
  }
  discharge = false;
  lcd.setCursor(11, 1);
  lcd.print(" Stby");
  digitalWrite(MOSFET_CONTROL_PIN, LOW);
}

void LogDischarge(unsigned long now, int battVoltage) {
  if (now > nextReadingTime) {
    DEBUG("Logging a reading of ");
    DEBUGLN(battVoltage);

    nextReadingTime += MILLISECONDS_PER_SAMPLE;
    voltageReadings[nextReadingPos++] = (byte) (battVoltage / 4);

    // Stop discharging when the battery has reached its minimum safe voltage
    if (battVoltage < MINIMUM_NIMH_VOLT) {
      DEBUGLN("Stopping discharge due to undervolt");
      SetStandby();
    }
  }
}

void UploadReadings() {
  if (Serial) {
    lcd.setCursor(6, 1);
    lcd.print(" Uploading");

    Serial.print("nextReadingPos:");
    Serial.println(nextReadingPos);
    PrintHeader();

    for (int x = 0; x < nextReadingPos; x++) {
      if (voltageReadings[x] == 0) {
        PrintHeader();
      } else {
        float fVoltage = voltageReadings[x] / (float) 256 * 5.0 - VOLTAGE_OFFSET;
        float fAmp = fVoltage / RESIST;
        int time = x * 10;
        Serial.print(time);
        Serial.print(",");
        Serial.print(fVoltage);
        Serial.print(",");
        Serial.println(fAmp);
      }
    }
    Serial.flush();
    lcd.setCursor(6, 1);
    lcd.print("Secs       ");
  }
}

void PrintHeader() {
  Serial.println("seconds,volts,amps");
}

/*--------------------------------------------------------------------------------------
  ReadButtons()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
--------------------------------------------------------------------------------------*/
byte ReadButtons() {
   unsigned int buttonVoltage;
   byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn
   
   buttonVoltage = analogRead(BUTTON_ADC_PIN);
   //read the button ADC pin voltage
   char buf[12]; // "-2147483648\0"
   //sense if the voltage falls within valid voltage windows
   if(buttonVoltage < (RIGHT_10BIT_ADC + BUTTONHYSTERESIS)) {
      button = BUTTON_RIGHT;
   } else if(  buttonVoltage >= (UP_10BIT_ADC - BUTTONHYSTERESIS)
           && buttonVoltage <= (UP_10BIT_ADC + BUTTONHYSTERESIS)) {
      button = BUTTON_UP;
   } else if(  buttonVoltage >= (DOWN_10BIT_ADC - BUTTONHYSTERESIS)
           && buttonVoltage <= (DOWN_10BIT_ADC + BUTTONHYSTERESIS)) {
      button = BUTTON_DOWN;
   } else if(  buttonVoltage >= (LEFT_10BIT_ADC - BUTTONHYSTERESIS)
           && buttonVoltage <= (LEFT_10BIT_ADC + BUTTONHYSTERESIS)) {
      button = BUTTON_LEFT;
   } else if(  buttonVoltage >= (SELECT_10BIT_ADC - BUTTONHYSTERESIS)
           && buttonVoltage <= (SELECT_10BIT_ADC + BUTTONHYSTERESIS)) {
      button = BUTTON_SELECT;
   }
   //handle button flags for just pressed and just released events
   if((buttonWas == BUTTON_NONE) && (button != BUTTON_NONE)) {
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
      buttonJustPressed  = true;
      buttonJustReleased = false;
   } else if((buttonWas != BUTTON_NONE) && (button == BUTTON_NONE)) {
      buttonJustPressed  = false;
      buttonJustReleased = true;
   }
   
   //save the latest button value, for change event detection next time round
   buttonWas = button;
   
   return(button);
}

