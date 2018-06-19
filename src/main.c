#include "stm32f10x.h"
#include "sda5708.h"

#define LED_ON()	GPIO_SetBits(GPIOC , GPIO_Pin_13)
#define LED_OFF()	GPIO_ResetBits(GPIOC , GPIO_Pin_13)

	
void init_led();
void delay(unsigned int j);

uint8_t heart[7] = {  };

int main(void){
	SystemInit();
	sda5708_init();
	init_led();

	sda5708_wrpattern(7, heart);
	
	
	uint8_t cols[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	int i;
	
	for(;;) {
		LED_ON();
		delay(550000);
		LED_OFF();
		delay(550000);

		for(i = 0; i < 8; i++) {
			cols[i]++;
			if(cols[i] > 12) cols[i] = 0;
			sda5708_put_dig(i, cols[i]);
		}
		
	}

	return 0;
}

void init_led() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);

	// LED=PC13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	return;
}

void delay(unsigned int j) {
	unsigned int i;

	for(i = 0; i < j; i++) {
		asm("nop");
	}
	return;
}
