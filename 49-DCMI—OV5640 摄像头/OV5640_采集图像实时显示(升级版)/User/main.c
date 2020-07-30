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
#include "./key/bsp_key.h" 

/*���������*/
uint32_t Task_Delay[NumOfTask];

uint8_t dispBuf[100];
OV5640_IDTypeDef OV5640_Camera_ID;
uint8_t fps=0;


//��ʾ֡�����ݣ�Ĭ�ϲ���ʾ����Ҫ��ʾʱ�����������Ϊ1���ɣ���������΢�������󣡣�
//������Һ����ʾ����ͷ����800*480���أ�֡��Ϊ14.2֡/�롣
#define FRAME_RATE_DISPLAY 	1

static int mode_state = 1;//0--ԭʼ״̬��1--����״̬
void Camera_Mode_Config(void)
{
	cam_mode.frame_rate = FRAME_RATE_15FPS,	
	
	//ISP����
	cam_mode.cam_isp_sx = 0;
	cam_mode.cam_isp_sy = 0;	
	
	cam_mode.cam_isp_width = 1920;
	cam_mode.cam_isp_height = 1080;
	
	//�������
	cam_mode.scaling = 1;      //ʹ���Զ�����
	cam_mode.cam_out_sx = 16;	//ʹ���Զ����ź�һ�����ó�16����
	cam_mode.cam_out_sy = 4;	  //ʹ���Զ����ź�һ�����ó�4����
	cam_mode.cam_out_width = 800;
	cam_mode.cam_out_height = 480;
	
	//LCDλ��
	cam_mode.lcd_sx = 0;
	cam_mode.lcd_sy = 0;
	cam_mode.lcd_scan = 5; //LCDɨ��ģʽ��
	
	//���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���	
	cam_mode.light_mode = 0;//�Զ�����ģʽ
	cam_mode.saturation = 0;	
	cam_mode.brightness = 0;
	cam_mode.contrast = 0;
	cam_mode.effect = 0;		//����ģʽ
	cam_mode.exposure = 0;		

	cam_mode.auto_focus = 1;//�Զ��Խ�
}


void Camera_Mode_Reconfig(void)
{
	cam_mode.frame_rate = FRAME_RATE_15FPS,	
	
	//ISP����
	cam_mode.cam_isp_sx = 0;
	cam_mode.cam_isp_sy = 0;	
	
	cam_mode.cam_isp_width = 1920;
	cam_mode.cam_isp_height = 1080;
	
	//�������
	cam_mode.scaling = 1;      //ʹ���Զ�����
	cam_mode.cam_out_sx = 16;	//ʹ���Զ����ź�һ�����ó�16����
	cam_mode.cam_out_sy = 4;	  //ʹ���Զ����ź�һ�����ó�4����
	cam_mode.cam_out_width = 480;
	cam_mode.cam_out_height = 320
  ;
	
	//LCDλ��
	cam_mode.lcd_sx = 0;
	cam_mode.lcd_sy = 0;
	cam_mode.lcd_scan = 5; //LCDɨ��ģʽ��
	
	//���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���	
	cam_mode.light_mode = 0;//�Զ�����ģʽ
	cam_mode.saturation = 0;	
	cam_mode.brightness = 3;
	cam_mode.contrast = 0;
	cam_mode.effect = 0;		//����ģʽ
	cam_mode.exposure = 0;		

	cam_mode.auto_focus = 1;//�Զ��Խ�
}

extern __attribute__ ((at(0xD0100000))) uint32_t cam_buff0[800*480];
extern int cur_index;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  uint8_t focus_status = 0;
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
//  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
//	LCD_SetTransparency(0xFF);
//	LCD_Clear(LCD_COLOR_BLACK);
	
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

  
  
  
  OV5640_RGB565Config();
  OV5640_USER_Config();
  OV5640_FOCUS_AD5820_Init();
	OV5640_Init();
	if(cam_mode.auto_focus ==1)
	{
		OV5640_FOCUS_AD5820_Constant_Focus();
		focus_status = 1;
	}
	//ʹ��DCMI�ɼ�����
  DCMI_Cmd(ENABLE); 
  DCMI_CaptureCmd(ENABLE); 

	/*DMAֱ�Ӵ�������ͷ���ݵ�LCD��Ļ��ʾ*/
  while(1)
	{
    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )//�л���30FPS��320*240
    {      
      //�رղɼ�  
      CAMERA_DEBUG("K1");
      
      //OV5640_Capture_Control(DISABLE);
      DCMI_Stop();
//      //�޸�Cam mode ����
      if(mode_state == 1)
      {
        mode_state = 0;
        Camera_Mode_Reconfig();
      }
      else
      {
        mode_state = 1;
        Camera_Mode_Config();
      }
////      LCD_LayerInit_Cam(0, 400-cam_mode.cam_out_width/2, 400-cam_mode.cam_out_width/2 + cam_mode.cam_out_width,
////                        240-cam_mode.cam_out_height/2,240-cam_mode.cam_out_height/2+cam_mode.cam_out_height,
////                        LCD_FB_START_ADDRESS,RGB565); 
//      HAL_DCMI_Start_DMA((uint32_t)cam_buff0,
//                        cam_mode.cam_out_height*cam_mode.cam_out_width/2);       
      //LCD_LayerCamInit(LCD_FRAME_BUFFER,cam_mode.cam_out_width, cam_mode.cam_out_height);      
      OV5640_USER_Config();

      if(cam_mode.auto_focus ==1)
      {
        OV5640_AUTO_FOCUS();
        focus_status = 1;
      }
      DCMI_Start(); 
      /*DMAֱ�Ӵ�������ͷ���ݵ�LCD��Ļ��ʾ*/
      //OV5640_Init();		
//      OV5640_Capture_Control(ENABLE);
    }     
      if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
      {
        if(focus_status == 1)
        {
          //��ͣ�Խ�
          OV5640_FOCUS_AD5820_Pause_Focus();
          focus_status = 0 ;
        }
        else
        {
          //�Զ��Խ�
          OV5640_AUTO_FOCUS();
          focus_status = 1 ;
        }
      }     
//��ʾ֡�ʣ�Ĭ�ϲ���ʾ		
#if FRAME_RATE_DISPLAY		
		if(Task_Delay[0]==0)
		{
			/*���֡��*/
			CAMERA_DEBUG("\r\n֡��:%.1f/s \r\n", (double)fps/5.0);
			//����
			fps =0;			
			
			Task_Delay[0]=5000; //��ֵÿ1ms���1������0�ſ������½�������


		}
			
#endif
		
	}




}



/*********************************************END OF FILE**********************/

