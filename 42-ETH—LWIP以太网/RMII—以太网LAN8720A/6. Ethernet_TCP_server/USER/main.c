/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   TCP server����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/systick/bsp_SysTick.h"
#include "./Bsp/key/bsp_key.h"
#include "lwip/tcp.h"
#include "netconf.h"
#include "LAN8742A.h"
#include "tcp_echoserver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
KEY Key1,Key2;
extern __IO uint8_t EthLinkStatus;
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
/* Private function prototypes -----------------------------------------------*/
static void TIM3_Config(uint16_t period,uint16_t prescaler);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	uint8_t flag=0;
	/* ��ʼ��LED */
	LED_GPIO_Config();
	
	Key1_GPIO_Config();
	Key2_GPIO_Config();
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();
	
	/* ��ʼ��ϵͳ�δ�ʱ�� */	
	SysTick_Init();
	
	TIM3_Config(999,899);//10ms��ʱ��
	printf("LAN8720A Ethernet Demo\n");

	/* Configure ethernet (GPIOs, clocks, MAC, DMA) */
  ETH_BSP_Config();	
  printf("LAN8720A BSP INIT AND COMFIGURE SUCCESS\n");
	
  /* Initilaize the LwIP stack */
  LwIP_Init();	
  
  printf("    KEY1: ����TCP����\n");
  printf("    KEY2: �Ͽ�TCP����\n");
  
  //IP��ַ�Ͷ˿ڿ���netconf.h�ļ��޸ģ�����ʹ��DHCP�����Զ���ȡIP(��Ҫ·����֧��)
  printf("����IP�Ͷ˿�: %d.%d.%d.%d:%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3,LOCAL_SERVER_PORT);
	while(1)
	{
		Key_RefreshState(&Key1);//ˢ�°���״̬
		Key_RefreshState(&Key2);//ˢ�°���״̬
    if((Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)&&(flag==0))//���������¹�
		{
			LED2_ON;
			if (EthLinkStatus == 0)
			{
				/*create tcp server */ 
        tcp_echoserver_init();
				flag=1;
			}
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if((Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)&&(flag==1))//���������¹�
		{
			LED2_OFF;
			tcp_echoserver_close();
			flag=0;
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
		/* check if any packet received */
    if (ETH_CheckFrameReceived())
    { 
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
	}
}

/**
  * @brief  ͨ�ö�ʱ��3�жϳ�ʼ��
  * @param  period : �Զ���װֵ��
  * @param  prescaler : ʱ��Ԥ��Ƶ��
  * @retval ��
  * @note   ��ʱ�����ʱ����㷽��:Tout=((period+1)*(prescaler+1))/Ft us.
  *          Ft=��ʱ������Ƶ��,ΪSystemCoreClock/2=90,��λ:Mhz
  */
static void TIM3_Config(uint16_t period,uint16_t prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=prescaler;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=period;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ��ʱ��3�жϷ�����
  * @param  ��
  * @retval ��
  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		LocalTime+=10;//10ms����
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
