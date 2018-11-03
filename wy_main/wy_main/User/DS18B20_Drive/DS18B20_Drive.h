#ifndef   __DS18B20_DRIVE_
#define	__DS18B20_DRIVE_

#include "Systick_Dirve.h"
#include "Delay.h"

#define	DS_GPIO_PIN	GPIO_Pin_7
#define	DS_GPIO_PORT	GPIOB
#define	DS_GPIO_RCC	RCC_APB2Periph_GPIOB

#define	mallus(a)	delay_us(a);

#define	DS_COM_ON_RROM			0XCC
#define	DS_COM_STAT_SW			0X44
#define	DS_COM_READ_DATA		0XBE
#define	DS_COM_READ_BASIC_INFO	0X33

#define	DS_OUT()	DS18B20_GPIO_output();
#define	DS_IN()		DS18B20_GPIO_input();
#define	DS_SET()	GPIO_SetBits(DS_GPIO_PORT, DS_GPIO_PIN);
#define	DS_RESET()	GPIO_ResetBits(DS_GPIO_PORT, DS_GPIO_PIN);
#define	DS_READ()	GPIO_ReadInputDataBit(DS_GPIO_PORT,DS_GPIO_PIN)
#define	DS_high()	DS_SET()
#define	DS_low()	DS_RESET()

void 		DS18B20_GPIO_output(void);
void 		DS18B20_GPIO_input(void);
void 		DS18_Init(void);
void 		DS18_write_Byte(uint8_t dat);
uint8_t 		DS18_read_Byte(void);
float 		DS_read_int_temperture(void);
void 		Get_DS18_id(uint8_t* dat);

#endif
