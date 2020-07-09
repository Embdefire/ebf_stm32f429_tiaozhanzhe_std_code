/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DHT11��ʪ�ȴ�������ȡ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./usart/rx_data_queue.h"
#include "systick/bsp_SysTick.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
	
	rx_queue_init();
	
	
	/* ����һ���ַ��� */
	Usart_SendString( DEBUG_USART,"����һ�������жϽ��ջ���ʵ��\n");
	printf("��ӭʹ��Ұ��STM32������\n\n\n\n");

  while(1)
	{	
		//��ȡ���ݲ����
		//ʵ��Ӧ���пɲο�pull data�ķ�ʽ��ȡ���ݽ��д���
		pull_data_from_queue();
	}	
}




/*********************************************END OF FILE**********************/

