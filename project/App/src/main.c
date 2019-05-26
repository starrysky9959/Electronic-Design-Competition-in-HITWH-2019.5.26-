/******************************************************************************
  * @file    main.c
  * @author  陆展
  * @version V1.0
  * @date    2019-4-34
  * @brief   2019-5-26，校内电赛采集锂离子电池电压
	* @attention ADC采集最高3.3V，锂电池电压为3.7V-4.0V 要串联电阻分压
*******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "oled.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "tim.h"
#include "exti.h"
int Mode = -1;
int main()
{
	static double k = 3.3 / 4095;
	double	Vol_Value;
	float Warning_Value = 3.3, tm;
	int temp,tmp,iLen;
	int Adc_Value;
	int i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置NVIC优先级组为2
	SystemInit();
	delay_init();
	TIM_PWM_Init();
	Adc_Init();
	OLED_Init();
	Exti_Key_Config();
	
	OLED_Clear();
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	TIM_SetCompare3(GENERAL_TIM, 0);//关闭蜂鸣器
	TIM_SetCompare3(ADVANCED_TIM, 0);
	TIM_SetCompare4(GENERAL_TIM, 0);
	while (1)
	{
		if (Mode==-1)
		{

			switch(Key_detect())
			{
				case KEY_UP:
					Warning_Value +=0.1;
					break;
					
				case KEY_DOWN:
					Warning_Value -=0.1;
					break;
			}
		  OLED_ShowString(0, 0, "Warning_Val=", 16);
			temp = Warning_Value;
			OLED_ShowNum(96, 0, temp, 1, 16);
			OLED_ShowChar(104, 0, '.', 16);
			tm = Warning_Value - temp;
			tm *= 10;
			temp = tm;
			OLED_ShowNum(112, 0, temp, 1, 16);
			OLED_ShowChar(120, 0, 'V', 16);
		}
		else
		{	
			OLED_ShowString(0, 0, "CH4:ADC_VAL=", 16);
			OLED_ShowString(0, 4, "VOL=", 16);
			Adc_Value=Adc_Get_Average(ADC_Channel_4,10);
			OLED_ShowNum(96, 0, Adc_Value, 4, 16);	//输出ADC采样值
			Vol_Value =2 * Adc_Value * k;
			if (Vol_Value <= Warning_Value)
			{
					GPIO_SetBits(GPIOB, GPIO_Pin_8);
				for (i=0; i<7; i++)
				{
					TIM_SetCompare3(GENERAL_TIM, 10000);
					TIM_SetCompare4(ADVANCED_TIM, 5000);
					delay_ms(200);
					TIM_SetCompare4(ADVANCED_TIM, 0);
					delay_ms(50);
				}
				TIM_SetCompare4(ADVANCED_TIM, 5000);
			}
			else
			{
				GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				TIM_SetCompare3(GENERAL_TIM, 0);
				TIM_SetCompare4(ADVANCED_TIM, 0);
			}
				
			temp = Vol_Value;
			OLED_ShowNum(32, 4, temp, 1, 16);				//输出电压整数部分
			OLED_ShowChar(40, 4, '.', 16);
			Vol_Value -= temp;
			Vol_Value *=1000000;
			//计算小数部分长度
			tmp = Vol_Value;
			iLen=0;
			while (tmp>0)
			{
				iLen++;
				tmp /=10;
			}
			temp = Vol_Value;
			
			//输出电压小数部分
			for (i=0; i<6-iLen; i++)
			OLED_ShowChar(48+i*8, 4, '0', 16); 
			if (iLen>0) OLED_ShowNum(48+i*8, 4, temp, iLen, 16);
			OLED_ShowChar(96, 4, 'V', 16);
			delay_ms(600);
		}
	}
}
