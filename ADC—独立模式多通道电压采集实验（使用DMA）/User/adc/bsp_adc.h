#ifndef __ADC_H
#define	__ADC_H


#include "stm32f4xx.h"

// RHEOSTAT: ����������/��λ��
// ��ͨ��ADC�ɼ�


//�������� PC3 ͨ����ñ���� ����������
//         PA4 ͨ����ñ���� ��������
//         PA6 ���գ���ͨ���Ű��߽�3V3����GND��ʵ��
//ת����ͨ������
//����ʹ�� ADC1��ͨ��13-PC3��ͨ��4-PA4��ͨ��6-PA6
#define RHEOSTAT_NOFCHANEL      3

// ADC �����غ궨��
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
#define RHEOSTAT_ADC_DR_ADDR      ((u32)ADC1+0x4c)

/*=====================ͨ��1 IO======================*/
// PC3 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_3
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL1      ADC_Channel_13
/*=====================ͨ��2 IO ======================*/
// PA4 ͨ����ñ�ӹ�������
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_4
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL2      ADC_Channel_4
/*=====================ͨ��3 IO ======================*/
// PA6 ���գ����öŰ��߽�3V3����GND��ʵ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT3    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN3     GPIO_Pin_6
#define RHEOSTAT_ADC_GPIO_CLK3     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL3     ADC_Channel_6


// DMA2 ������0 ͨ��0
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0
 

void Rheostat_Init(void);


#endif /* __ADC_H */

