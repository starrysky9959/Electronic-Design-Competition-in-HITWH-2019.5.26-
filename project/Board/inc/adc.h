#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h" 

#define KEY_UP_ADC 0.1
#define KEY_UP 1

#define KEY_DOWN_ADC 0.8
#define KEY_DOWN 2


extern void Adc_Init(void);																		//ADC 初始化
extern uint16_t Adc_Get(uint8_t ch);													//获取 ADC 的值
extern float Adc_Get_Average(uint8_t ch,uint8_t Times);		//计算 ADC 的平均值
uint16_t Get_Adc(uint8_t Ch);					//获取一次adc值
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times);		//获取任意次adc的平均值
extern int Key_detect(void);						//检测按键
#endif
