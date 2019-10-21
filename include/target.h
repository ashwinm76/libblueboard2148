/*
 * target.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_TARGET_H_
#define INCLUDE_TARGET_H_

// Crystal frequency in Hz.
#define Fosc  12000000

// This is calculated by target_init().
extern unsigned long cclk_period_ns;

enum vector_location
{
  BOOTLOADER = 0,
  FLASH = 1,
  RAM = 2
};

struct target_config
{
  enum vector_location vector_location;
  int enable_pll;
  int enable_mam;
  unsigned long fcclk;
  unsigned long cco_mul;
  unsigned long pclk_div;
};

void target_init(struct target_config cfg);

#endif /* INCLUDE_TARGET_H_ */
