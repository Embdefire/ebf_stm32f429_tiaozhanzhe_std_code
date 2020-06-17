/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO���--ʹ�ù̼������LED��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h" 
#include "./sdram/bsp_sdram.h"
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./malloc/malloc.h"
#include "./nand/ftl.h"
#include "./nand/bsp_nand.h"
#include "./nand/nandtest.h"
#include "./key/bsp_key.h" 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
uint8_t CNT = 0;
int main(void)
{
  uint8_t *buf;
	uint8_t *backbuf;	
  
  HAL_Init();

  /* ϵͳʱ�ӳ�ʼ����180 MHz */
  SystemClock_Config();
  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();	 
  /* ��ʼ������ */
  DEBUG_USART_Config();

  printf("\r\nҰ��STM32F429 NAND FLASH ��д��������\r\n");
    
	/*��ʼ��SDRAMģ��*/
	SDRAM_Init();
	/*��ʼ���ⲿ�ڴ��*/
	my_mem_init(SRAMEX);		
	/*����������ʾ���ڶ�д NAND FLASH ����*/
	LED_BLUE;
  
	while(FTL_Init())	//���NAND FLASH,����ʼ��FTL
	{
		printf("NAND Error!");
		HAL_Delay(500);				 
		printf("Please Check");
		HAL_Delay(500);				 
		LED1_TOGGLE;//�����˸
	}
  backbuf=mymalloc(SRAMEX,NAND_ECC_SECTOR_SIZE);	//����һ�������Ļ���
	buf=mymalloc(SRAMIN,NAND_ECC_SECTOR_SIZE);		//����һ�������Ļ���
	sprintf((char*)buf,"NAND Size:%dMB",(nand_dev.block_totalnum/1024)*(nand_dev.page_mainsize/1024)*nand_dev.block_pagenum);
	printf((char*)buf);	//��ʾNAND����  
	FTL_ReadSectors(backbuf,2,NAND_ECC_SECTOR_SIZE,1);//Ԥ�ȶ�ȡ����0����������,��ֹ��д�����ļ�ϵͳ��.
  
	while(1)
	{
    CNT++;
    if(CNT >= 255)
      CNT = 1;
    if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      /* д֮ǰ��Ҫ���� */
      NAND_EraseBlock(0);
      test_writepage(32,0,255,CNT);
      printf("write: %d\n", CNT);
    }
    if(Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
    {
      test_readpage(32,0,255);//0x40
    }
	}		
}

/**
  * @brief  ϵͳʱ������ 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  ��
  * @retval ��
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* ʹ��HSE������HSEΪPLL��ʱ��Դ������PLL�ĸ��ַ�Ƶ����M N P Q 
   * PLLCLK = HSE/M*N/P = 25M / 25 *360 / 2 = 180M
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }
  /* ���� OverDrive ģʽ */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1);
  }
  /* ѡ��PLLCLK��ΪSYSCLK�������� HCLK, PCLK1 and PCLK2 ��ʱ�ӷ�Ƶ���� 
	 * SYSCLK = PLLCLK     = 180M
	 * HCLK   = SYSCLK / 1 = 180M
	 * PCLK2  = SYSCLK / 2 = 90M
	 * PCLK1  = SYSCLK / 4 = 45M
	 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
