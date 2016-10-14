/*
 * delay.c
 *
 *  Created on: Oct 12, 2016
 *      Author: Ashwin Menon
 */

#include "target.h"

void delay_us(unsigned long time_us)
{
  unsigned long count = ((1000 * time_us)/(4 * cclk_period_ns));
  asm volatile (
      "mov r0, %0 \n\t"
      "%=: subs r0, r0, #1 \n\t"
      "bhi %=b \n\t"
      :
      : "r" (count)
      : "r0","cc"
      );
}
