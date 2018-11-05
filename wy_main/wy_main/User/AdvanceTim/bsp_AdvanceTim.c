#include "bsp_AdvanceTim.h" 
#include "Systick_Dirve.h"
#include "LED.h"
#include "Includes.h"

static void MOTO_Drive_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(MOTO1_PIR_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MOTO2_PIR_GPIO_CLK, ENABLE);


	GPIO_InitStruct.GPIO_Pin =  MOTO1_PIR_GPIO_PIN;
  	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(MOTO1_PIR_GPIO_PORT, &GPIO_InitStruct);
	 
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Pin =  MOTO2_PIR_GPIO_PIN;
	 GPIO_Init(MOTO2_PIR_GPIO_PORT, &GPIO_InitStruct);

}

static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出PWM1通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

    // 输出PWM2通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM8_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM8_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM8_CH1_PORT, &GPIO_InitStructure);


}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)


/**********************电机1*********************************/
void Motor_tim_config(uint16_t offset)
{
	// 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=offset;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
}

static void ADVANCE_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM, DISABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_ITConfig(ADVANCE_TIM, TIM_IT_Update, ENABLE);
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

static void Moto_pwm_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}





/***********************电机2******************************/
void Motor_tim8_config(uint16_t offset)
{
	// 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM8_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=offset;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM8, &TIM_TimeBaseStructure);
}

static void ADVANCE_TIM8_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM8_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM8, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(ADVANCE_TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM8, TIM_OCPreload_Enable);
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM8, DISABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_ITConfig(ADVANCE_TIM8, TIM_IT_Update, ENABLE);
	TIM_CtrlPWMOutputs(ADVANCE_TIM8, ENABLE);
}

static void Moto_pwm2_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}









void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	MOTO_Drive_GPIO_Config();
	Moto_pwm_nvic_config();
	ADVANCE_TIM_Mode_Config();	

	/*电机2tim参数设置*/
	Moto_pwm2_nvic_config();
	ADVANCE_TIM8_Mode_Config();
}

void TIM1_UP_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(ADVANCE_TIM, TIM_IT_Update) != RESET)
	{		
		TIM_Cmd(ADVANCE_TIM, DISABLE);	
		TIM_ClearITPendingBit( ADVANCE_TIM, TIM_IT_Update);
	}	 
}

void	TIM8_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(ADVANCE_TIM8, TIM_IT_Update) != RESET)
	{		
		TIM_Cmd(ADVANCE_TIM8, DISABLE);	
		TIM_ClearITPendingBit( ADVANCE_TIM8, TIM_IT_Update);
	}	
}

/*设置电机1旋转角度*/
void Moto1_rotate_angle(uint16_t offset)
{
	uint16_t buf = 0;
	
	buf = offset*100/45;
	Motor_tim_config(buf);
	TIM_Cmd(ADVANCE_TIM, ENABLE);
}

/*设置电机2旋转角度*/
void Moto2_rotate_angle(uint16_t offset)
{
	uint16_t buf = 0;
	
	buf = offset*100/45;
	Motor_tim8_config(buf);
	TIM_Cmd(ADVANCE_TIM8, ENABLE);
}

uint8_t Motor_switch(void)
{
	static uint8_t dat = 0;
	static uint8_t temp = 2;
//	static uint8_t offset = 0;
	
	if(GPIO_ReadInputDataBit(GPIO_KEY2_RORT,GPIO_KEY2_PIN))
	{
		if(temp == 2)
		{
			dat = 1;
			temp--;
			Moto1_rotate_angle(90);
			Moto2_rotate_angle(90);
		}
		else if(temp == 1)
		{
			temp = 1;	
		} 
	}
	else
	{
		temp = 2;
		dat = 0;
	}

	return dat;
}


uint8_t PWM_TUGO(void)
{
	static uint8_t dat = 0;
	static uint8_t temp = 2;
	
	if(GPIO_ReadInputDataBit(GPIO_KEY1_RORT,GPIO_KEY1_PIN))
	{
		if(temp == 2)
		{
			dat = 1;
			digitalToggle(GPIOB, GPIO_Pin_5);
			digitalToggle(GPIOB, GPIO_Pin_12);
			digitalToggle(GPIOB, GPIO_Pin_6);
			temp--;
		}
		else if(temp == 1)
		{
			temp = 1;
		} 
	}
	else
	{
		temp = 2;
		dat = 0;
	}
	
	return dat;
}


/*********************************************END OF FILE**********************/
