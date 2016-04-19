#ifndef __ADC_H
#define	__ADC_H


#include "stm32f4xx.h"

// RHEOSTAT: ����������/��λ��

// ��λ�������ADC IO��ֻҪ�޸���Щ�꼴�ɣ�1��Ч��0��Ч��������Ĭ�Ͻ�PC3
// ע��PF6 PF7 PF8 PF9�ڿ��������Ѿ������������ط�����������ADCʵ��
// �����ṩ����ֻ��Ϊ����ʾ����������Լ�����Ŀ���⼸��IO��û�и��ã����������ʹ�� 
#define RHEOSTAT_PC3     1
#define RHEOSTAT_PF6     0
#define RHEOSTAT_PF7     0
#define RHEOSTAT_PF8     0
#define RHEOSTAT_PF9     0

#if     RHEOSTAT_PC3 //PC3 �ӵ�λ��

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_3
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOC

// ADC ��ź궨��
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_13

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#elif   RHEOSTAT_PF6 // PF6 �ӵ�λ��
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC3+0x4c)

#define RHEOSTAT_ADC_GPIO_PORT    GPIOF
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_6
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOF

#define RHEOSTAT_ADC              ADC3
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC3
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_4

#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_2
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#elif   RHEOSTAT_PF7 // PF7 �ӵ�λ��
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC3+0x4c)

#define RHEOSTAT_ADC_GPIO_PORT    GPIOF
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_7
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOF

#define RHEOSTAT_ADC              ADC3
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC3
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_5

#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_2
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#elif   RHEOSTAT_PF8 // PF8 �ӵ�λ��
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC3+0x4c)

#define RHEOSTAT_ADC_GPIO_PORT    GPIOF
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_8
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOF

#define RHEOSTAT_ADC              ADC3
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC3
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_6

#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_2
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#elif   RHEOSTAT_PF9 // PF9 �ӵ�λ��
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC3+0x4c)

#define RHEOSTAT_ADC_GPIO_PORT    GPIOF
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_9
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOF

#define RHEOSTAT_ADC              ADC3
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC3
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_7

#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_2
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#endif   

void Rheostat_Init(void);


#endif /* __ADC_H */

