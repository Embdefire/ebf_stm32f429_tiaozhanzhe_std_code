/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   OV5640����ͷ��ʾ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32  F429������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./camera/bsp_ov5640.h"
#include "./systick/bsp_SysTick.h"
#include "./camera/ov5640_AF.h"


/*���������*/
uint32_t Task_Delay[NumOfTask];

uint8_t dispBuf[100];
OV5640_IDTypeDef OV5640_Camera_ID;

uint8_t fps=0;

//��ʾ֡�����ݣ�Ĭ�ϲ���ʾ����Ҫ��ʾʱ�����������Ϊ1���ɣ���������΢�������󣡣�
//������Һ����ʾ����ͷ����800*480���أ�֡��Ϊ14.2֡/�롣
#define FRAME_RATE_DISPLAY 	1




/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
	/*����ͷ��RGB LED�ƹ������ţ���Ҫͬʱʹ��LED������ͷ*/
	
  Debug_USART_Config();   
	
	/* ����SysTick Ϊ10us�ж�һ��,ʱ�䵽�󴥷���ʱ�жϣ�
	*����stm32fxx_it.c�ļ���SysTick_Handler����ͨ�����жϴ�����ʱ
	*/
	SysTick_Init();

	
	/*��ʼ��Һ����*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(TRANSPARENCY);
	
//	LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);

//	LCD_ClearLine(LINE(18));
//  LCD_DisplayStringLine_EN_CH(LINE(18),(uint8_t* )" ģʽ:WVGA 800x480");

  CAMERA_DEBUG("STM32F429 DCMI ����OV5640����");
	

  /* ��ʼ������ͷGPIO��IIC */
  OV5640_HW_Init();   

  /* ��ȡ����ͷоƬID��ȷ������ͷ�������� */
  OV5640_ReadID(&OV5640_Camera_ID);

   if(OV5640_Camera_ID.PIDH  == 0x56)
  {
//    sprintf((char*)dispBuf, "              OV5640 ����ͷ,ID:0x%x", OV5640_Camera_ID.PIDH);
//		LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*)dispBuf);
    CAMERA_DEBUG("%x %x",OV5640_Camera_ID.PIDH ,OV5640_Camera_ID.PIDL);

  }
  else
  {
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*) "         û�м�⵽OV5640�������¼�����ӡ�");
    CAMERA_DEBUG("û�м�⵽OV5640����ͷ�������¼�����ӡ�");

    while(1);  
  }

  
  OV5640_Init();
  
  OV5640_RGB565Config();
  OV5640_AUTO_FOCUS();
  
	//ʹ��DCMI�ɼ�����
  DCMI_Cmd(ENABLE); 
  DCMI_CaptureCmd(ENABLE); 	



	/*DMAֱ�Ӵ�������ͷ���ݵ�LCD��Ļ��ʾ*/
  while(1)
	{
//��ʾ֡�ʣ�Ĭ�ϲ���ʾ		
#if FRAME_RATE_DISPLAY		
		if(Task_Delay[0]==0)
		{
						
			LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);

			LCD_ClearLine(LINE(17));
			sprintf((char*)dispBuf, " ֡��:%.1f/s", (float)fps/5.0);
			
			/*���֡��*/
			LCD_DisplayStringLine_EN_CH(LINE(17),dispBuf);
			//����
			fps =0;
			
            
            
			Task_Delay[0]=5000; //��ֵÿ1ms���1������0�ſ������½�������


		}
			
#endif
		
	}




}



/*********************************************END OF FILE**********************/

