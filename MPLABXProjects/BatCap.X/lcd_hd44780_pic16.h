
#ifndef LCD_HD44780_PIC16_H
#define	LCD_HD44780_PIC16_H

#include <xc.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

//#define LCD_DEBUG

#define CONCAT(x, y) x##y
#define PORT(x)  CONCAT(PORT, x)
#define TRIS(x)  CONCAT(TRIS, x)
#define LATCH(x) CONCAT(LAT, x)

#define BIT_CONCAT(type, port, pos) type##port##bits.##type##port##pos
#define TRISBIT(port, pos)  BIT_CONCAT(TRIS, port, pos)
#define PORTBIT_CONCAT(port, pos) R##port##pos
#define PORTBIT(port, pos)  PORTBIT_CONCAT(port, pos)
#define LATCHBIT(port, pos) BIT_CONCAT(LAT, port, pos)

/************************************************
	LCD CONNECTIONS
*************************************************/

//LCD Data Port
//Port PD0-PD3 are connected to C0-C3
#define LCD_DATA        C   //Port PD0-PD3 are connected to C0-C3
#define LCD_DATA_POS    0

//Register Select (RS)
//RS is connected to Port A bit 4
#define LCD_RS_PORTNAME     A
#define LCD_RS_POS      4

//Read/Write (RW)
//RW is connected to Port A bit 5
#define LCD_RW_PORTNAME     A
#define LCD_RW_POS      5

//Enable signal (E)
//E is connected to Port C bit 4
#define LCD_E_PORTNAME      C
#define LCD_E_POS       4

/***********************************************

LCD Type Selection
Uncomment Just one of them

************************************************/

//#define LCD_TYPE_202	//For 20 Chars by 2 lines

//#define LCD_TYPE_204	//For 20 Chars by 4 lines

#define LCD_TYPE_162	//For 16 Chars by 2 lines

//#define LCD_TYPE_164	//For 16 Chars by 4 lines

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE	 0B00000000

void LCDInit(uint8_t style);
void LCDWriteString(const char *msg);
void LCDGotoXY(uint8_t x,uint8_t y);

void LCDByte(uint8_t,uint8_t);

#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))
#define LCDClear() LCDCmd(0b00000001)
#define LCDHome()  LCDCmd(0b00000010)

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HD44780_PIC16_H */

