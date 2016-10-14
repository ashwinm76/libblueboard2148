/*
 * target.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#include "LPC214x.h"
#include "target.h"
#include "gpio.h"

unsigned long cclk_period_ns;

void target_init(struct target_config cfg)
{
  cclk_period_ns = 1000000000/cfg.fcclk;

  // Interrupt vector mapping.
  MEMMAP = cfg.vector_location;

  // Set the VPB divider.
  switch(cfg.pclk_div)
  {
    case 1 :
      VPBDIV = 1; break;

    case 2 :
      VPBDIV = 2; break;

    default: /* Fpclk = Fcclk/4 */
      VPBDIV = 0; break;
  }

  // Enable the PLL if required.
  PLLCON = cfg.enable_pll ? 1 : 0;

  // Set up the PLL.
  if (cfg.enable_pll)
  {
    switch(cfg.cco_mul)
    {
      case 2:
        PLLCFG = ((cfg.fcclk / Fosc) - 1) | (0 << 5);
        break;

      case 4:
        PLLCFG = ((cfg.fcclk / Fosc) - 1) | (1 << 5);
        break;

      case 8:
        PLLCFG = ((cfg.fcclk / Fosc) - 1) | (2 << 5);
        break;

      case 16:
        PLLCFG = ((cfg.fcclk / Fosc) - 1) | (3 << 5);
        break;
    }

    PLLFEED = 0xaa;
    PLLFEED = 0x55;

    // Wait for the PLL to lock.
    while((PLLSTAT & (1 << 10)) == 0)
    {
    }

    // Connect the PLL.
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
  }

  // Disable the MAM.
  MAMCR = 0;

  // Set up and enable the MAM if required.
  if (cfg.enable_mam)
  {
    // Set the MAM timing.
    if (cfg.fcclk < 20000000)
    {
    MAMTIM = 1;
    }
    else if (cfg.fcclk < 40000000)
    {
      MAMTIM = 2;
    }
    else
    {
      MAMTIM = 3;
    }

    // Enable the MAM.
    MAMCR = 2;
  }
}
