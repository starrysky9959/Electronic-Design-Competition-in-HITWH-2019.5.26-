#ifndef _EXTI_H_
#define _EXTI_H_
#include "stm32f10x.h" 

#define KEY1_INT_GPIO_PORT        		GPIOC
#define KEY1_INT_GPIO_CLK         		RCC_APB2Periph_GPIOC
#define KEY1_INT_GPIO_PIN         		GPIO_Pin_12
#define KEY1_INT_NVIC_IRQChannel   		EXTI15_10_IRQn
#define KEY1_INT_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOC
#define KEY1_INT_EXTI_PINSOURCE    		GPIO_PinSource12
#define KEY1_INT_EXTI_LINE						EXTI_Line12
#define KEY1_IRQHandler								EXTI15_10_IRQHandler

#define KEY2_INT_GPIO_PORT        		GPIOE 
#define KEY2_INT_GPIO_CLK         		RCC_APB2Periph_GPIOE
#define KEY2_INT_GPIO_PIN         		GPIO_Pin_2
#define KEY2_INT_NVIC_IRQChannel   		EXTI2_IRQn 
#define KEY2_INT_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOE
#define KEY2_INT_EXTI_PINSOURCE    		GPIO_PinSource2
#define KEY2_INT_EXTI_LINE						EXTI_Line2
#define KEY2_IRQHandler								EXTI2_IRQHandler

#define KEY1  GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN) //读取按键1
#define KEY2  GPIO_ReadInputDataBit(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN) //读取按键2  
extern void Exti_Key_NVIC_Config(void);
extern void Exti_Key_GPIO_Config(void);
extern void Exti_Key_EXTI_Config(void);
extern void Exti_Key_Config(void);
extern void KEY_IRQHandler(void);
#endif
