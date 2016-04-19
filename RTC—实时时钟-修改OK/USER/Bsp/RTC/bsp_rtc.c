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

// ѡ�� RTC ʱ��Դ������ͬ��/�첽Ԥ��Ƶ����ֵ
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI

#define LSE_STARTUP_TIMEOUT     ((uint16_t)0x05000)

void RTC_Config(void)
{  
  __IO uint16_t StartUpCounter = 0;
	FlagStatus LSEStatus = RESET;	
	RTC_InitTypeDef RTC_InitStructure;
	
	// ʹ�� PWR ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  // PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ���
  PWR_BackupAccessCmd(ENABLE);
	
/*=========================ѡ��RTCʱ��Դ==============================*/
// Ĭ��ʹ��LSE�����LSE��������ʹ��LSI 
  //ʹ��LSE
  RCC_LSEConfig(RCC_LSE_ON);	
	
	// �ȴ�LSE�����ȶ��������ʱ���˳�
  do
  {
    LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
    StartUpCounter++;
  }while((LSEStatus == RESET) && (StartUpCounter != LSE_STARTUP_TIMEOUT));
	
	
	if(LSEStatus == SET )
  {
		printf("\n\r LSE �����ɹ� \r\n");
		// ѡ��LSE��ΪRTC��ʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  }
	else
	{
		printf("\n\r LSE ���ϣ�תΪʹ��LSI \r\n");
		
		// ʹ��LSI	
		RCC_LSICmd(ENABLE);
		// �ȴ�LSI�ȶ� 
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{			
		}
		
		printf("\n\r LSI �����ɹ� \r\n");
		// ѡ��LSI��ΪRTC��ʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	}
	
  // ʹ�� RTC ʱ��
  RCC_RTCCLKCmd(ENABLE);
  // �ȴ� RTC APB �Ĵ���ͬ��
  RTC_WaitForSynchro();

/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	// ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ
	
	// �����첽Ԥ��Ƶ����ֵΪ127
	RTC_InitStructure.RTC_AsynchPrediv = 127;
	// ����ͬ��Ԥ��Ƶ����ֵΪ255
	RTC_InitStructure.RTC_SynchPrediv = 255;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	// ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ���
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
}

// ����ʱ�������
void RTC_TimeAndDate_Init(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
		// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = 0;        // 0~23
	RTC_TimeStructure.RTC_Minutes = 0;      // 0~59
	RTC_TimeStructure.RTC_Seconds = 0;      // 0~59
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = 1;       // 1~7
	RTC_DateStructure.RTC_Date = 29;         // 1~31
	RTC_DateStructure.RTC_Month = 3;         // 1~12
	RTC_DateStructure.RTC_Year = 16;         // 0~99
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

// ��ʾʱ�������
void RTC_TimeAndDate_Show(void)
{
	uint8_t Rtctmp=0;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	while(1)
	{
		// ��ȡ����
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		
		// ÿ���ӡһ��
  	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
    {
			// ��ӡ����
      printf("The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);
			
			// ��ӡʱ��
      printf("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.RTC_Hours, 
			RTC_TimeStructure.RTC_Minutes, 
			RTC_TimeStructure.RTC_Seconds);
			
      (void)RTC->DR;
    }
    Rtctmp = RTC_TimeStructure.RTC_Seconds;
	}	
}



