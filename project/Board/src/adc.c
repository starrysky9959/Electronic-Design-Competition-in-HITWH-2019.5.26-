/******************************************************************************
  * @file    adc.c
  * @author  陆展
  * @version V1.0
  * @date    2019-5-25
  * @brief   ADC电压采集
	* @attention Adc_Init();初始化完成
							 Adc_Get_Average(uint8_t ch,uint8_t Times)	获取电压
*******************************************************************************/

#include "adc.h"
#include "stm32f10x.h"
#include "delay.h"

/**************************************************************
 *@brief	ADC初始化
 *@param
 *@retval
 *@addition
**************************************************************/
void Adc_Init() 																					//ADC 初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;   									//初始化GPIO结构体
	ADC_InitTypeDef ADC_InitStructure;											//初始化ADC结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化 GPIOA.4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化 GPIOA.4
	//初始化 ADC1 参数,设置 ADC1 的工作模式以及规则序列的相关信息
																//使能 ADC1 通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 												//设置 ADC 分频因子 6 (72M/6=12,ADC 最大时间不能超过 14M)
	ADC_DeInit(ADC1);																				//将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 		//ADC 工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//AD 单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//AD 单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//设置 ADC 数据对齐方式为右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;//16;							//设置规则序列的长度,这里我们是单次转换,所以值为 1 即可
	ADC_Init(ADC1, &ADC_InitStructure);											//配置 ADC1 
	ADC_Cmd(ADC1, ENABLE); 																	//使能指定的 ADC1
	//在设置完了以上信息后,我们就使能 AD 转换器,执行复位校准和 AD 校准
	//注意这两步是必须的！不校准将导致结果很不准确
	//使能 ADC 并校准
	ADC_ResetCalibration(ADC1);										//执行复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); 		//等待复位校准结束
	ADC_StartCalibration(ADC1);										//开始 ADC1 的校准状态
	while(ADC_GetCalibrationStatus(ADC1));					////等待 ADC 校准结束
	

	ADC_DeInit(ADC2);																				//将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 		//ADC 工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//AD 单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//AD 单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//设置 ADC 数据对齐方式为右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;//16;							//设置规则序列的长度,这里我们是单次转换,所以值为 1 即可
	ADC_Init(ADC2, &ADC_InitStructure);											//配置 ADC1 
	ADC_Cmd(ADC2, ENABLE); 																	//使能指定的 ADC1
	//在设置完了以上信息后,我们就使能 AD 转换器,执行复位校准和 AD 校准
	//注意这两步是必须的！不校准将导致结果很不准确
	//使能 ADC 并校准
	ADC_ResetCalibration(ADC2);										//执行复位校准
	while(ADC_GetResetCalibrationStatus(ADC2)); 		//等待复位校准结束
	ADC_StartCalibration(ADC2);										//开始 ADC1 的校准状态
	while(ADC_GetCalibrationStatus(ADC2));					////等待 ADC 校准结束
}	

uint16_t Adc_Get(uint8_t ch)						//获取 ADC 的值
{
	//设置规则序列 1 里面的通道,采样顺序,以及通道的采样周期,然后启动 ADC 转换
	//在转换结束后读取 ADC 转换结果值
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_55Cycles5); 		//选择规则序列中的第 1 个转换,采样周期为 55.5
																																		// rank 用于配置每个通道的采集顺序
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);											  	//使能指定的 ADC1 的软件转换启动功能
	
	//同时在 AD 转换中,我们还要根据状态寄存器的标志位来获取 AD 转换的各个状态信息 
	//用到的函数为 FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)); 						 		 		//等待转换结束  ADC_FLAG_EOC 为转换结束标志位
	return (ADC_GetConversionValue(ADC1)); 													//开启转换之后,返回转换 ADC 转换结果数据
}

float Adc_Get_Average(uint8_t ch,uint8_t Times)		//计算 ADC 的平均值
{
	uint32_t temp_val=0;
	uint8_t t;
	
	for(t=0;t<Times;t++)
	{
		temp_val+=Adc_Get(ch);
		delay_ms(5);
	}
	return (temp_val/Times);		//求平均
}
















/**
  * @brief  获得 ADC 值
  * @param  ch:通道值 0~3
  * @retval None
  */
uint16_t Get_Adc(uint8_t Ch)
{
	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC2, Ch, 1, ADC_SampleTime_55Cycles5 );
	//通道 3,规则采样顺序值为 1,采样时间为 239.5 周期
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); //使能软件转换功能
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC2); //返回最近一次 ADC1 规则组的转换结果
}

/**
  * @brief  计算平均值
	* @param  Ch:通道值 0~3
						Times:取平均的次数
  * @retval None
  */
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<Times;t++)
	{ 
		temp_val+=Get_Adc(Ch);
		delay_ms(5);
	}
	return temp_val/Times;		//求平均
}
/**
  * @brief  判断哪个按键被按下
  * @param  None
  * @retval None
  */
int Key_detect(void)
{
	uint16_t adcx;
	float temp;
	
	adcx=Get_Adc(ADC_Channel_2);//获取adc值
	temp=(float)adcx*(3.3/4096);					//计算电压
	
	//判断哪个按键
	if (temp<=KEY_UP_ADC+0.1&&temp>=KEY_UP_ADC-0.1){
		delay_ms(80);//延时消抖
		adcx=Get_Adc_Average(ADC_Channel_2,2);//获取adc值
		temp=(float)adcx*(3.3/4096);					//计算电压
		if (temp<=KEY_UP_ADC+0.1&&temp>=KEY_UP_ADC-0.1){
			return KEY_UP;
		}
	}
	else if(temp<=KEY_DOWN_ADC+0.1&&temp>=KEY_DOWN_ADC-0.1){
		delay_ms(80);
		adcx=Get_Adc_Average(ADC_Channel_2,2);//获取adc值
		temp=(float)adcx*(3.3/4096);					//计算电压
		if (temp<=KEY_DOWN_ADC+0.1&&temp>=KEY_DOWN_ADC-0.1){
			return KEY_DOWN;
		}
	}

	else{
		return 0;
	}
	return 0;
}
