#include "stm32f10x.h"
#include "sda5708.h"

/* prototypes, local used functions */
static void sda5708_delay(int ticks);

/* maybe you'll need to implement a small delay on a very fast CPU.
 *if you have a very very fast cpu, enable some time-eating of 50nS or more */
static void sda5708_delay(int ticks) {
	/* systick_delay(1); */
	return;
}

/* full blown charset, takes 2k of codespace */
static const uint8_t charset[256][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x00
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x01
	{ 0x0e, 0x1f, 0x15, 0x1f, 0x15, 0x1b, 0x0e },   // 0x02 ghost 1
	{ 0x0e, 0x1f, 0x15, 0x1f, 0x15, 0x11, 0x0e },   // 0x03 ghost 2
	{ 0x00, 0x0a, 0x1f, 0x1f, 0x0e, 0x04, 0x00 },   // 0x04 heart
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x05
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x06
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x07
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x08
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x09
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0a
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0b
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0c
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0d
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0e
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x0f
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x10
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x11
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x12
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x13
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x14
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x15
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x16
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x17
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x18
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x19
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1A
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1B
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1C
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1d
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1e
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x1f
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },	// 0x20	Space
	{ 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04 },	// 0x21	!
	{ 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 },	// 0x22 "
	{ 0x0A, 0x0A, 0x1f, 0x0A, 0x1F, 0x0A, 0x0A },	// 0x23 #
	{ 0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04 },	// 0x24	$
	{ 0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03 },	// 0x25 %
	{ 0x00, 0x08, 0x14, 0x08, 0x15, 0x12, 0x0D },	// 0x26 &
	{ 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },	// 0x27 '
	{ 0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02 },	// 0x28 (
	{ 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08 },	// 0x29 )
	{ 0x00, 0x15, 0x0e, 0x04, 0x0e, 0x15, 0x00 },	// 0x2A *
	{ 0x00, 0x04, 0x04, 0xff, 0x04, 0x04, 0x00 },	// 0x2B +
	{ 0x00, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x08 },	// 0x2C ,
	{ 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00 },	// 0x2D -
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c },	// 0x2E .
	{ 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00 },	// 0x2F /
        { 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e },   // 0x30 0
        { 0x02, 0x06, 0x0A, 0x02, 0x02, 0x02, 0x02 },   // 0x31 1
        { 0x0e, 0x11, 0x11, 0x02, 0x04, 0x08, 0x1f },   // 0x32 2
        { 0x0e, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0e },   // 0x33 3
        { 0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02 },   // 0x34 4
        { 0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e },   // 0x35 5
        { 0x0e, 0x11, 0x10, 0x1e, 0x11, 0x11, 0x0e },   // 0x36 6
        { 0x1f, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04 },   // 0x37 7
        { 0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e },   // 0x38 8
        { 0x0e, 0x11, 0x11, 0x0f, 0x01, 0x11, 0x0e },   // 0x39 9
	{ 0x00, 0x0C, 0x0c, 0x00, 0x0c, 0x0c, 0x00 },	// 0x3A :
	{ 0x00, 0x0c, 0x0C, 0x00, 0x0C, 0x04, 0x08 },	// 0x3B	;
	{ 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02 },	// 0x3C <
	{ 0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00 },	// 0x3D =
	{ 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08 },	// 0x3E >
	{ 0x0E, 0x11, 0x02, 0x04, 0x04, 0x00, 0x04 },	// 0x3F ?
	{ 0x0e, 0x11, 0x15, 0x17, 0x10, 0x11, 0x0e },	// 0x40 @
	{ 0x04, 0x0A, 0x11, 0x1f, 0x11, 0x11, 0x11 },	// 0x41 A
	{ 0x1E, 0x09, 0x09, 0x0e, 0x09, 0x09, 0x1e },	// 0x42 B
	{ 0x0e, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0e },	// 0x43 C
	{ 0x1E, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1e },	// 0x44 D
	{ 0x1F, 0x10, 0x10, 0x1C, 0x10, 0x10, 0x1F },	// 0x45 E
	{ 0x1F, 0x10, 0x10, 0x1C, 0x10, 0x10, 0x10 },	// 0x46 F
	{ 0x0F, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0E },	// 0x47 G
	{ 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11 },	// 0x48 H
	{ 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e },	// 0x49	I
	{ 0x0e, 0x04, 0x04, 0x04, 0x04, 0x14, 0x08 },	// 0x4A J
	{ 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11 },	// 0x4B K
	{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f },	// 0x4C L
	{ 0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11 },	// 0x4D M
	{ 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11 },	// 0x4E N
	{ 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E },	// 0x4F O
	{ 0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10 },	// 0x50 P
	{ 0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D },	// 0x51 Q
	{ 0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11 },	// 0x52 R
	{ 0x0e, 0x11, 0x10, 0x0e, 0x01, 0x11, 0x0e },	// 0x53 S
	{ 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 },	// 0x54 T
	{ 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e },	// 0x55 U
	{ 0x11, 0x11, 0x11, 0x11, 0x0a, 0x0a, 0x04 },	// 0x56 V
	{ 0x11, 0x11, 0x11, 0x11, 0x15, 0x15, 0x0a },	// 0x57 W
	{ 0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11 },	// 0x58 X
	{ 0x11, 0x11, 0x0a, 0x0a, 0x04, 0x04, 0x04 },	// 0x59 Y
	{ 0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f },	// 0x5A Z
	{ 0x0f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0f },	// 0x5B [
	{ 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00 },	// 0x5C backslash
	{ 0x1e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x1e }, 	// 0x5D ]
	{ 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00 },	// 0x5E ^
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F },	// 0x5F _
	{ 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },	// 0x60 �
	{ 0x00, 0x0e, 0x01, 0x0d, 0x13, 0x11, 0x0f },	// 0x61	a
	{ 0x10, 0x10, 0x10, 0x16, 0x19, 0x11, 0x1e },	// 0x62 b
	{ 0x00, 0x00, 0x0f, 0x10, 0x10, 0x10, 0x0f },	// 0x63 c
	{ 0x01, 0x01, 0x01, 0x0d, 0x13, 0x11, 0x0f },	// 0x64 d
	{ 0x00, 0x00, 0x0e, 0x11, 0x1e, 0x10, 0x0e },	// 0x65 e
	{ 0x06, 0x09, 0x08, 0x1c, 0x08, 0x08, 0x08 },	// 0x66 f
	{ 0x00, 0x0f, 0x11, 0x13, 0x0d, 0x01, 0x1e },	// 0x67 g
	{ 0x10, 0x10, 0x10, 0x16, 0x19, 0x11, 0x11 },	// 0x68	h
	{ 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x04 },	// 0x69 i
	{ 0x00, 0x02, 0x00, 0x02, 0x02, 0x12, 0x0c },	// 0x6A j
	{ 0x10, 0x10, 0x11, 0x12, 0x1c, 0x12, 0x11 },	// 0x6B k
	{ 0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e },	// 0x6C l
	{ 0x00, 0x00, 0x1a, 0x15, 0x15, 0x15, 0x15 },	// 0x6D m
	{ 0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11 },	// 0x6E n
	{ 0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x1e },	// 0x6F o
	{ 0x00, 0x1e, 0x11, 0x19, 0x16, 0x10, 0x10 },	// 0x70 p
	{ 0x00, 0x0f, 0x11, 0x13, 0x0d, 0x01, 0x01 },	// 0x71 q
	{ 0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10 },	// 0x72	r
	{ 0x00, 0x00, 0x0f, 0x10, 0x0e, 0x01, 0x1e },	// 0x73 s
	{ 0x04, 0x04, 0x0e, 0x04, 0x04, 0x05, 0x00 },	// 0x74	t
	{ 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x0D },	// 0x75	u
	{ 0x00, 0x00, 0x11, 0x11, 0x11, 0x0a, 0x04 },	// 0x76	v
	{ 0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0a },	// 0x77 w
	{ 0x00, 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11 },	// 0x78 x
	{ 0x00, 0x00, 0x11, 0x11, 0x0f, 0x02, 0x0c },	// 0x79 y
	{ 0x00, 0x00, 0x1f, 0x02, 0x04, 0x08, 0x1f },	// 0x7A z
	{ 0x03, 0x04, 0x04, 0x08, 0x04, 0x04, 0x03 },	// 0x7B {
	{ 0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04 },	// 0x7C |
	{ 0x18, 0x04, 0x04, 0x02, 0x04, 0x04, 0x18 },	// 0x7D }
	{ 0x80, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00 },	// 0x7E ~
	{ 0xff, 0xff, 0xFF, 0xfF, 0xff, 0xff, 0xff },	// 0x7F box
};

