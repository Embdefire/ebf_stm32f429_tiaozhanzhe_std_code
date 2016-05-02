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

#define USE_SPI_FLASH        0

#define XBF_XINSONGTI25      1
#define XBF_XINSONGTI19      1
#define UNIGBK               1
#define HZLIB                1
#define SD8782               1
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT result; 
UINT  bw;            					    /* File R/W count */

/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */
static void delay(void)
{
  uint32_t i,j;
  for(i=0;i<20000;i++)
    for(j=0;j<10000;j++)
    {      
    }
}
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

/**
  * @brief  ������
  * @param  ��
  * @retval ��
	* @note    ��ȡSD��XBF�ֿ��ļ�(eg.xinsongti25.xbf)������д��SPI Flash���ļ�ϵͳ
	*          �����С�
  */
int main(void)
{
	uint32_t write_addr=0,j=0;
	uint8_t tempbuf[256]={0};
	BL8782_PDN_INIT();
	/* ��ʼ��LED */
	LED_GPIO_Config();
	delay();
  
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();
	
  printf("\r\n this is a fatfs test demo \r\n");

#if USE_SPI_FLASH==0
	TM_FATFS_FLASH_SPI_disk_initialize();
#endif //#if USE_SPI_FLASH==0
  
/*************************   �ļ�ϵͳ   *********************************************/
	//�����ļ�ϵͳ
#if USE_SPI_FLASH
	result = f_mount(&fs,"1:",1);
#else
  result = f_mount(&fs,"0:",1);
#endif //#if USE_SPI_FLASH
  
	printf("\r\nf_mount result=%d \r\n",result);
	
	//����ļ�ϵͳ����ʧ�ܾ�ͣ��
	if(result != FR_OK)
	{
		while(1);
	}

/**************************  ������25.xbf  *****************************************/		
#if XBF_XINSONGTI25
#if USE_SPI_FLASH
	result = f_open(&fnew,"1:������25.xbf",FA_OPEN_EXISTING | FA_READ);
#else
  result = f_open(&fnew,"0:������25.xbf",FA_OPEN_EXISTING | FA_READ);
#endif  //#if USE_SPI_FLASH
  
	printf("f_open ������25.xbf res=%d\n",result);
	if(result==FR_OK)
  {
	  write_addr=60*4096;
	  for(j=0;j<649;j++)//������������ʼλ��60*4096��2596KB
	  {
	   	SPI_FLASH_SectorErase(write_addr+j*4096);
	  }
	  j=0;
	  write_addr=60*4096;
	  while(result == FR_OK) 
	  {
		  result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
		  if(result!=FR_OK)break;			 //ִ�д���  
		  SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
		  write_addr+=256;				
		  j++;
		  if(bw !=256)break;
	  } 
  }
	f_close(&fnew);
	
	SPI_FLASH_BufferRead(tempbuf,60*4096,100);			//��ȡ���ݣ���ӡ��֤
	printf("readbuf ������25.xbf\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n���tempbuf��ȫΪ FF ����ô˵���ֿ⿽���ɹ�������\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED1_ON;
#endif  //#if XBF_XINSONGTI25
/**************************  END ������25.xbf  *****************************************/  
  
  

/**************************  ������19.xbf  *****************************************/		
#if XBF_XINSONGTI19
#if USE_SPI_FLASH
	result = f_open(&fnew,"1:������19.xbf",FA_OPEN_EXISTING | FA_READ);
#else
  result = f_open(&fnew,"0:������19.xbf",FA_OPEN_EXISTING | FA_READ);
#endif  //#if USE_SPI_FLASH
  
	printf("f_open ������19.xbf res=%d\n",result);
	if(result==FR_OK)
  {
	  write_addr=710*4096;
	  for(j=0;j<529;j++)//������������ʼλ��710*4096��2116KB
	  {
	   	SPI_FLASH_SectorErase(write_addr+j*4096);
	  }
	  j=0;
	  write_addr=710*4096;
	  while(result == FR_OK) 
	  {
		  result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
		  if(result!=FR_OK)break;			 //ִ�д���  
		  SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
		  write_addr+=256;				
		  j++;
		  if(bw !=256)break;
	  } 
  }
	f_close(&fnew);
	
	SPI_FLASH_BufferRead(tempbuf,710*4096,100);			//��ȡ���ݣ���ӡ��֤
	printf("readbuf ������19.xbf\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n���tempbuf��ȫΪ FF ����ô˵���ֿ⿽���ɹ�������\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED2_ON;
#endif  //#if XBF_XINSONGTI19
/**************************  END ������19.xbf  *****************************************/ 
  


/**************************  UNIGBK.BIN  *****************************************/		
#if UNIGBK
#if USE_SPI_FLASH
	result = f_open(&fnew,"1:UNIGBK.BIN",FA_OPEN_EXISTING | FA_READ);
#else
  result = f_open(&fnew,"0:UNIGBK.BIN",FA_OPEN_EXISTING | FA_READ);
#endif  //#if USE_SPI_FLASH
  
	printf("f_open UNIGBK.BIN res=%d\n",result);
	if(result==FR_OK)
  {
	  write_addr=1240*4096;
	  for(j=0;j<43;j++)//������������ʼλ��1240*4096��172KB
	  {
	   	SPI_FLASH_SectorErase(write_addr+j*4096);
	  }
	  j=0;
	  write_addr=1240*4096;
	  while(result == FR_OK) 
	  {
		  result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
		  if(result!=FR_OK)break;			 //ִ�д���  
		  SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
		  write_addr+=256;				
		  j++;
		  if(bw !=256)break;
	  } 
  }
	f_close(&fnew);
	
	SPI_FLASH_BufferRead(tempbuf,1240*4096+174344/2,100);			//��ȡ���ݣ���ӡ��֤
	printf("readbuf UNIGBK.BIN\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n���tempbuf��ȫΪ FF ����ô˵���ֿ⿽���ɹ�������\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED3_ON;
#endif  //#if UNIGBK
/**************************  END UNIGBK.BIN  *****************************************/  
  
  
/**************************  HZLIB.bin  *****************************************/
#define START_BLOCK 1360			//��ʼλ1360*4096
#define BLOCK_NUM  144				//144*4096 = 576KB
	
#if HZLIB
#if USE_SPI_FLASH
	result = f_open(&fnew,"1:GB2312_H2424.FON",FA_OPEN_EXISTING | FA_READ);
#else
  result = f_open(&fnew,"0:GB2312_H2424.FON",FA_OPEN_EXISTING | FA_READ);
#endif  //#if USE_SPI_FLASH
  
	printf("f_open HZLIB.bin res=%d\n",result);
	if(result==FR_OK)
  {
	  write_addr=START_BLOCK*4096;//��ʼλ��
	  for(j=0;j<BLOCK_NUM;j++)//����������������С
	  {
	   	SPI_FLASH_SectorErase(write_addr+j*4096);
	  }
	  j=0;
	  write_addr=START_BLOCK*4096;
	  while(result == FR_OK) 
	  {
		  result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
		  if(result!=FR_OK)break;			 //ִ�д���  
		  SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
		  write_addr+=256;				
		  j++;
		  if(bw !=256)break;
	  } 
  }
	f_close(&fnew);
	
	SPI_FLASH_BufferRead(tempbuf,START_BLOCK*4096,100);			//��ȡ���ݣ���ӡ��֤
	printf("readbuf HZLIB.bin\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n���tempbuf��ȫΪ FF ����ô˵���ֿ⿽���ɹ�������\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED3_ON;
#endif  //#if HZLIB
/**************************  END HZLIB.bin  *****************************************/  
  

/**************************  HZLIB.bin  *****************************************/		
#if SD8782
#if USE_SPI_FLASH
	result = f_open(&fnew,"1:sd8782_uapsta.bin",FA_OPEN_EXISTING | FA_READ);
#else
  result = f_open(&fnew,"0:sd8782_uapsta.bin",FA_OPEN_EXISTING | FA_READ);
#endif  //#if USE_SPI_FLASH
  
	printf("f_open sd8782_uapsta.bin res=%d\n",result);
	if(result==FR_OK)
  {
	  write_addr=1290*4096;
	  for(j=0;j<61;j++)//������������ʼλ��1240*4096��172KB
	  {
	   	SPI_FLASH_SectorErase(write_addr+j*4096);
	  }
	  j=0;
	  write_addr=1290*4096;
	  while(result == FR_OK) 
	  {
		  result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
		  if(result!=FR_OK)break;			 //ִ�д���  
		  SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
		  write_addr+=256;				
		  j++;
		  if(bw !=256)break;
	  } 
  }
	f_close(&fnew);
	
	SPI_FLASH_BufferRead(tempbuf,1300*4096,100);			//��ȡ���ݣ���ӡ��֤
	printf("readbuf sd8782_uapsta.bin\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n���tempbuf��ȫΪ FF ����ô˵���ֿ⿽���ɹ�������\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED3_ON;
#endif  //#if SD8782
/**************************  END HZLIB.bin  *****************************************/  
  
		
	//����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ
#if USE_SPI_FLASH
	result = f_mount(&fs,"1:",0);
#else
  result = f_mount(&fs,"0:",0);
#endif
	while(1)
	{
	}

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
