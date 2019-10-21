/*
 * led.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#include "LPC214x.h"
#include "spi.h"
#include "gpio.h"

#define PIN_ST_CP 7

void led_init()
{
  // SSEL0 is used as ST_CP. Make it an output before configuring the SPI as
  // a master, otherwise if the SPI sees the SSEL0 pin asserted as a master then
  // it will signal a mode fault.
  gpio0_make_output_fast32(PIN_ST_CP);
  gpio0_set_fast32(1 << PIN_ST_CP);

  // Set up SPI:
  spi_init(8, // transfer size
           1, // CPHA
           1, // CPOL
           1, // Master mode
           0, // MSB first transfers
           64 // PCLK divisor
           );
}

void led_write(unsigned long value)
{
  gpio0_clr_fast32(1 << PIN_ST_CP);
  spi_xfer(1, &value, 0);
  gpio0_set_fast32(1 << PIN_ST_CP);
}