/* console data */
static int cursor;

/* goes from 0 to 7 */
static int brightness;

/* set software cursor (0 to 7)
 */
void sda5708_set_cursor(int col) {
	if(col > 7) col = 7;
	cursor = col;
	return;
}

/* print a string (more than 8 chars possible, but sucks) */
void sda5708_puts(char *s) {
	while(*s) {
		sda5708_putc(*s);
		s++;
	}
	return;
}

/* print a single char, currently no scrolling, stucks in last col */
void sda5708_putc(uint8_t c) {
	sda5708_put_char_at(cursor, c);
	cursor++;
	if(cursor > 7) {
		cursor = 7;
		//sda5708_clr();
		/* TODO: scrolling */
	}
}

/* print a character at a specific column */
void sda5708_put_char_at(uint8_t col, uint8_t c) {
	uint8_t buffer[7];
	uint8_t i;

	/* copy character data to buffer */
	for(i = 0; i < 7; i++) {
		buffer[i] = charset[c][i];
	}

	/* send buffer to display */
	sda5708_wrpattern(col, buffer);

	return;
}

/* put a digint from an uint8_t on display, digit is taken from charset */
void sda5708_put_dig(uint8_t col, uint8_t num) {
        uint8_t buffer[7];
        uint8_t i;
	uint8_t pos_in_charset;

	if(num > 9) num = 9; // check args

	/* copy character data to buffer */
	pos_in_charset = num + 0x30;
        for(i = 0; i < 7; i++) {
                buffer[i] = charset[pos_in_charset][i];
        }

	/* send buffer to display */
	sda5708_wrpattern(col, buffer);

	return;
}

