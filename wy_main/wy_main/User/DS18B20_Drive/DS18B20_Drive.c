#include "DS18B20_Drive.h"

void DS18B20_GPIO_output(void)
{
	GPIO_InitTypeDef GPIO_InitSturct;

	RCC_APB2PeriphClockCmd(DS_GPIO_RCC, ENABLE);

	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturct.GPIO_Pin = DS_GPIO_PIN;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS_GPIO_PORT,  &GPIO_InitSturct);
}

void DS18B20_GPIO_input(void)
{
	GPIO_InitTypeDef GPIO_InitSturct;

	RCC_APB2PeriphClockCmd(DS_GPIO_RCC, ENABLE);

	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitSturct.GPIO_Pin = DS_GPIO_PIN;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS_GPIO_PORT,  &GPIO_InitSturct);
}

uint8_t DS18_pre(void)
{
	uint8_t time_out = 0;

	DS_IN();
	while( DS_READ() && (time_out<100) )
	{
		time_out++;
		mallus(1);
	}
	if(time_out >= 100)
		return 1;
	else
		time_out = 0;
	while(!DS_READ() && (time_out < 100))
	{
		time_out++;
		mallus(1);
	}
	if(time_out >= 100)
		return 1;
	else
		return 0;
}

void	DS18_Init(void)
{
	DS_OUT();
	DS_low();
	mallus(750);
	DS_high();
	mallus(15);
	DS18_pre();
}

void DS18_write_Byte(uint8_t dat)
{
	uint8_t index = 0;

	DS_OUT();
	for(index = 0;index < 8;index++)
	{
		if(dat&0x01)
		{
			DS_low();
			mallus(8);
			DS_high();
			mallus(58);
		}
		else
		{
			DS_low();
			mallus(70);
			DS_high();
			mallus(2);
		}
		dat >>=1;
	}
}

uint8_t DS_read_bit(void)
{
	uint8_t dat = 0;
	
	DS_OUT();
	DS_low();
	mallus(10);
	DS_IN();
	if(DS_READ())
		dat = 1;
	else
		dat = 0;
	mallus(45);

	return dat;
}

uint8_t DS18_read_Byte(void)
{
	uint8_t index= 0;
	uint8_t data = 0;
	uint8_t bbit = 0;

	for(index = 0;index < 8;index++)
	{
		bbit = DS_read_bit();
		data |= bbit<<index;
	}
	return data;
}

float DS_read_int_temperture(void)
{
	float _buf = 0;
	uint8_t High_byte = 0;
	uint8_t Low_byte = 0;
	float float_byte = 0;

	DS18_Init();
	DS18_write_Byte(DS_COM_ON_RROM);
	DS18_write_Byte(DS_COM_STAT_SW);
	mallus(500);
	DS18_Init();
	DS18_write_Byte(DS_COM_ON_RROM);
	DS18_write_Byte(DS_COM_READ_DATA);
	Low_byte = DS18_read_Byte();
	High_byte = DS18_read_Byte();
	float_byte = (float)(Low_byte&0x0f)*0.0625;
	_buf = Low_byte>>4|High_byte<<4;
	_buf +=float_byte;
	return _buf;
}

void Get_DS18_id(uint8_t* dat)
{
	uint8_t index = 0;

	DS18_Init();
	DS18_write_Byte(DS_COM_READ_BASIC_INFO);
	for(index = 0;index < 8;index++)
		dat[index] = DS18_read_Byte();
}

