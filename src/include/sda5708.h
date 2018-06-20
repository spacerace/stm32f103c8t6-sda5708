#ifndef __SDA5708_H__
#define __SDA5708_H__

#include "stm32f10x.h"

/* Make it portable, only 4 IO pins must be defined +
 * some functions to control pins:
 *  SDA5708_RESET_H()
 *  SDA5708_RESET_L()
 *  SDA5708_DATA_H()
 *  SDA5708_DATA_L()
 *  SDA5708_CLOCK_H()
 *  SDA5708_CLOCK_L()
 *  SDA5708_LOAD_H()
 *  SDA5708_LOAD_L()
 *
 * If your CPU is very fast (>72MHz) maybe you'll need a delay:
 *  sda5708_delay(int ticks)
 * This function is located in sda5708.c. If you dont need delay
 * just leave this function empty.
 *
 * Also sda5708_init() must be changed (pin directions).
 *
 */ 
#define SDA5708_PIN_LOAD		GPIO_Pin_0
#define SDA5708_PIN_DATA		GPIO_Pin_1
#define SDA5708_PIN_CLOCK		GPIO_Pin_2
#define SDA5708_PIN_RESET		GPIO_Pin_3
#define SDA5708_PORT				GPIOA

#define SDA5708_RESET_H()		GPIO_SetBits(SDA5708_PORT, SDA5708_PIN_RESET);
#define SDA5708_DATA_H()		GPIO_SetBits(SDA5708_PORT, SDA5708_PIN_DATA);
#define SDA5708_CLOCK_H()		GPIO_SetBits(SDA5708_PORT, SDA5708_PIN_CLOCK);
#define SDA5708_LOAD_H()		GPIO_SetBits(SDA5708_PORT, SDA5708_PIN_LOAD);

#define SDA5708_RESET_L()		GPIO_ResetBits(SDA5708_PORT, SDA5708_PIN_RESET);
#define SDA5708_DATA_L()		GPIO_ResetBits(SDA5708_PORT, SDA5708_PIN_DATA);
#define SDA5708_CLOCK_L()		GPIO_ResetBits(SDA5708_PORT, SDA5708_PIN_CLOCK);
#define SDA5708_LOAD_L()		GPIO_ResetBits(SDA5708_PORT, SDA5708_PIN_LOAD);

/* prototypes */
void sda5708_init(void);															// init display
void sda5708_reset_pulse(void);												// reset display
void sda5708_wrbyte(uint8_t data);										// send one byte with soft SPI
void sda5708_wrpattern(int col, uint8_t pattern[7]);	// send one character
void sda5708_put_dig(uint8_t col, uint8_t num);				// put a digit (0-9)
void sda5708_clr_col(uint8_t col);										// clear a digit
void sda5708_clr(void);																// clear whole display
void sda5708_set_cursor(int col);											// set cursor to 0-7
void sda5708_put_char_at(uint8_t col, uint8_t c);			// put a character (ASCII)
void sda5708_putc(uint8_t c);													// print a character
void sda5708_puts(char *s);														// print a string
void sda5708_set_brightness(int br);
#define init_sda5708	sda5708_init

/* registers */
#define SDA5708_REG_CONTROL			0xC0
#define SDA5708_DISP_NORMALOP		0x20
#define SDA5708_CURRENT_LIMIT		0x08

#define SDA5708_REG_ADDRESS			0xA0
#define SDA5708_DIGIT0					0x00
#define SDA5708_DIGIT1					0x01
#define SDA5708_DIGIT2					0x02
#define SDA5708_DIGIT3					0x03
#define SDA5708_DIGIT4					0x04
#define SDA5708_DIGIT5					0x05
#define SDA5708_DIGIT6					0x06
#define SDA5708_DIGIT7					0x07

#endif // __SDA5708_H__
