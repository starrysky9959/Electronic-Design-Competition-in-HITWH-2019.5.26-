/******************************************************************************
  * @file    adc.c
  * @author  ½չ
  * @version V1.0
  * @date    2019-5-25
  * @brief   ADC��ѹ�ɼ�
	* @attention Adc_Init();��ʼ�����
							 Adc_Get_Average(uint8_t ch,uint8_t Times)	��ȡ��ѹ
*******************************************************************************/

#include "adc.h"
#include "stm32f10x.h"
#include "delay.h"

/**************************************************************
 *@brief	ADC��ʼ��
 *@param
 *@retval
 *@addition
**************************************************************/
void Adc_Init() 																					//ADC ��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;   									//��ʼ��GPIO�ṹ��
	ADC_InitTypeDef ADC_InitStructure;											//��ʼ��ADC�ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ�� GPIOA.4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ�� GPIOA.4
	//��ʼ�� ADC1 ����,���� ADC1 �Ĺ���ģʽ�Լ��������е������Ϣ
																//ʹ�� ADC1 ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 												//���� ADC ��Ƶ���� 6 (72M/6=12,ADC ���ʱ�䲻�ܳ��� 14M)
	ADC_DeInit(ADC1);																				//������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 		//ADC ����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//AD ��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//AD ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//���� ADC ���ݶ��뷽ʽΪ�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//16;							//���ù������еĳ���,���������ǵ���ת��,����ֵΪ 1 ����
	ADC_Init(ADC1, &ADC_InitStructure);											//���� ADC1 
	ADC_Cmd(ADC1, ENABLE); 																	//ʹ��ָ���� ADC1
	//����������������Ϣ��,���Ǿ�ʹ�� AD ת����,ִ�и�λУ׼�� AD У׼
	//ע���������Ǳ���ģ���У׼�����½���ܲ�׼ȷ
	//ʹ�� ADC ��У׼
	ADC_ResetCalibration(ADC1);										//ִ�и�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)); 		//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);										//��ʼ ADC1 ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));					////�ȴ� ADC У׼����
	

	ADC_DeInit(ADC2);																				//������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 		//ADC ����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//AD ��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//AD ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//���� ADC ���ݶ��뷽ʽΪ�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//16;							//���ù������еĳ���,���������ǵ���ת��,����ֵΪ 1 ����
	ADC_Init(ADC2, &ADC_InitStructure);											//���� ADC1 
	ADC_Cmd(ADC2, ENABLE); 																	//ʹ��ָ���� ADC1
	//����������������Ϣ��,���Ǿ�ʹ�� AD ת����,ִ�и�λУ׼�� AD У׼
	//ע���������Ǳ���ģ���У׼�����½���ܲ�׼ȷ
	//ʹ�� ADC ��У׼
	ADC_ResetCalibration(ADC2);										//ִ�и�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC2)); 		//�ȴ���λУ׼����
	ADC_StartCalibration(ADC2);										//��ʼ ADC1 ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC2));					////�ȴ� ADC У׼����
}	

uint16_t Adc_Get(uint8_t ch)						//��ȡ ADC ��ֵ
{
	//���ù������� 1 �����ͨ��,����˳��,�Լ�ͨ���Ĳ�������,Ȼ������ ADC ת��
	//��ת���������ȡ ADC ת�����ֵ
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_55Cycles5); 		//ѡ����������еĵ� 1 ��ת��,��������Ϊ 55.5
																																		// rank ��������ÿ��ͨ���Ĳɼ�˳��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);											  	//ʹ��ָ���� ADC1 �����ת����������
	
	//ͬʱ�� AD ת����,���ǻ�Ҫ����״̬�Ĵ����ı�־λ����ȡ AD ת���ĸ���״̬��Ϣ 
	//�õ��ĺ���Ϊ FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)); 						 		 		//�ȴ�ת������  ADC_FLAG_EOC Ϊת��������־λ
	return (ADC_GetConversionValue(ADC1)); 													//����ת��֮��,����ת�� ADC ת���������
}

float Adc_Get_Average(uint8_t ch,uint8_t Times)		//���� ADC ��ƽ��ֵ
{
	uint32_t temp_val=0;
	uint8_t t;
	
	for(t=0;t<Times;t++)
	{
		temp_val+=Adc_Get(ch);
		delay_ms(5);
	}
	return (temp_val/Times);		//��ƽ��
}
















/**
  * @brief  ��� ADC ֵ
  * @param  ch:ͨ��ֵ 0~3
  * @retval None
  */
uint16_t Get_Adc(uint8_t Ch)
{
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC2, Ch, 1, ADC_SampleTime_55Cycles5 );
	//ͨ�� 3,�������˳��ֵΪ 1,����ʱ��Ϊ 239.5 ����
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); //ʹ�����ת������
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC2); //�������һ�� ADC1 �������ת�����
}

/**
  * @brief  ����ƽ��ֵ
	* @param  Ch:ͨ��ֵ 0~3
						Times:ȡƽ���Ĵ���
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
	return temp_val/Times;		//��ƽ��
}
/**
  * @brief  �ж��ĸ�����������
  * @param  None
  * @retval None
  */
int Key_detect(void)
{
	uint16_t adcx;
	float temp;
	
	adcx=Get_Adc(ADC_Channel_2);//��ȡadcֵ
	temp=(float)adcx*(3.3/4096);					//�����ѹ
	
	//�ж��ĸ�����
	if (temp<=KEY_UP_ADC+0.1&&temp>=KEY_UP_ADC-0.1){
		delay_ms(80);//��ʱ����
		adcx=Get_Adc_Average(ADC_Channel_2,2);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_UP_ADC+0.1&&temp>=KEY_UP_ADC-0.1){
			return KEY_UP;
		}
	}
	else if(temp<=KEY_DOWN_ADC+0.1&&temp>=KEY_DOWN_ADC-0.1){
		delay_ms(80);
		adcx=Get_Adc_Average(ADC_Channel_2,2);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_DOWN_ADC+0.1&&temp>=KEY_DOWN_ADC-0.1){
			return KEY_DOWN;
		}
	}

	else{
		return 0;
	}
	return 0;
}
