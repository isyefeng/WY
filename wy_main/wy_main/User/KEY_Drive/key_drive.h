#ifndef __KEY_DRIVE_H
#define __KEY_DRIVE_H

#include "stm32f10x.h"

#define GPIO_KEY1_PIN  GPIO_Pin_0
#define GPIO_KEY1_RORT GPIOA
#define GPIO_KEY1_RCC  RCC_APB2Periph_GPIOA

#define GPIO_KEY2_PIN  GPIO_Pin_13
#define GPIO_KEY2_RORT GPIOC
#define GPIO_KEY2_RCC  RCC_APB2Periph_GPIOC


#define KEY_ON  1
#define KEY_OFF 0

void Key1_Init(void);
void Key2_Init(void);
uint8_t Key1_Read(void);
uint8_t Motor_switch(void);
uint8_t PWM_TUGO(void);

#endif /*__KEY_DRIVE_H*/
