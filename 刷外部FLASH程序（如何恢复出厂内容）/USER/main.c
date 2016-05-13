/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��1.5.1�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "ff.h"
#include "./fatfs/drivers/fatfs_flash_spi.h"
#include "aux_data.h"


#define USE_SPI_FLASH        0

#define XBF_XINSONGTI25      1
#define XBF_XINSONGTI19      1
#define UNIGBK               1
#define GB2312               1
#define SD8782               1
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
extern FIL fnew;													/* file objects */
extern FATFS fs;													/* Work area (file system object) for logical drives */
extern FRESULT result; 
extern UINT  bw;            					    /* File R/W count */

/*
*********************************************************************************************************
* Description : ��ʼ��BL8782 wifiģ��ʹ�����ţ����ر�ʹ��
* Argument(s) : none.
* Return(s)   : none.
*********************************************************************************************************
*/
static void BL8782_PDN_INIT(void)
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_9);  //�ر�BL_8782wifiʹ��
}
/*
*********************************************************************************************************
* Description : ��ʼ��KEY1
* Argument(s) : none.
* Return(s)   : none.
*********************************************************************************************************
*/
static void KEY1_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  /*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  /*�������Ų�����Ҳ������*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/* ���������ٶ� */
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
  * @brief  ������
  * @param  ��
  * @retval ��
	* @note    ��ȡSD��XBF�ֿ��ļ�(eg.xinsongti25.xbf)������д��SPI Flash���ļ�ϵͳ
	*          �����С�
  */
int main(void)
{
  FRESULT res;
  
	BL8782_PDN_INIT();
	/* ��ʼ��LED */
  LED_GPIO_Config();
  KEY1_INIT();

  /* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
  Debug_USART_Config();
  TM_FATFS_FLASH_SPI_disk_initialize();

    
  printf("\r\n ��һ��KEY1��ʼ��д�ֿ� \r\n"); 
  
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1); 

  //��¼�ļ�    
   burn_file_sd2flash(burn_data,AUX_MAX_NUM);  

   while(1);


}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
