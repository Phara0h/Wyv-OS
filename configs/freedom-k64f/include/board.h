/************************************************************************************
 * configs/freedom-k64f/include/board.h
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __CONFIG_FREEDOM_K64F_INCLUDE_BOARD_H
#define __CONFIG_FREEDOM_K64F_INCLUDE_BOARD_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#ifndef __ASSEMBLY__
# include <stdint.h>
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Clocking *************************************************************************/
/* The Freedom K64F uses a 50MHz external clock from the Micrel Ethernet PHY.  The
 * Kinetis MCU startup from an internal digitally-controlled oscillator (DCO). Nuttx
 * will enable the main external oscillator (EXTAL0/XTAL0).   The external
 * oscillator/resonator can range from 32.768 KHz up to 50 MHz. The default external
 * source for the MCG oscillator inputs (EXTAL) is 50 MHz clock source from Micrel
 * Ethernet PHY.
 */

#define BOARD_EXTCLOCK       1              /* External clock */
#define BOARD_EXTAL_FREQ     48000000       /* 50MHz Oscillator */
#define BOARD_XTAL32_FREQ    32768          /* 32KHz RTC Oscillator */

/* PLL Configuration.  Either the external clock or crystal frequency is used to
 * select the PRDIV value. Only reference clock frequencies are supported that will
 * produce a 2MHz reference clock to the PLL.
 *
 *   PLL Input frequency:   PLLIN  = REFCLK/PRDIV = 50MHz/25 = 2MHz
 *   PLL Output frequency:  PLLOUT = PLLIN*VDIV   = 2Mhz*48 = 96MHz
 *   MCG Frequency:         PLLOUT = 96MHz
 */

#define BOARD_PRDIV          19             /* PLL External Reference Divider */
#define BOARD_VDIV           24             /* PLL VCO Divider (frequency multiplier) */

#define BOARD_PLLIN_FREQ     (BOARD_EXTAL_FREQ / BOARD_PRDIV)
#define BOARD_PLLOUT_FREQ    (BOARD_PLLIN_FREQ * BOARD_VDIV)
#define BOARD_MCG_FREQ       BOARD_PLLOUT_FREQ

/* SIM CLKDIV1 dividers */

#define BOARD_OUTDIV1        1              /* Core        = MCG, 96MHz */
#define BOARD_OUTDIV2        2              /* Bus         = MCG/2, 48MHz */
#define BOARD_OUTDIV3        2              /* FlexBus     = MCG/2, 48MHz */
#define BOARD_OUTDIV4        4              /* Flash clock = MCG/4, 24MHz */

#define BOARD_CORECLK_FREQ  (BOARD_MCG_FREQ / BOARD_OUTDIV1)
#define BOARD_BUS_FREQ      (BOARD_MCG_FREQ / BOARD_OUTDIV2)
#define BOARD_FLEXBUS_FREQ  (BOARD_MCG_FREQ / BOARD_OUTDIV3)
#define BOARD_FLASHCLK_FREQ (BOARD_MCG_FREQ / BOARD_OUTDIV4)

/* SDHC clocking ********************************************************************/

/* SDCLK configurations corresponding to various modes of operation.   Formula is:
 *
 *   SDCLK  frequency = (base clock) / (prescaler * divisor)
 *
 * The SDHC module is always configure configured so that the core clock is the base
 * clock.
 */

/* Identification mode:  400KHz = 96MHz / ( 16 * 15) */

#define BOARD_SDHC_IDMODE_PRESCALER    SDHC_SYSCTL_SDCLKFS_DIV16
#define BOARD_SDHC_IDMODE_DIVISOR      SDHC_SYSCTL_DVS_DIV(15)

/* MMC normal mode: 16MHz  = 96MHz / (2 * 3) */

#define BOARD_SDHC_MMCMODE_PRESCALER   SDHC_SYSCTL_SDCLKFS_DIV2
#define BOARD_SDHC_MMCMODE_DIVISOR     SDHC_SYSCTL_DVS_DIV(3)

/* SD normal mode (1-bit): 16MHz  = 96MHz / (2 * 3) */

#define BOARD_SDHC_SD1MODE_PRESCALER   SDHC_SYSCTL_SDCLKFS_DIV2
#define BOARD_SDHC_SD1MODE_DIVISOR     SDHC_SYSCTL_DVS_DIV(3)

/* SD normal mode (4-bit): 24MHz  = 96MHz / (2 * 2) (with DMA)
 * SD normal mode (4-bit): 16MHz  = 96MHz / (2 * 3) (no DMA)
 */

