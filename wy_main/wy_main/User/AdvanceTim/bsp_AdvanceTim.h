#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"


/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1

#define            ADVANCE_TIM8                   TIM8
#define            ADVANCE_TIM8_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM8_CLK               RCC_APB2Periph_TIM8


// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (1000-1)
#define            ADVANCE_TIM_PSC               (720-1)
#define            ADVANCE_TIM_PULSE             4

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 输出PWM通道
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM8 输出PWM通道
#define            ADVANCE_TIM8_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM8_CH1_PORT         	    GPIOC
#define            ADVANCE_TIM8_CH1_PIN         	    GPIO_Pin_6

/*电机1方向控制引脚*/
#define 		MOTO1_PIR_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		MOTO1_PIR_GPIO_PIN			GPIO_Pin_6
#define 		MOTO1_PIR_GPIO_PORT			GPIOB

/*电机2方向控制引脚*/
#define		MOTO2_PIR_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		MOTO2_PIR_GPIO_PIN			GPIO_Pin_12
#define 		MOTO2_PIR_GPIO_PORT			GPIOB

#define 	MOTOR_ANGLE 		90
#define	MOTOR_ON()			TIM_Cmd(ADVANCE_TIM, ENABLE);	
#define	MOTOR_OFF()			TIM_Cmd(ADVANCE_TIM, DISABLE);	
#define	MOTOR_TOGGLE()		digitalToggle(GPIOB, GPIO_Pin_6);

/**************************函数声明********************************/

void ADVANCE_TIM_Init(void);
void Moto1_rotate_angle(uint16_t offset);
void Motor_tim_config(uint16_t offset);
uint8_t Motor_switch(void);
uint8_t PWM_TUGO(void);

#endif	/* __BSP_ADVANCETIME_H */


