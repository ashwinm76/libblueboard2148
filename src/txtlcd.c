/*
 * txtlcd.c
 *
 *  Created on: Oct 12, 2016
 *      Author: Ashwin Menon
 */

#include "gpio.h"
#include "delay.h"

// Port 0 pins
#define LCD_D4 10
#define LCD_D5 11
#define LCD_D6 12
#define LCD_D7 13
#define DATA_MASK ((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7))

// Port 1 pins
#define LCD_BL 21
#define LCD_EN 22
#define LCD_RW 23
#define LCD_RS 24

static void set_data_outputs()
{
  gpio0_set_dir_fast32(gpio0_get_dir_fast32() | DATA_MASK);
}

static void set_data_inputs()
{
  gpio0_set_dir_fast32(gpio0_get_dir_fast32() & ~DATA_MASK);
}

static void wait_busy()
{
  int busy = 1;

  set_data_inputs();

  // RS=0, RW=1
  gpio1_set_fast32(1 << LCD_RW);
  gpio1_clr_fast32(1 << LCD_RS);

  while(busy)
  {
    // EN=1
    gpio1_set_fast32(1 << LCD_EN);

    delay_us(1);

    // Read the flags and address counter
    busy = (gpio0_read_fast32() >> LCD_D7) & 1;

    // EN=0
    gpio1_clr_fast32(1 << LCD_EN);

    delay_us(1);

    // EN=1
    gpio1_set_fast32(1 << LCD_EN);

    delay_us(1);

    // EN=0
    gpio1_clr_fast32(1 << LCD_EN);

    delay_us(1);
  }
}

static void write_nibble(int nibble)
{
  set_data_outputs();

  // EN=1
  gpio1_set_fast32(1 << LCD_EN);

  // Write nibble
  gpio0_clr_fast32(DATA_MASK);
  gpio0_set_fast32(nibble << LCD_D4);

  delay_us(1);

  // EN=0
  gpio1_clr_fast32(1 << LCD_EN);

  delay_us(1);
}

static void write_command_4bit(int cmd, int check_busy, int upper_nibble_only)
{
  if (check_busy)
  {
    wait_busy();
  }

  // RS=0, RW=0
  gpio1_clr_fast32((1 << LCD_RS) | (1 << LCD_RW));

  // Write upper nibble
  write_nibble((cmd & 0xf0) >> 4);

  if (!upper_nibble_only)
  {
    write_nibble(cmd & 0xf);
  }
}

static void write_data_4bit(int cmd, int check_busy)
{
  if (check_busy)
  {
    wait_busy();
  }

  // RS=1, RW=0
  gpio1_set_fast32(1 << LCD_RS);
  gpio1_clr_fast32(1 << LCD_RW);

  // Write upper nibble
  write_nibble((cmd & 0xf0) >> 4);

  // Write lower nibble
  write_nibble(cmd & 0xf);
}

void txtlcd_init()
{
  unsigned long ctrl_mask;

  // Set up port 0 pin directions
  set_data_inputs();

  // Set up port 1 pin directions and initial value
  ctrl_mask = (1 << LCD_BL) | (1 << LCD_EN) | (1 << LCD_RW) | (1 << LCD_RS);
  gpio1_clr_fast32(ctrl_mask);
  gpio1_set_dir_fast32(gpio1_get_dir_fast32() | ctrl_mask);

  // LCD reset sequence
  delay_us(100000);
  write_command_4bit(0x30, 0, 1);
  delay_us(5000);
  write_command_4bit(0x30, 0, 1);
  delay_us(200);
  write_command_4bit(0x30, 0, 1);
  delay_us(5000);
  write_command_4bit(0x20, 0, 1);
  delay_us(5000);

  // 4-bit interface, 2 lines, 5x8 character matrix
  write_command_4bit(0x28, 0, 0);
  delay_us(5000);

  // Display on, no cursor, no cursor position blink
  write_command_4bit(0x0c, 1, 0);

  // Cursor moves to the right, no display shift
  write_command_4bit(0x06, 1, 0);

  // Display clear
  write_command_4bit(0x1, 1, 0);
}

void txtlcd_putchar(char c)
{
  write_data_4bit(c, 1);
}

void txtlcd_puts(char *s)
{
  while(*s)
  {
    write_data_4bit(*s, 1);
    s++;
  }
}

void txtlcd_backlight_on()
{
  gpio1_set_fast32(1 << LCD_BL);
}

void txtlcd_backlight_off()
{
  gpio1_clr_fast32(1 << LCD_BL);
}

void txtlcd_gotoxy(int row, int col)
{
  if ((col < 0) || (col > 15))
  {
    return;
  }

  if (row == 0)
  {
    write_command_4bit(0x80 + col, 1, 0);
  }
  else if (row == 1)
  {
    write_command_4bit(0xc0 + col, 1, 0);
  }
}

void txtlcd_cls()
{
  write_command_4bit(0x1, 1, 0);
}
