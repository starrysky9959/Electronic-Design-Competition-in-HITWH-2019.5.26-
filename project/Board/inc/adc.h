#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h" 

#define KEY_UP_ADC 0.1
#define KEY_UP 1

#define KEY_DOWN_ADC 0.8
#define KEY_DOWN 2


extern void Adc_Init(void);																		//ADC ��ʼ��
extern uint16_t Adc_Get(uint8_t ch);													//��ȡ ADC ��ֵ
extern float Adc_Get_Average(uint8_t ch,uint8_t Times);		//���� ADC ��ƽ��ֵ
uint16_t Get_Adc(uint8_t Ch);					//��ȡһ��adcֵ
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times);		//��ȡ�����adc��ƽ��ֵ
extern int Key_detect(void);						//��ⰴ��
#endif
