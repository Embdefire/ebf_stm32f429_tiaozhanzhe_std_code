/**
  ******************************************************************************
  * @file    bsp_breath_led.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_breath_led.h"


/* LED���ȵȼ� PWM�� */
uint8_t indexWave[] = {1,1,2,2,3,4,6,8,10,14,19,25,33,44,59,80,
	107,143,191,255,255,191,143,107,80,59,44,33,25,19,14,10,8,6,4,3,2,2,1,1};



 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( BREATH_LED1_GPIO_CLK|BREATH_LED2_GPIO_CLK|BREATH_LED3_GPIO_CLK|LED4_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(BREATH_LED1_GPIO_PORT,BREATH_LED1_PINSOURCE,BREATH_LED1_AF); 
		GPIO_PinAFConfig(BREATH_LED2_GPIO_PORT,BREATH_LED2_PINSOURCE,BREATH_LED2_AF); 
		GPIO_PinAFConfig(BREATH_LED3_GPIO_PORT,BREATH_LED3_PINSOURCE,BREATH_LED3_AF);  
	
		/*BREATH_LED1*/															   
		GPIO_InitStructure.GPIO_Pin = BREATH_LED1_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_Init(BREATH_LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*BREATH_LED2*/															   
		GPIO_InitStructure.GPIO_Pin = BREATH_LED2_PIN;	
    GPIO_Init(BREATH_LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*BREATH_LED3*/															   
		GPIO_InitStructure.GPIO_Pin = BREATH_LED3_PIN;	
    GPIO_Init(BREATH_LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*LED4*/	
		/*��������ģʽΪ���ģʽ LED4���Ƕ�ʱ����ͨ��������ʹ��pwm���ƣ����Բ�������������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  		
		GPIO_InitStructure.GPIO_Pin = LED4_PIN;	
    GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);	
		
//		/*�ر�RGB��*/
//		LED_RGBOFF;
		
		/*ָʾ��Ĭ�Ͽ���*/
		LED4(ON);

}


 /**
  * @brief  ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */ 
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BREATH_TIM_IRQn; 
	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/*
 * TIM_Period / Auto Reload Register(ARR) = 256-1   TIM_Prescaler=((SystemCoreClock/2)/1000000)*30-1 
 *  *	
 * ��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
 *				PCLK1 = HCLK / 4 
 *				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
 * ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz
 * �ж�����Ϊ = 1/(1/30MHz) * 256 = x ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	// ����TIMx_CLK,x[2,3,4,5] 
  RCC_APB1PeriphClockCmd(BREATH_TIM_CLK, ENABLE); 
	
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		 
  TIM_TimeBaseStructure.TIM_Period = 256-1;       //����ʱ����0������255����Ϊ256�Σ�Ϊһ����ʱ����
	
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
	// ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz
  /*������ʱ������TIM_Prescaler����Ч�������ü��ɣ��ж�����С ����˸�죬������˸����*/
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/2)/1000000)*30-1;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BREATH_TIM, &TIM_TimeBaseStructure);	
	
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;	  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ LED����
  TIM_OC1Init(BREATH_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(BREATH_TIM, TIM_OCPreload_Enable);
	
	
	TIM_OC2Init(BREATH_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��2  
	/*ʹ��ͨ��2����*/
	TIM_OC2PreloadConfig(BREATH_TIM, TIM_OCPreload_Enable);


  TIM_OC3Init(BREATH_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��3  
	/*ʹ��ͨ��3����*/
	TIM_OC3PreloadConfig(BREATH_TIM, TIM_OCPreload_Enable);


	TIM_ARRPreloadConfig(BREATH_TIM, ENABLE);			//ʹ��TIM���ؼĴ���ARR

	// �����������ж�
	TIM_ITConfig(BREATH_TIM,TIM_IT_Update,ENABLE);
	
	// ʹ�ܼ�����
	TIM_Cmd(BREATH_TIM, ENABLE);																		


}

/**
  * @brief  ��ʼ��������
  * @param  ��
  * @retval ��
  */
void BreathLED_Config(void)
{

	TIMx_GPIO_Config();
	
	TIM_Mode_Config();

	TIMx_NVIC_Configuration();	
}

/*********************************************END OF FILE**********************/
