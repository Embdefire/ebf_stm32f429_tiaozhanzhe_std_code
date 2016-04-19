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
#include "./Bsp/led/bsp_led.h" 
#include "./Bsp/usart/bsp_debug_usart.h"

/* Uncomment the corresponding line to select the RTC Clock source */
   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */ 

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

RTC_TimeTypeDef  RTC_TimeStampStructure;
RTC_DateTypeDef  RTC_TimeStampDateStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
  

void RTC_Config(void);
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_DateShow(void);
void RTC_TimeStampShow(void);
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  uint8_t Rtctmp=0;

	// ��ʼ�����Դ���
	Debug_USART_Config();
  

//  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
//  {
//    /* RTC configuration  */
//    RTC_Config();



//    /* Configure the time register */
//    RTC_TimeRegulate(); 
//  }
//  else
//  {
//    /* Check if the Power On Reset flag is set */
//    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
//    {
//      printf("\r\n Power On Reset occurred....\r\n");
//    }
//    /* Check if the Pin Reset flag is set */
//    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
//    {
//      printf("\r\n External Reset occurred....\r\n");
//    }

//    printf("\r\n No need to configure RTC....\r\n");
//    
//    /* Enable the PWR clock */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

//    /* Allow access to RTC */
//    PWR_BackupAccessCmd(ENABLE);

//    /* Wait for RTC APB registers synchronisation */
//    RTC_WaitForSynchro();


//    /* Clear the RTC Alarm Flag */
//    RTC_ClearFlag(RTC_FLAG_ALRAF);

//    /* Display the RTC Time/Date and TimeStamp Time/Date */
//    RTC_TimeShow();
//    RTC_DateShow();
//    RTC_TimeStampShow();
//  } 

	RTC_Config();
	
	// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = 16;
	RTC_TimeStructure.RTC_Minutes = 46;
	RTC_TimeStructure.RTC_Seconds = 50;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = 1;
	RTC_DateStructure.RTC_Date = 29;
	RTC_DateStructure.RTC_Month = 3;
	RTC_DateStructure.RTC_Year = 16;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	
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

// ѡ�� RTC ʱ��Դ������ͬ��/�첽Ԥ��Ƶ����ֵ
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI
void RTC_Config(void)
{  
  RTC_InitTypeDef RTC_InitStructure;
	// ʹ�� PWR ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  // PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ���
  PWR_BackupAccessCmd(ENABLE);
	
/*=========================ѡ��RTCʱ��Դ==============================*/
#if defined (RTC_CLOCK_SOURCE_LSI)
  // ʹ��LSI	
  RCC_LSICmd(ENABLE);
  // �ȴ�LSI�ȶ� 
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  // ѡ��LSI��ΪRTC��ʱ��Դ
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE) 
  //ʹ��LSE
  RCC_LSEConfig(RCC_LSE_ON);
  // �ȴ� LSE �ȶ�  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
		printf("\n\r LSE ����ʧ�� \r\n");
  }
  // ѡ��LSE��ΪRTC��ʱ��Դ
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  printf("\n\r LSE �����ɹ� \r\n");  
#else
  #error Please select the RTC Clock source inside the main.c file
#endif 

  // ʹ�� RTC ʱ��
  RCC_RTCCLKCmd(ENABLE);
  // �ȴ� RTC APB �Ĵ���ͬ��
  RTC_WaitForSynchro();

/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	// ����������ʱ��ck_spare = LSE/(255+1)*(127+) = 1HZ
	// ����ͬ��Ԥ��Ƶ����ֵΪ255
	RTC_InitStructure.RTC_SynchPrediv = 255;
	// �����첽Ԥ��Ƶ����ֵ
	RTC_InitStructure.RTC_AsynchPrediv = 127;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	// ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ���
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ��ʼ��ʧ�� \r\n");
	}
}

///**
//  * @brief  Returns the time entered by user, using Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_TimeRegulate(void)
//{
//  uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
//  
//  printf("\r\n==============Time Settings=====================================\r\n");
//  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
//  printf("\r\n  Please Set Hours (00-23)");
//  while (tmp_hh == 0xFF)
//  {
//    tmp_hh = USART_Scanf(0, 23);
//    RTC_TimeStructure.RTC_Hours = tmp_hh;
//  }
//  printf(":  %0.2d\n", tmp_hh);
//  
//  printf("\r\n  Please Set Minutes (00-59)");
//  while (tmp_mm == 0xFF)
//  {
//    tmp_mm = USART_Scanf(0, 59);
//    RTC_TimeStructure.RTC_Minutes = tmp_mm;
//  }
//  printf(":  %0.2d\n", tmp_mm);
//  
//  printf("\r\n  Please Set Seconds (00-59)");
//  while (tmp_ss == 0xFF)
//  {
//    tmp_ss = USART_Scanf(0, 59);
//    RTC_TimeStructure.RTC_Seconds = tmp_ss;
//  }
//  printf(":  %0.2d\n", tmp_ss);