#ifdef CONFIG_SDIO_DMA
#  define BOARD_SDHC_SD4MODE_PRESCALER SDHC_SYSCTL_SDCLKFS_DIV2
#  define BOARD_SDHC_SD4MODE_DIVISOR   SDHC_SYSCTL_DVS_DIV(2)
#else
//#  define BOARD_SDHC_SD4MODE_PRESCALER SDHC_SYSCTL_SDCLKFS_DIV2
//#  define BOARD_SDHC_SD4MODE_DIVISOR   SDHC_SYSCTL_DVS_DIV(3)
#  define BOARD_SDHC_SD4MODE_PRESCALER SDHC_SYSCTL_SDCLKFS_DIV16
#  define BOARD_SDHC_SD4MODE_DIVISOR   SDHC_SYSCTL_DVS_DIV(15)
#endif

/* LED definitions ******************************************************************/
/* The Freedom K64F has a single RGB LED driven by the K64F as follows:
 *
 *   LED    K64
 *   ------ -------------------------------------------------------
 *   RED    PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT
 *   BLUE   PTB21/SPI2_SCK/FB_AD30/CMP1_OUT
 *   GREEN  PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN
 *
 * If CONFIG_ARCH_LEDS is not defined, then the user can control the LEDs in any
 * way.  The following definitions are used to access individual LEDs.
 */

/* LED index values for use with board_userled() */

#define BOARD_LED_R       0
#define BOARD_LED_G       1
#define BOARD_LED_B       2
#define BOARD_NLEDS       3

/* LED bits for use with board_userled_all() */

#define BOARD_LED_R_BIT   (1 << BOARD_LED_R)
#define BOARD_LED_G_BIT   (1 << BOARD_LED_G)
#define BOARD_LED_B_BIT   (1 << BOARD_LED_B)

/* If CONFIG_ARCH_LEDs is defined, then NuttX will control the LED on board
 * the Freedom K64F.  The following definitions describe how NuttX controls
 * the LEDs:
 *
 *   SYMBOL                Meaning                 LED state
 *                                                 RED   GREEN  BLUE
 *   -------------------  -----------------------  -----------------   */
#define LED_STARTED       1 /* NuttX has been started    OFF  OFF  OFF */
#define LED_HEAPALLOCATE  2 /* Heap has been allocated   OFF  OFF  ON  */
#define LED_IRQSENABLED   0 /* Interrupts enabled        OFF  OFF  ON  */
#define LED_STACKCREATED  3 /* Idle stack created        OFF  ON   OFF */
#define LED_INIRQ         0 /* In an interrupt          (no change)    */
#define LED_SIGNAL        0 /* In a signal handler      (no change)    */
#define LED_ASSERTION     0 /* An assertion failed      (no change)    */
#define LED_PANIC         4 /* The system has crashed    FLASH OFF OFF */
#undef  LED_IDLE            /* K64 is in sleep mode     (Not used)     */

/* Button definitions ***************************************************************/
/* Two push buttons, SW2 and SW3, are available on FRDM-K64F board, where SW2 is
 * connected to PTC6 and SW3 is connected to PTA4. Besides the general purpose
 * input/output functions, SW2 and SW3 can be low-power wake up signal. Also, only
 * SW3 can be a non-maskable interrupt.
 *
 *   Switch    GPIO Function
 *   --------- ---------------------------------------------------------------
 *   SW2       PTC6/SPI0_SOUT/PD0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK/LLWU_P10
 *   SW3       PTA4/FTM0_CH1/NMI_b/LLWU_P3
 */

#define BUTTON_SW2        0
#define BUTTON_SW3        1

#define BUTTON_SW2_BIT    (1 << BUTTON_WAKEUP)
#define BUTTON_SW3_BIT    (1 << BUTTON_TAMPER)

/* Alternative pin resolution *******************************************************/
/* If there are alternative configurations for various pins in the
 * kinetis_k64pinmux.h header file, those alternative pins will be labeled with a
 * suffix like _1, _2, etc.  The logic in this file must select the correct pin
 * configuration for the board by defining a pin configuration (with no suffix) that
 * maps to the correct alternative.
 */

/* The primary serial port interface signals are PTB16 UART1_RX and PTB17 UART1_TX.
 * These signals are connected to the OpenSDAv2 circuit.
 *
 * REVISIT: The above quoate was taken from the "FRDM-K64F Freedom Module User’s
 * Guide". It appears from the "K64 Sub-Family Reference Manual" that the only
 * options for UART1 are {PTC3, PTE1} for UART1_RX and {PTC4, PTE0} for UART1_TX.
 */

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: kinetis_boardinitialize
 *
 * Description:
 *   All STM32 architectures must provide the following entry point.  This entry point
 *   is called early in the intitialization -- after all memory has been configured
 *   and mapped but before any devices have been initialized.
 *
 ************************************************************************************/

void kinetis_boardinitialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif  /* __CONFIG_FREEDOM_K64F_INCLUDE_BOARD_H */