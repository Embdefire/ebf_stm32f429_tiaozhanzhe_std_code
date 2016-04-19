#ifndef __BREATH_LED_H
#define	__BREATH_LED_H

#include "stm32f4xx.h"

//���Ŷ���
/*******************************************************/
//R ��ɫ��
#define BREATH_LED1_PIN                  GPIO_Pin_10                 
#define BREATH_LED1_GPIO_PORT            GPIOH                      
#define BREATH_LED1_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define BREATH_LED1_PINSOURCE						GPIO_PinSource10
#define BREATH_LED1_AF										GPIO_AF_TIM5



//G ��ɫ��
#define BREATH_LED2_PIN                  GPIO_Pin_11                 
#define BREATH_LED2_GPIO_PORT            GPIOH                      
#define BREATH_LED2_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define BREATH_LED2_PINSOURCE						GPIO_PinSource11
#define BREATH_LED2_AF										GPIO_AF_TIM5

//B ��ɫ��
#define BREATH_LED3_PIN                  GPIO_Pin_12                 
#define BREATH_LED3_GPIO_PORT            GPIOH                       
#define BREATH_LED3_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define BREATH_LED3_PINSOURCE						GPIO_PinSource12
#define BREATH_LED3_AF										GPIO_AF_TIM5

//Сָʾ�ƣ���ͨIO�ڣ�������PWM����
#define LED4_PIN                  GPIO_Pin_11                 
#define LED4_GPIO_PORT            GPIOD                       
#define LED4_GPIO_CLK             RCC_AHB1Periph_GPIOD



/*��ʱ��*/

#define BREATH_TIM           		TIM5
#define BREATH_TIM_CLK       		RCC_APB1Periph_TIM5

#define BREATH_LED1_CCRx								CCR1
#define BREATH_LED2_CCRx								CCR2
#define BREATH_LED3_CCRx								CCR3

#define BREATH_LED1_TIM_CHANNEL				TIM_Channel_1
#define BREATH_LED2_TIM_CHANNEL				TIM_Channel_2
#define BREATH_LED3_TIM_CHANNEL				TIM_Channel_3

#define BREATH_TIM_IRQn					TIM5_IRQn
#define BREATH_TIM_IRQHandler 	TIM5_IRQHandler
/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
//
#define BREATH_LED1(a)	if (a)	\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED1_TIM_CHANNEL,TIM_CCx_Disable);\
					else		\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED1_TIM_CHANNEL,TIM_CCx_Enable)

#define BREATH_LED2(a)	if (a)	\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED2_TIM_CHANNEL,TIM_CCx_Disable);\
					else		\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED2_TIM_CHANNEL,TIM_CCx_Enable)

#define BREATH_LED3(a)	if (a)	\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED3_TIM_CHANNEL,TIM_CCx_Disable);\
					else		\
					TIM_CCxCmd (BREATH_TIM,BREATH_LED3_TIM_CHANNEL,TIM_CCx_Enable)
		
//LED 4����ͨIO��					
#define LED4(a)	if (a)	\
					GPIO_SetBits(LED4_GPIO_PORT,LED4_PIN);\
					else		\
					GPIO_ResetBits(LED4_GPIO_PORT,LED4_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/*���ƺ����Ƶĺ�*/
#define BREATH_LED1_OFF				TIM_CCxCmd (BREATH_TIM,BREATH_LED1_TIM_CHANNEL,TIM_CCx_Disable);
#define BREATH_LED1_ON				TIM_CCxCmd (BREATH_TIM,BREATH_LED1_TIM_CHANNEL,TIM_CCx_Enable);

#define BREATH_LED2_OFF				TIM_CCxCmd (BREATH_TIM,BREATH_LED2_TIM_CHANNEL,TIM_CCx_Disable);
#define BREATH_LED2_ON				TIM_CCxCmd (BREATH_TIM,BREATH_LED2_TIM_CHANNEL,TIM_CCx_Enable);

#define BREATH_LED3_OFF				TIM_CCxCmd (BREATH_TIM,BREATH_LED3_TIM_CHANNEL,TIM_CCx_Disable);
#define BREATH_LED3_ON				TIM_CCxCmd (BREATH_TIM,BREATH_LED3_TIM_CHANNEL,TIM_CCx_Enable);

/* �������IO�ĺ� */
#define LED4_TOGGLE		digitalToggle(LED4_GPIO_PORT,LED4_PIN)
#define LED4_OFF				digitalHi(LED4_GPIO_PORT,LED4_PIN)
#define LED4_ON				digitalLo(LED4_GPIO_PORT,LED4_PIN)


/* ������ɫ������߼��÷�ʹ��PWM��ȫ��LED��ʵ�飩�ɻ��ȫ����ɫ,��Ч������ */

//��
#define BREATH_LED_RED  \
					BREATH_LED1_ON;\
					BREATH_LED2_OFF\
					BREATH_LED3_OFF

//��
#define BREATH_LED_GREEN		\
					BREATH_LED1_OFF;\
					BREATH_LED2_ON\
					BREATH_LED3_OFF

//��
#define BREATH_LED_BLUE	\
					BREATH_LED1_OFF;\
					BREATH_LED2_OFF\
					BREATH_LED3_ON

					
//��(��+��)					
#define BREATH_LED_YELLOW	\
					BREATH_LED1_ON;\
					BREATH_LED2_ON\
					BREATH_LED3_OFF
//��(��+��)
#define BREATH_LED_PURPLE	\
					BREATH_LED1_ON;\
					BREATH_LED2_OFF\
					BREATH_LED3_ON

//��(��+��)
#define BREATH_LED_CYAN \
					BREATH_LED1_OFF;\
					BREATH_LED2_ON\
					BREATH_LED3_ON
					
//��(��+��+��)
#define BREATH_LED_WHITE	\
					BREATH_LED1_ON;\
					BREATH_LED2_ON\
					BREATH_LED3_ON
					
//��(ȫ���ر�)
#define BREATH_LED_RGBOFF	\
					BREATH_LED1_OFF;\
					BREATH_LED2_OFF\
					BREATH_LED3_OFF
					




void BreathLED_Config(void);

#endif /* __BREATH_LED_H */