/* clears a single col by sending 7 empty bytes, in fact, we write a 'space' */
void sda5708_clr_col(uint8_t col) {
	int i;
	uint8_t buffer[7];
	for(i = 0; i < 7; i++) buffer[i] = 0;

	sda5708_wrpattern(col, buffer);
}

/* display supports a clear command for whole display, we use this.
 * Instead of clocking out 63bytes, we only need two bytes.
 */
void sda5708_clr(void) {
	sda5708_wrbyte(SDA5708_REG_CONTROL | brightness);
	sda5708_delay(1);
	sda5708_wrbyte(SDA5708_REG_CONTROL | SDA5708_DISP_NORMALOP | brightness);

	cursor = 0;
	return;
}

/*
 *
 */
void sda5708_set_brightness(int br) {
	uint8_t ctrl_byte;

	br = ~br;
	br &= 0x07;		// 0-7
	brightness = br;

	ctrl_byte = SDA5708_REG_CONTROL | SDA5708_DISP_NORMALOP | brightness;
	sda5708_wrbyte(ctrl_byte);

	return;
}

/*
 *
 */
int sda5708_get_brightness(void) {
	return brightness;
}


/*
 *
 */
void sda5708_wrpattern(int col, uint8_t pattern[7]) {
	int i;
	uint8_t data;

	col &= 0x07;

	sda5708_wrbyte(SDA5708_REG_ADDRESS | col);

	for(i = 0; i < 7; i++) {
		data = pattern[i];
		data &= 0x1F;
		sda5708_wrbyte(data);
	}
}

/*
 *
 */
void sda5708_wrbyte(uint8_t data) {
  int i;
  SDA5708_LOAD_L();
	sda5708_delay(1);
	for (i=0; i <= 7; i++) {
		if ((data>>i)&1) {
			SDA5708_DATA_H();
		}	else {
			SDA5708_DATA_L();
		}

		sda5708_delay(1);
		SDA5708_CLOCK_H();
		SDA5708_CLOCK_L();
  }
  SDA5708_LOAD_H();
}

/*
 *
 */
void sda5708_reset_pulse(void) {
	SDA5708_RESET_L();
	sda5708_delay(1);
	SDA5708_RESET_H();
}

/*
 *
 */
void sda5708_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA5708_PIN_CLOCK | SDA5708_PIN_DATA | SDA5708_PIN_LOAD | SDA5708_PIN_RESET;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// default pin states
	SDA5708_RESET_H();
	SDA5708_LOAD_H();
	SDA5708_DATA_L();
	SDA5708_CLOCK_H();

	sda5708_delay(1);
	sda5708_reset_pulse();
	sda5708_delay(1);

	sda5708_wrbyte(SDA5708_REG_CONTROL | SDA5708_DISP_NORMALOP);
	sda5708_delay(1);

	cursor = 0;
	sda5708_set_brightness(7);

	sda5708_clr();

	return;
}

