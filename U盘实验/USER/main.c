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
#include "./Bsp/key/bsp_key.h"
#include "ff.h"
#include "./APP/usbh_bsp.h"

/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
FRESULT result;
FATFS fs;
FIL file;

KEY Key1,Key2;

/* �������ļ��ڵ��õĺ������� */
static void ViewRootDir(void);
static void ReadFileData(void);
/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* ��ʼ��LED */
	LED_GPIO_Config();
	Key1_GPIO_Config();
	Key2_GPIO_Config();
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();
	
  printf("\r\n this is a fatfs test demo \r\n");
	
	/* Init Host Library */
		USBH_Init(&USB_OTG_Core,
			USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		
		Key_RefreshState(&Key1);//ˢ�°���״̬
		Key_RefreshState(&Key2);//ˢ�°���״̬
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//���������¹�
		{
			LED2_TOGGLE;
			ViewRootDir();		/* ��ʾSD����Ŀ¼�µ��ļ��� */
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//���������¹�
		{
			LED3_TOGGLE;
			ReadFileData();		/* ��ȡ��Ŀ¼��test.txt������ */
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ViewRootDir
*	����˵��: ��ʾ��Ŀ¼�µ��ļ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ViewRootDir(void)
{
	DIR DirInf;
	FILINFO FileInf;
	uint32_t cnt = 0;
	char lfname[256];

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs,"0:",1);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
		return;
//    result=f_mkfs("0:",0,0);							//��ʽ��
//		printf("\r\nmkfs result=%d",result);
//		result = f_mount(&fs,"0:",0);						//��ʽ������ȡ������
//		printf("\r\n f_mount result=%d \r\n",result);
//		result = f_mount(&fs,"0:",1);						//���¹���
//		printf("\r\n f_mount result=%d \r\n",result);
	}
  
  printf("�����ļ�ϵͳ (%d)\r\n", result);
  
	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%d)\r\n", result);
		return;
	}
  
  printf("�򿪸�Ŀ¼ (%d)\r\n", result);
  
	/* ��ȡ��ǰ�ļ����µ��ļ���Ŀ¼ */
	FileInf.lfname = lfname;
	FileInf.lfsize = 256;

	printf("    ����    |  �ļ���С  |   ���ļ���   |  ���ļ���\r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf,&FileInf); 		/* ��ȡĿ¼��������Զ����� */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}

		if (FileInf.fname[0] == '.')
		{
			continue;
		}

		/* �ж����ļ�������Ŀ¼ */
		if (FileInf.fattrib & AM_DIR)
		{
			printf("(0x%02d)Ŀ¼  ", FileInf.fattrib);
		}
		else
		{
			printf("(0x%02d)�ļ�  ", FileInf.fattrib);
		}

		/* ��ӡ�ļ���С, ���4G */
		printf(" %10d ", FileInf.fsize);
		
		printf("  %s |", FileInf.fname);	/* ���ļ��� */
		
		printf("    %s\r\n", (char *)FileInf.lfname);	/* ���ļ��� */
	}

	/* ж���ļ�ϵͳ */
	f_mount(&fs,"0:",0);
}


/*
*********************************************************************************************************
*	�� �� ��: ReadFileData
*	����˵��: ��ȡ�ļ�test.txtǰ128���ַ�������ӡ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ReadFileData(void)
{	
	DIR DirInf;
	uint32_t bw;
	char buf[128];

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs,"0:",1);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ��(%d)\r\n", result);    
		return;    
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%d)\r\n", result);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "test.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : test.txt\r\n");
		return;
	}

	/* ��ȡ�ļ� */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\ntest.txt �ļ����� : \r\n%s\r\n", buf);
	}
	else
	{
		printf("\r\ntest.txt �ļ����� : \r\n");
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(&fs,"0:",0);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
