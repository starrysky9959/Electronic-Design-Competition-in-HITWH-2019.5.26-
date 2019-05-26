#include "stm32f10x.h"
#include "exti.h"
#include "delay.h"
#include "oled.h"

extern int Mode;
static void Exti_Key_NVIC_Config() 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//KEY1部分 
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_NVIC_IRQChannel;//配置中断源：KEY1 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;// 配置抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//配置从优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
	//KEY2部分
	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_NVIC_IRQChannel;//配置中断源：KEY2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;// 配置抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//配置从优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断通道 
	NVIC_Init(&NVIC_InitStructure);
}

void Exti_Key_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//KEY1部分 
	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	//KEY2部分 
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

} 

void Exti_Key_EXTI_Config()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	//KEY1部分 
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);//配置GPIO与中断线的映射关系 即配置EXTI的信号源
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //配置EXTI为中断请求 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//配置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能中断 
	EXTI_Init(&EXTI_InitStructure);
	//KEY2部分 
	GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);//配置GPIO与中断线的映射关系 即配置EXTI的信号源 
	EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //配置EXTI为中断请求 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置输入线路上升沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能中断 
	EXTI_Init(&EXTI_InitStructure);
} 
void Exti_Key_Config()
{
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK | RCC_APB2Periph_AFIO,ENABLE);//使能时钟 
	Exti_Key_NVIC_Config();//配置NVIC
	Exti_Key_GPIO_Config();//配置GPIO 
	Exti_Key_EXTI_Config();//配置EXTI 
}



void KEY2_IRQHandler()//中断服务函数key2--led2部分 
{	
		delay_ms(100);//延时消抖
//	if (KEY2==1)
	if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE)!=RESET) //检查KEY2对应的的EXTI线路触发请求发生与否 
	{
		Mode = -Mode;//反转状态 
		OLED_Clear();
		if (Mode==1)
		{
			OLED_ShowString(0, 0, "CH4:ADC_VAL=", 16);
			OLED_ShowString(0, 4, "VOL=", 16);
		}
		else
		{
			OLED_Clear();
		}
	}
	EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);//清除EXTI线路挂起位
}
