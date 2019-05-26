/**********************PWM�ź� ���ں�ռ�ձȵļ���*********************
* ARR���Զ���װ�ؼĴ�����ֵ
* CLK_CNT����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
* PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
* ռ�ձ�P = CCR/(ARR+1)     
******************************************************************/

#include "TIM.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


/**
  * @brief	ͨ�ö�ʱ��GPIO����
  *	@param	��
  *	@retval ��
  */
static void GENERAL_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��4 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	//ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN|GENERAL_TIM_CH4_PIN;		//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�������Ƶ��
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	//��ʼ��GPIO
}

/**
  * @brief	�߼���ʱ��GPIO����
  *	@param	��
  *	@retval ��
  */
static void ADVANCED_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��3��4 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCED_TIM_CH3_GPIO_CLK, ENABLE);	//ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  ADVANCED_TIM_CH3_PIN | ADVANCED_TIM_CH4_PIN;	//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�������Ƶ��
	GPIO_Init(ADVANCED_TIM_CH3_PORT, &GPIO_InitStructure);	//��ʼ��GPIO
}

/**
  * @brief	ͨ�ö�ʱ��PWMģʽ����
  *	@param	��
  *	@retval ��
  */
static void GENERAL_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//��ʱ��ʱ���ṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;	//��ʱ������ȽϽṹ��
	//ʹ�ܶ�ʱ��ʱ��
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ����Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �����ظ���������ֵΪ0���������ظ�����
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/
	
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	//ʹ������Ƚ�
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_Pulse = 0;
	//�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//��ʼ������Ƚ�ͨ��3
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);  
	//ʹ��ͨ��3��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);  
	//��ʼ������Ƚ�ͨ��3
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);  
	//ʹ��ͨ��4��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);  
	//ʹ��TIM
	TIM_Cmd(GENERAL_TIM, ENABLE);  
}

/**
  * @brief	�߼���ʱ��PWMģʽ����
  *	@param	��
  *	@retval ��
  */
static void ADVANCED_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//��ʱ��ʱ���ṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;	//��ʱ������ȽϽṹ��
	//ʹ�ܶ�ʱ��ʱ��
	ADVANCED_TIM_APBxClock_FUN(ADVANCED_TIM_CLK, ENABLE);
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCED_TIM_Period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCED_TIM_Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �����ظ���������ֵΪ0���������ظ�����
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCED_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/
	
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	//ʹ������Ƚ�
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_Pulse = 0;
	//�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/**********************ͨ��3*******************************/
	//��ʼ������Ƚ�ͨ��3
	TIM_OC3Init(ADVANCED_TIM, &TIM_OCInitStructure);  
	//ʹ��ͨ��4��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(ADVANCED_TIM, TIM_OCPreload_Enable);  	
	
	/**********************ͨ��4*******************************/
	//��ʼ������Ƚ�ͨ��4
	TIM_OC4Init(ADVANCED_TIM, &TIM_OCInitStructure);  
	//ʹ��ͨ��4��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(ADVANCED_TIM, TIM_OCPreload_Enable);  
	
	//ʹ��TIM
	TIM_Cmd(ADVANCED_TIM, ENABLE);
	//�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��
	TIM_CtrlPWMOutputs(ADVANCED_TIM, ENABLE);
}

/**
  * @brief	��ʱ��PWMģʽ��ʼ��
  *	@param	��
  *	@retval ��
  */
void TIM_PWM_Init()
{
	GENERAL_TIM_GPIO_Config();
	ADVANCED_TIM_GPIO_Config();
	
	GENERAL_TIM_Mode_Config();
	ADVANCED_TIM_Mode_Config();
}

