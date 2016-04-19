/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RTC����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./Bsp/RTC/bsp_rtc.h"
#include "./Bsp/usart/bsp_debug_usart.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	// ��ʼ�����Դ���
	Debug_USART_Config();
	printf("\r\n RTC ʵʱʱ��ʵ��\r\n");
  
  // �ж��Ƿ����ϵ�/�ⲿ��λ���� ��λ
//  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
//  {
//		printf("\r\n RTC ��ʼ��\r\n");
//		RTC_Config();
//		RTC_TimeAndDate_Init();
//  }
//  else
//  {
//    // �ж��ǲ����ϵ縴λ
//    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
//    {
//      printf("\r\n �����ϵ縴λ\r\n");
//    }
//    // �ж��ǲ��� �ⲿ��λ���� ��λ
//    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
//    {
//      printf("\r\n �����ⲿpin��λ\r\n");
//    }

//    printf("\r\n ������������RTC....\r\n");
//    
//    // ʹ��PWRʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//    // PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ���
//    PWR_BackupAccessCmd(ENABLE);
//    // �ȴ� RTC APB �Ĵ���ͬ��
//    RTC_WaitForSynchro();		
//		
//  }
		RTC_Config();
		RTC_TimeAndDate_Init();
	// ��ʾʱ�������
	RTC_TimeAndDate_Show();
}