//  /* Configure the RTC time register */
//  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
//  {
//    printf("\r\n>> !! RTC Set Time failed. !! <<\r\n");
//  } 
//  else
//  {
//    printf("\r\n>> !! RTC Set Time success. !! <<\r\n");
//    RTC_TimeShow();
//    /* Indicator for the RTC configuration */
//    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
//  }
//  
//  tmp_hh = 0xFF;
//  tmp_mm = 0xFF;
//  tmp_ss = 0xFF;

//  printf("\r\n==============Date Settings=====================================\r\n");

//  printf("\r\n  Please Set WeekDay (01-07)");
//  while (tmp_hh == 0xFF)
//  {
//    tmp_hh = USART_Scanf(1, 7);
//    RTC_DateStructure.RTC_WeekDay = tmp_hh;
//  }
//  printf(":  %0.2d\n", tmp_hh);
//  tmp_hh = 0xFF;
//  printf("\r\n  Please Set Date (01-31)");
//  while (tmp_hh == 0xFF)
//  {
//    tmp_hh = USART_Scanf(1, 31);
//    RTC_DateStructure.RTC_Date = tmp_hh;
//  }
//  printf(":  %0.2d\n", tmp_hh);
//  
//  printf("\r\n  Please Set Month (01-12)");
//  while (tmp_mm == 0xFF)
//  {
//    tmp_mm = USART_Scanf(1, 12);
//    RTC_DateStructure.RTC_Month = tmp_mm;
//  }
//  printf(":  %0.2d\n", tmp_mm);
//  
//  printf("\r\n  Please Set Year (00-99)");
//  while (tmp_ss == 0xFF)
//  {
//    tmp_ss = USART_Scanf(0, 99);
//    RTC_DateStructure.RTC_Year = tmp_ss;
//  }
//  printf(":  %0.2d\n", tmp_ss);

//  /* Configure the RTC date register */
//  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
//  {
//    printf("\r\n>> !! RTC Set Date failed. !! <<\r\n");
//  } 
//  else
//  {
//    printf("\r\n>> !! RTC Set Date success. !! <<\r\n");
//    RTC_DateShow();
//    /* Indicator for the RTC configuration */
//    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
//  }

//}

///**
//  * @brief  Display the current time on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_TimeShow(void)
//{
//  
//  /* Get the current Time and Date */
//  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//  printf("\r\n\r\n============== Current Time Display ============================\n");
//  printf("\r\n  The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
//  /* Unfreeze the RTC DR Register */
//  (void)RTC->DR;
//}

///**
//  * @brief  Display the current date on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_DateShow(void)
//{
//  /* Get the current Date */
//  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//  printf("\r\n============== Current Date Display ============================\n");
//  printf("\r\n  The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
//}


///**
//  * @brief  Display the current TimeStamp (time and date) on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_TimeStampShow(void)
//{
//  /* Get the current TimeStamp */
//  RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
//  printf("\r\n==============TimeStamp Display (Time and Date)=================\n");
//  printf("\r\n  The current time stamp time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n", RTC_TimeStampStructure.RTC_Hours, RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
//  printf("\r\n  The current timestamp date (WeekDay-Date-Month) is :  %0.2d-%0.2d-%0.2d \n", RTC_TimeStampDateStructure.RTC_WeekDay, RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
//}


///**
//  * @brief  Gets numeric values from the hyperterminal.
//  * @param  MinValue: minimum value to be used.
//  * @param  MaxValue: maximum value to be used.
//  * @retval None
//  */
//uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue)
//{
//  uint32_t index = 0;
//  uint32_t tmp[2] = {0, 0};

//  while (index < 2)
//  {
//    /* Loop until RXNE = 1 */
//    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
//    {}
//    tmp[index++] = (USART_ReceiveData(USART1));
//    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
//    {
//      printf("\r\n Please enter valid number between 0 and 9\n");
//      index--;
//    }
//  }
//  /* Calculate the Corresponding value */
//  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
//  /* Checks the value */
//  if ((index < MinValue) || (index > MaxValue))
//  {
//    printf("\r\n Please enter valid number between %02d and %02d\n", MinValue, MaxValue);
//    return 0xFF;
//  }
//  return index;
//}

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
