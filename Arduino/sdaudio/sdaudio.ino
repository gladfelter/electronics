/**
 * Example of transcoding a wav:
 * mplayer -ao pcm:waveheader:file=/tmp/output.wav \
 *     -vo null -vc null \
 *     --af=format=u8,channels=1,resample=11200:0:0 \
 *     /media/david/3947-50C2/concentrate.wav 
 */

#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
#include <EEPROM.h>
#include "button_array.h"
TMRpcm tmrpcm;   // create an object for use in this sketch
ButtonArray buttons;
char c;
int x;

char *filenames[] = {
  "candy.wav",
  "computer.wav",
  "concent.wav",
  "crazy.wav",
  "dangrs.wav",
  "ecky.wav",
  "exct3.wav",
  "excex.wav",
  "extempor.wav",
  "fanfare2.wav",
  "holy_g.wav",
  "ice_c.wav",
  "laser_x.wav",
  "urine.wav",
  "warning.wav",
  "war_room.wav",
};

char cmds[] = {
  'a',
  'm',
  'o',
  'c',
  'd',
  'k',
  'x',
  'e',
  'r',
  'f',
  'h',
  'i',
  'l',
  'u',
  'n',
  'w',
};

void writeMenu() {
  for (x = 0; x < sizeof(cmds) / sizeof(char); x++) {
    Serial.write("'");
    Serial.write(cmds[x]);
    Serial.write("'");
    Serial.write(": ");
    Serial.write(filenames[x]);
    Serial.write('\n');
  }
}

void setup(){

  Serial.begin(9600);
  buttons.init(A0);
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc

  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  writeMenu();
}

int nextButton;

void loop(){  
  if (!tmrpcm.isPlaying()) {
    tmrpcm.disable();
  }
  nextButton = buttons.readButton();
  if (nextButton >= 0) {
    Serial.print("Button ");
    Serial.print(nextButton);
    Serial.println(" pressed");
    tmrpcm.play(filenames[nextButton]);
  } else if(Serial.available()){   
    c = Serial.read();
    Serial.write(c);
    Serial.write('\n');
    for (x = 0; x < sizeof(cmds) / sizeof(char); x++) {
      if (cmds[x] == c) {
        Serial.write("Playing ");
        Serial.write(filenames[x]);
        Serial.write('\n');
        tmrpcm.play(filenames[x]);
        writeMenu();
        break;
      }
    }
    if (!tmrpcm.isPlaying()) {
      Serial.write("Could not play\n");
    }
  }
}
