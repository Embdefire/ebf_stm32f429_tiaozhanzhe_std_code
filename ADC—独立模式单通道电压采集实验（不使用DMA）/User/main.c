/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC�ɼ���ѹ��ADC�ж�ģʽ����DMAģʽ
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
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./led/bsp_led.h"


// ADCת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
__IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_Vol; 

static void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

		LED_GPIO_Config();
    /*��ʼ��USART1*/
    Debug_USART_Config();
 
		/* ��ʼ�������������õ���ADC��ADC�ɼ���ɻ����ADC�жϣ�
		��stm32f10x_it.c�ļ��е��жϷ���������ADC_ConvertedValue��ֵ */
    Rheostat_Init();
  
    printf("\r\n ����һ��ADC ��ѹ�ɼ� ʵ��(��ʹ��DMA)\r\n");
	
    
    while (1)
    {
    
      printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
      printf("\r\n The current AD value = %f V \r\n",ADC_Vol);     

			ADC_Vol =(float) ADC_ConvertedValue/4096*(float)3.3; // ��ȡת����ADֵ


      Delay(0xffffee);  
    }
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

