#include "stm32f10x.h"
#include "sda5708.h"

static const uint8_t charset_numbers[13][8] = {
        { 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e },   // 0
        { 0x02, 0x06, 0x0A, 0x02, 0x02, 0x02, 0x02 },   // 1
        { 0x0e, 0x11, 0x11, 0x02, 0x04, 0x08, 0x1f },   // 2
        { 0x0e, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0e },   // 3
        { 0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02 },   // 4
        { 0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e },   // 5
        { 0x0e, 0x11, 0x10, 0x1e, 0x11, 0x11, 0x0e },   // 6
        { 0x1f, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04 },   // 7
        { 0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e },   // 8
        { 0x0e, 0x11, 0x11, 0x0f, 0x01, 0x11, 0x0e },   // 9
        { 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00 },   // :
        { 0x00, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f },   // a
        { 0x00, 0x0a, 0x1f, 0x1f, 0x0e, 0x04, 0x00 }	// heart
};


void sda5708_init() {
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
	
	sda5708_reset_pulse();
	
	sda5708_wrbyte(SDA5708_REG_CONTROL | SDA5708_DISP_NORMALOP);	
	
}

void sda5708_put_num(uint8_t col, uint8_t num) {
        uint8_t buffer[7];
        uint8_t i;
        for(i = 0; i < 7; i++) {
                buffer[i] = charset_numbers[num][i];
        }
        sda5708_wrpattern(col, buffer);
}


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

void sda5708_wrbyte(uint8_t data) {
        int i;      
        SDA5708_LOAD_L();

	for (i=0; i <= 7; i++) {
                if ((data>>i)&1) { SDA5708_DATA_H(); }
		else 		 { SDA5708_DATA_L(); }
		
		SDA5708_CLOCK_H();
		SDA5708_CLOCK_L();
        }
        SDA5708_LOAD_H();
}


void sda5708_reset_pulse() {
	SDA5708_RESET_L();
	SDA5708_RESET_H();
}