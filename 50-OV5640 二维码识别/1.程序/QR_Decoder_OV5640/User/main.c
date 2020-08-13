/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����OV5640����ͷ��ά��ɨ������
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
#include "qr_decoder_user.h"
#include "./beep/bsp_beep.h"  
/*���������*/
uint32_t Task_Delay[NumOfTask];

uint8_t dispBuf[100];
OV5640_IDTypeDef OV5640_Camera_ID;

uint8_t beep_on_flag = 0;//������״̬��1��ʾ�������죬0��ʾ����������
/**
  * @brief  TIM2����10msʱ����ʼ������
  * @param  
  * @param  
  * @note 	
  */
void Time2_init()
{	 
  	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
  	TIM_TimeBaseStructure.TIM_Period = 10000;	//10000us=10ms
  	TIM_TimeBaseStructure.TIM_Prescaler = 90-1;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//�ж�ʹ��
  	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
/**
  * @brief  TIM4_IRQHandler:10msʱ���жϺ���
  * @param  
  * @param  
  * @note 	
  */
void Time2_IRQ()
{
    static u32 BeepTime=8;
         
    if(beep_on_flag)   
    {
	  BEEP_ON;
      if((--BeepTime) == 0)
      {
        BeepTime=8;
        beep_on_flag =0;
        BEEP_OFF;  
      }  
    }
}
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    char  qr_type_len=0;
    short qr_data_len=0;
    char qr_type_buf[10];
    char qr_data_buf[512];
    int addr=0;
    int i=0,j=0;
    char qr_num=0;
	/*����ͷ��RGB LED�ƹ������ţ���Ҫͬʱʹ��LED������ͷ*/
	
  Debug_USART_Config();   
	
	/* ����SysTick Ϊ10us�ж�һ��,ʱ�䵽�󴥷���ʱ�жϣ�
	*����stm32fxx_it.c�ļ���SysTick_Handler����ͨ�����жϴ�����ʱ
	*/
	SysTick_Init();

    BEEP_GPIO_Config();
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
   LCD_Clear_ARGB8888(LCD_COLOR_BLACK_ARGB8888); 
   //����͸����
   LCD_SetTextColor_ARGB8888(TRANSPARENCY_ARGB8888);
   LCD_DrawFullRect_ARGB8888(Frame_width,Frame_width);
   //����ɨ���
   LCD_View_Finder_ARGB8888(Frame_width,Frame_line_length,Frame_line_size,LCD_COLOR_GREEN_ARGB8888);

  CAMERA_DEBUG("STM32F429 ��ά���������");
	

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
		LCD_SetLayer(LCD_BACKGROUND_LAYER); 

    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t*) "         û�м�⵽OV5640�������¼�����ӡ�");
    CAMERA_DEBUG("û�м�⵽OV5640����ͷ�������¼�����ӡ�");

    while(1);  
  }

  
  OV5640_Init();
  
  OV5640_RGB565Config();
  OV5640_AUTO_FOCUS();
  
  //ʹ��DCMI�ɼ�����
  //DMAֱ�Ӵ�������ͷ���ݵ�LCD��Ļ��ʾ
  DCMI_Cmd(ENABLE); 
  DCMI_CaptureCmd(ENABLE);
  
  Time2_init();

  
  while(1)
	{
      //��ά��ʶ�𣬷���ʶ������ĸ���
      qr_num = QR_decoder();
       
       if(qr_num)
       {
           //ʶ��ɹ������������־
           beep_on_flag =1;
           
           //����������ǰ���ʶ������ĸ�����װ�õĶ�ά���飬��Щ������Ҫ
           //����ʶ������ĸ�������������ͨ�����ڷ��͵���λ�������ն�
           for(i=0;i < qr_num;i++)
           {
               qr_type_len = decoded_buf[i][addr++];//��ȡ�������ͳ���
               
               for(j=0;j < qr_type_len;j++)
                   qr_type_buf[j]=decoded_buf[i][addr++];//��ȡ������������
               
               qr_data_len  = decoded_buf[i][addr++]<<8; //��ȡ�������ݳ��ȸ�8λ
               qr_data_len |= decoded_buf[i][addr++];    //��ȡ�������ݳ��ȵ�8λ
               
               for(j=0;j < qr_data_len;j++)
                   qr_data_buf[j]=decoded_buf[i][addr++];//��ȡ��������
               
               uart_send_buf((unsigned char *)qr_type_buf, qr_type_len);//���ڷ��ͽ�������
               while(get_send_sta()); //�ȴ����ڷ������
               uart_send_buf((unsigned char *)":", 1);    //���ڷ��ͷָ���
               while(get_send_sta()); //�ȴ����ڷ������
               uart_send_buf((unsigned char *)qr_data_buf, qr_data_len);//���ڷ��ͽ�������
               while(get_send_sta()); //�ȴ����ڷ������
               uart_send_buf((unsigned char *)"\r\n", 2); //���ڷ��ͷָ���
               while(get_send_sta());//�ȴ����ڷ������
               addr =0;//����
           }
               
       } 
		
	}

}


/*********************************************END OF FILE**********************/

