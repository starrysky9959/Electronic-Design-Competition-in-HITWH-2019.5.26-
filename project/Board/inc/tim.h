#ifndef _TIM_H_
#define _TIM_H_

/**********************************ע��*****************************************
* TIM1��TIM8Ϊ�߼���ʱ��������������APB2��
* TIM2��3��4��5Ϊͨ�ö�ʱ��������������APB1��

* ��ʹ�ò�ͬ�Ķ�ʱ��ʱ����Ӧ��GPIO��һ������Ҫ�޸�
* ����ʹ�õ�TIM8��ͨ��3��4��TIM4��ͨ��4
******************************************************************************/

/*****************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8***************************/
#define		ADVANCED_TIM					TIM8
#define 	ADVANCED_TIM_APBxClock_FUN		RCC_APB2PeriphClockCmd
#define 	ADVANCED_TIM_CLK				RCC_APB2Periph_TIM8
#define     ADVANCED_TIM_Period            	20000
#define     ADVANCED_TIM_Prescaler         	71

//TIMͨ��3
#define     ADVANCED_TIM_CH3_GPIO_CLK      	RCC_APB2Periph_GPIOC
#define     ADVANCED_TIM_CH3_PORT          	GPIOC
#define     ADVANCED_TIM_CH3_PIN           	GPIO_Pin_8

//TIMͨ��4
#define     ADVANCED_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOC
#define     ADVANCED_TIM_CH4_PORT          	GPIOC
#define     ADVANCED_TIM_CH4_PIN           	GPIO_Pin_9


/*****************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5***************************/
#define		GENERAL_TIM						TIM4
#define 	GENERAL_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define 	GENERAL_TIM_CLK					RCC_APB1Periph_TIM4
#define     GENERAL_TIM_Period            	20000
#define     GENERAL_TIM_Prescaler         	1

//TIMͨ��3
#define     GENERAL_TIM_CH3_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH3_PORT          	GPIOB
#define     GENERAL_TIM_CH3_PIN           	GPIO_Pin_8


//TIMͨ��4
#define     GENERAL_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH4_PORT          	GPIOB
#define     GENERAL_TIM_CH4_PIN           	GPIO_Pin_9


extern void TIM_PWM_Init(void);

#endif
