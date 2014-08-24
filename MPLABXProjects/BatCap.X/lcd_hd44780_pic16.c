#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include "frequency.h"
#include "lcd_hd44780_pic16.h"

#define LCD_DATA_PORT 	PORT(LCD_DATA)
#define LCD_DATA_TRIS 	TRIS(LCD_DATA)
#define LCD_DATA_LATCH 	LATCH(LCD_DATA)

#define LCD_E_PORT      PORT(LCD_E_PORTNAME)
#define LCD_E_LATCH     LATCH(LCD_E_PORTNAME)
#define LCD_E           ((LCD_E_PORT >> LCD_E_POS) & 0x01)
#define LCD_E_TRIS      TRISBIT(LCD_E_PORTNAME, LCD_E_POS)
#define LCD_E_LATCHBIT  (((LCD_E_LATCH) >> LCD_E_POS) & 0x01)

#define LCD_RS_PORT     PORT(LCD_RS_PORTNAME)
#define LCD_RS_LATCH    LATCH(LCD_RS_PORTNAME)
#define LCD_RS          ((LCD_RS_PORT >> LCD_RS_POS) & 0x01)
#define LCD_RS_TRIS     TRISBIT(LCD_RS_PORTNAME, LCD_RS_POS)
#define LCD_RS_LATCHBIT (((LCD_RS_LATCH) >> LCD_RS_POS) & 0x01)

#define LCD_RW_PORT     PORT(LCD_RW_PORTNAME)
#define LCD_RW_LATCH    LATCH(LCD_RW_PORTNAME)
#define LCD_RW          ((LCD_RW_PORT >> LCD_RW_POS) & 0x01)
#define LCD_RW_TRIS     TRISBIT(LCD_RW_PORTNAME, LCD_RW_POS)
#define LCD_RW_LATCHBIT (((LCD_RW_LATCH) >> LCD_RW_POS) & 0x01)

#define SET_E()  (LCD_E_PORT = LCD_E_LATCH | (0x01 << LCD_E_POS))
#define SET_RS() (LCD_RS_PORT = LCD_RS_LATCH | (0x01 << LCD_RS_POS))
#define SET_RW() (LCD_RW_PORT = LCD_RW_LATCH | (0x01 << LCD_RW_POS))

#define CLEAR_E()  (LCD_E_PORT = LCD_E_LATCH & ~(0x01 << LCD_E_POS))
#define CLEAR_RS() (LCD_RS_PORT = LCD_RS_LATCH & ~(0x01 << LCD_RS_POS))
#define CLEAR_RW() (LCD_RW_PORT = LCD_RW_LATCH & ~(0x01 << LCD_RW_POS))

#ifdef LCD_TYPE_162
#define LCD_TYPE_204
#endif

#ifdef LCD_TYPE_202
#define LCD_TYPE_204
#endif

void LCDWriteNibble(char data) {
  char portTmp = (LCD_DATA_LATCH &
      ~(0x0F << LCD_DATA_POS)) | ((data) << LCD_DATA_POS);
#ifdef LCD_DEBUG
  printf("writing 0x%x\r\n", portTmp);
#endif
  LCD_DATA_PORT = portTmp;
  CLEAR_E();
  __delay_us(1);
#ifdef LCD_DEBUG
  if (LCD_E) {
    printf("ERROR: LCD Enable didn't go low at start of pulse\r\n");
  }
#endif
  SET_E();
  __delay_us(1);
#ifdef LCD_DEBUG
  printf("LCD Port Value: 0x%x\r\n", LCD_DATA_PORT);
  printf("LCD Latch Value: 0x%x\r\n", LCD_DATA_LATCH);
  printf("LCD Enable/RS/RW Values: %d/%d/%d\r\n", LCD_E, LCD_RS, LCD_RW);
  printf("LCD Enable/RS/RW Latches: %d/%d/%d\r\n", LCD_E_LATCHBIT,
      LCD_RS_LATCHBIT, LCD_RW_LATCHBIT);
  if (!LCD_E) {
    printf("ERROR: LCD Enable didn't go high\r\n");
  }
#endif
  CLEAR_E();
  __delay_us(100);
#ifdef LCD_DEBUG
  if (LCD_E) {
    printf("ERROR: LCD Enable didn't go low at end of pulse");
  }
#endif
}

void LCDByte(uint8_t c, uint8_t isdata) {
  //Sends a byte to the LCD in 4bit mode
  //cmd=0 for data
  //cmd=1 for command
    if (isdata) {
      SET_RS();
    } else {
      CLEAR_RS();
    }
    CLEAR_RW();
    LCDWriteNibble((c) >> 4);
    LCDWriteNibble((c) & 0x0F);
}

void LCDInit(uint8_t style) {
  __delay_ms(100);

  //Set IO Ports
  LCD_DATA_TRIS &= (~(0x0F << LCD_DATA_POS)); //Output

  LCD_E_TRIS = 0; //Output
  LCD_RS_TRIS = 0; //Output
  LCD_RW_TRIS = 0; //Output

  LCD_DATA_PORT &= (~(0x0F << LCD_DATA_POS)); //Clear data port
  CLEAR_RS();
  CLEAR_RW();
  CLEAR_E();
#ifdef LCD_DEBUG
  printf("TRISA=0x%x\r\n", TRISA);
  printf("TRISC=0x%x\r\n", TRISC);
#endif

  //Set 4-bit mode
#ifdef LCD_DEBUG
  printf("initializing lcd\r\n");
#endif
  LCDWriteNibble(0b0011);
  __delay_us(4500);
  LCDWriteNibble(0b0011);
  __delay_us(4500);
  LCDWriteNibble(0b0011);
  __delay_us(150);
  LCDWriteNibble(0b0010);

  LCDByte(0b00101000, 0); // 4-bit, 2-line, 5x7
  LCDByte(0b00001100, 0); // Display on, cursor/blink off
  LCDByte(0b00000001, 0); // Clear display
  __delay_us(2000);
  LCDByte(0b00000110, 0); // Set text direction

  LCDCmd(0b00001100 | style); //Display on, set style
}

void LCDWriteString(const char *msg) {
  while (*msg != '\0') {
    LCDData(*msg);
    msg++;
  }
}

void LCDGotoXY(uint8_t x, uint8_t y) {
  if (x >= 20 || y > 3) {
    return;
  }
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  x = 0x80 | (x + row_offsets[y]);
  LCDCmd(x);
}