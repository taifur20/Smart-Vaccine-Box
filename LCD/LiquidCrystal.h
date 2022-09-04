/*
 *  LiquidCrystal.h
 *
 *  Created on: Aug 5, 2022
 *  Author: Md. Khairul Alam
 */

#define RS 36 //B1
#define EN 35 //B0
#define D4 51 //B12
#define D5 54 //B15
#define D6 53 //B14
#define D7 52 //B13

#ifndef LCD_LIQUIDCRYSTAL_H_
#define LCD_LIQUIDCRYSTAL_H_

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void rt_lcd_init(void);
void rt_lcd_command(uint8_t data);
void rt_lcd_data(uint8_t data);
void rt_lcd_print(const char *s);
void rt_lcd_clear(void);
void rt_lcd_home(void);
void rt_lcd_setCursor(uint8_t col, uint8_t row);
void rt_lcd_setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3);

#endif /* LCD_LIQUIDCRYSTAL_H_ */
