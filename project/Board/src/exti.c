#include "stm32f10x.h"
#include "exti.h"
#include "delay.h"
#include "oled.h"

extern int Mode;
static void Exti_Key_NVIC_Config() 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//KEY1���� 
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_NVIC_IRQChannel;//�����ж�Դ��KEY1 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;// ������ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//���ô����ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
	//KEY2����
	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_NVIC_IRQChannel;//�����ж�Դ��KEY2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;// ������ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//���ô����ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�ͨ�� 
	NVIC_Init(&NVIC_InitStructure);
}

void Exti_Key_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//KEY1���� 
	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	//KEY2���� 
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

} 

void Exti_Key_EXTI_Config()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	//KEY1���� 
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);//����GPIO���ж��ߵ�ӳ���ϵ ������EXTI���ź�Դ
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����EXTIΪ�ж����� 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//����������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж� 
	EXTI_Init(&EXTI_InitStructure);
	//KEY2���� 
	GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);//����GPIO���ж��ߵ�ӳ���ϵ ������EXTI���ź�Դ 
	EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����EXTIΪ�ж����� 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//����������·������Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж� 
	EXTI_Init(&EXTI_InitStructure);
} 
void Exti_Key_Config()
{
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK | RCC_APB2Periph_AFIO,ENABLE);//ʹ��ʱ�� 
	Exti_Key_NVIC_Config();//����NVIC
	Exti_Key_GPIO_Config();//����GPIO 
	Exti_Key_EXTI_Config();//����EXTI 
}



void KEY2_IRQHandler()//�жϷ�����key2--led2���� 
{	
		delay_ms(100);//��ʱ����
//	if (KEY2==1)
	if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE)!=RESET) //���KEY2��Ӧ�ĵ�EXTI��·������������� 
	{
		Mode = -Mode;//��ת״̬ 
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
	EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);//���EXTI��·����λ
}
