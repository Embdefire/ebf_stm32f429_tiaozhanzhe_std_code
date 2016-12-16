/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����������
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
#include "./led/bsp_breath_led.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(__IO u32 nCount); 


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{

  /*��ʼ������*/
  Debug_USART_Config();
 
	printf("\r\n ��ӭʹ�ñ���  STM32 F429 �����塣\r\n");		
	printf("\r\n ����������\r\n");	  
  printf("\r\n RGB LED �Ժ����Ƶ���ʽ��˸\r\n ");

  /* ��ʼ�������� */
	BreathLED_Config(); 
  
  
  while(1)
  {
		BREATH_LED_RED;
		Delay(0xAFFFFFF); 
		
		BREATH_LED_GREEN;	
		Delay(0xAFFFFFF);
		
		BREATH_LED_BLUE;	
		Delay(0xAFFFFFF);
		
		BREATH_LED_YELLOW;
		Delay(0xAFFFFFF); 

		
		BREATH_LED_PURPLE;	
		Delay(0xAFFFFFF);
		
		BREATH_LED_CYAN;	
		Delay(0xAFFFFFF);		
	
		BREATH_LED_WHITE;	
		Delay(0xAFFFFFF);
		
		BREATH_LED_RGBOFF;	
		Delay(0xAFFFFFF);


  }

}


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

