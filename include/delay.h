/*
 * delay.h
 *
 *  Created on: Oct 12, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_DELAY_H_
#define INCLUDE_DELAY_H_

// Delay at least 1 CCLK
#define WAIT_CCLK \
  do { \
    asm volatile ("mov r0, r0\n":::); \
    \
  } while(0)

// Delay for a minimum time of time_us microseconds.
void delay_us(unsigned long time_us);

#endif /* INCLUDE_DELAY_H_ */
