/*
 *  LiquidCrystal.c
 *
 *  Created on: Aug 5, 2022
 *  Author: Md. Khairul Alam
 */
#include "debug.h"
#include "LiquidCrystal.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

rt_uint16_t data_pins[4] = {D4, D5, D6, D7};
rt_uint8_t row_offsets[4];

void rt_lcd_init(void)
{
    rt_pin_mode(RS, PIN_MODE_OUTPUT);
    rt_pin_mode(EN, PIN_MODE_OUTPUT);
    rt_pin_mode(D4, PIN_MODE_OUTPUT);
    rt_pin_mode(D5, PIN_MODE_OUTPUT);
    rt_pin_mode(D6, PIN_MODE_OUTPUT);
    rt_pin_mode(D7, PIN_MODE_OUTPUT);

    //Now we pull both RS and R/W low to begin commands
    rt_pin_write(RS, PIN_LOW);
    rt_pin_write(EN, PIN_LOW);
    rt_thread_mdelay(15);

    rt_lcd_command(0x03); //4 bit mode
    rt_thread_mdelay(5);

    rt_lcd_command(0x03); //4 bit mode
    rt_thread_mdelay(5);

    rt_lcd_command(0x03); //4 bit mode
    rt_thread_mdelay(5);

    rt_lcd_command(0x02); //4 bit mode
    rt_thread_mdelay(1);

    rt_lcd_command(LCD_FUNCTIONSET | 0x08); //first try
    rt_thread_mdelay(5);

    rt_lcd_command(LCD_FUNCTIONSET | 0x08); //second try
    rt_thread_mdelay(1);

    rt_lcd_command(LCD_FUNCTIONSET | 0x08); //final go
    rt_lcd_command(LCD_FUNCTIONSET | 0x08); //final go

    rt_lcd_command(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF | LCD_DISPLAYCONTROL);

    rt_lcd_command(LCD_CLEARDISPLAY);
    rt_thread_mdelay(2);

    rt_lcd_command(LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT | LCD_ENTRYMODESET);

    rt_lcd_setRowOffsets(0x00, 0x40, 0x00 + 16, 0x40 + 16);
    //rt_hw_us_delay(100);
}

void rt_lcd_command(uint8_t data){

    rt_pin_write(RS, PIN_LOW);
    rt_pin_write(EN, PIN_HIGH);
    rt_uint8_t data_first = data>>4;
    rt_uint8_t data_last = data;
    rt_uint8_t i;
    for (i = 0; i < 4; ++i) {
       rt_pin_write(data_pins[i], (data_first >> i) & 0x01);
    }
    rt_pin_write(EN, PIN_LOW);
    rt_thread_mdelay(1);
    rt_pin_write(EN, PIN_HIGH);
    for (i = 0; i < 4; ++i) {
       rt_pin_write(data_pins[i], (data_last >> i) & 0x01);
    }
    rt_pin_write(EN, PIN_LOW);
    rt_thread_mdelay(1);
}


void rt_lcd_data(uint8_t data){

    rt_pin_write(RS, PIN_HIGH);
    rt_pin_write(EN, PIN_HIGH);
    rt_uint8_t data_first = data>>4;
    rt_uint8_t data_last = data;
    rt_uint8_t i;
    for (i = 0; i < 4; ++i) {
       rt_pin_write(data_pins[i], (data_first >> i) & 0x01);
    }
    rt_pin_write(EN, PIN_LOW);
    rt_thread_mdelay(1);
    rt_pin_write(EN, PIN_HIGH);
    for (i = 0; i < 4; ++i) {
       rt_pin_write(data_pins[i], (data_last >> i) & 0x01);
    }
    rt_pin_write(EN, PIN_LOW);
    rt_thread_mdelay(1);
}

void rt_lcd_print(const char *s)
{
    rt_thread_mdelay(1);
    while(*s){
       rt_lcd_data(*s++);
    }
}

void rt_lcd_clear(void)
{
    rt_lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    rt_thread_mdelay(2);  // this command takes a long time!
}

void rt_lcd_home(void)
{
    rt_lcd_command(LCD_RETURNHOME);  // clear display, set cursor position to zero
    rt_thread_mdelay(2);  // this command takes a long time!
}

void rt_lcd_setCursor(uint8_t col, uint8_t row)
{
  if ( row >= 2 ) {
    row = 1;
  }
  if ( row >= 2 ) {
    row = 1;
  }

  rt_lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}


void rt_lcd_setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
  row_offsets[0] = row0;
  row_offsets[1] = row1;
  row_offsets[2] = row2;
  row_offsets[3] = row3;
}

