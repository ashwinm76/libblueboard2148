/*
 * txtlcd.h
 *
 *  Created on: Oct 12, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_TXTLCD_H_
#define INCLUDE_TXTLCD_H_

void txtlcd_init();
void txtlcd_putchar(char c);
void txtlcd_puts(char *s);
void txtlcd_backlight_on();
void txtlcd_backlight_off();
void txtlcd_gotoxy(int row, int col);
void txtlcd_cls();

#endif /* INCLUDE_TXTLCD_H_ */
