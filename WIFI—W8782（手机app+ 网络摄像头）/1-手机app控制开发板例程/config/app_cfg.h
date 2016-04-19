#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "type.h"

#define SUPPORT_AUDIO			0	
#define SUPPORT_WEBSERVER		1	//������37k flash
#define MACHTALK_ENABLE		0	//MACHTALKƽ̨Զ�̿���

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
* UCOSÿ���߳����ȼ�������ͬ����������ͳһ����
*********************************************************************************************************
*/
#define WORK_QUEUE_MAX_SIZE 4
#ifdef OS_UCOS
enum TASK_PRIO{
TASK_UNUSED_PRIO = 10,
TASK_MOAL_WROK_PRIO,
TASK_SDIO_THREAD_PRIO,	
TASK_ETH_INT_PRIO,
OS_TASK_TMR_PRIO,
TASK_TCPIP_THREAD_PRIO,
TASK_TIMER_TASKLET_PRIO,
TASK_MONITOR_PRIO,
REASSOCIATION_THREAD_PRIO, 
//����˳��������Ķ�

//user task
TASK_MAIN_PRIO,
TASK_IMG_SEND_PRIO,
TASK_ADC_RECV_PKG_PRIO,
TASK_TCP_RECV_PRIO,
TASK_TCP_ACCEPT_PRIO,
TASK_WEB_PRIO,
TASK_DHCP_RECV_PRIO,
TASK_SUB_MQTT_PRIO,
TASK_TCP_SEND_PRIO,






TASK_TCP_SERVER_PRIO,
TASK_TCP_CLIENT_PRIO,
TASK_CLIENT_SOCK_STATS_PRIO,


};
#endif

#ifdef OS_FREE_RTOS
enum TASK_PRIO{
//user task
TASK_TCP_SEND_PRIO = 2,
TASK_WEB_PRIO,
TASK_TCP_ACCEPT_PRIO,
TASK_TCP_RECV_PRIO,
TASK_ADC_RECV_PKG_PRIO,
TASK_MAIN_PRIO,
TASK_IMG_SEND_PRIO,
TASK_SUB_MQTT_PRIO,

//����
REASSOCIATION_THREAD_PRIO, 
TASK_MONITOR_PRIO,
TASK_TIMER_TASKLET_PRIO,
TASK_TCPIP_THREAD_PRIO,
TASK_ETH_INT_PRIO,
TASK_SDIO_THREAD_PRIO,	
TASK_MOAL_WROK_PRIO,

OS_TASK_TMR_PRIO,
TASK_UNUSED_PRIO,
};
#endif



///*
//*********************************************************************************************************
//*                                            TASK STACK SIZES
//*��λWORD
//*********************************************************************************************************
//*/
//#define TASK_TCPIP_THREAD_STACK_SIZE		512*2
//#define TIMER_TASKLET_STACK_SIZE			256*2
//#define MOAL_WROK_STACK_SIZE				256*2
//#define TASK_MONITOR_STACK_SIZE				128*2
//#define REASSOCIATION_THREAD_STACK_SIZE     400*2

////������ֵ������Ķ�

//#define TASK_MAIN_STACK_SIZE				9192
//#define TASK_ADC_RECV_PKG_STACK_SIZE		256*2
//#if ENABLE_MP3
//#define TASK_TCP_RECV_STACK_SIZE			(1024 + 512)
//#else
//#define TASK_TCP_RECV_STACK_SIZE			512*2
//#endif
//#define TASK_TCP_SEND_STACK_SIZE			256*2
//#define TASK_ACCEPT_STACK_SIZE				256*2
//#define TASK_WEB_STK_SIZE					256*2
//#define TASK_IMG_SEND_STACK_SIZE			512
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*��λWORD
*********************************************************************************************************
*/
#define TASK_TCPIP_THREAD_STACK_SIZE		512
#define TIMER_TASKLET_STACK_SIZE			256
#define MOAL_WROK_STACK_SIZE				256
#define TASK_MONITOR_STACK_SIZE				128
#define REASSOCIATION_THREAD_STACK_SIZE     400

//������ֵ������Ķ�

#define TASK_MAIN_STACK_SIZE				9192
#define TASK_ADC_RECV_PKG_STACK_SIZE		256
#if ENABLE_MP3
#define TASK_TCP_RECV_STACK_SIZE			(1024 + 512)
#else
#define TASK_TCP_RECV_STACK_SIZE			512
#endif
#define TASK_TCP_SEND_STACK_SIZE			256
#define TASK_ACCEPT_STACK_SIZE				256
#define TASK_WEB_STK_SIZE					256
#define TASK_IMG_SEND_STACK_SIZE			512


#define TASK_TCP_SERVER_STACK_SIZE			256
#define TASK_TCP_CLIENT_STACK_SIZE			256
#define TASK_CLIENT_SOCK_STATS_STACK_SIZE  256
/*
*********************************************************************************************************
*                               		IRQ_PRIORITY
*����жϺ�������Ҫ��ӡ��Ϣ������ж����ȼ�Ҫ����uart�ж�
*********************************************************************************************************
*/

enum IRQ_PRIORITY{
	TIM2_IRQn_Priority,   //ir control
	DEBUG_USART_Priority = 1,
	USART3_IRQn_Priority,
	EXTI2_IRQn_Priority,
	ETH_IRQn_Priority,
	EXTI9_5_IRQn_Priority,
	EXTI15_10_IRQn_Priority,
	SDIO_IRQn_Priority,
	DEBUG_USART_DMA_RX_IRQn_Priority,
	DEBUG_USART_DMA_TX_IRQn_Priority,
	ADC_IRQn_Priority,
	DMA1_Stream6_IRQn_Priority,
	SPI3_IRQn_Priority,
	DMA2_Stream0_IRQn_Priority,
	TIM3_IRQn_Priority,
	TIM4_IRQn_Priority,
	DMA2_Stream1_IRQn_Priority,
	DCMI_IRQn_Priority,
	SysTick_IRQn_Priority
};

/*
*********************************************************************************************************
*                                      kernel ���泣����Դֵ����
*�������monitor���ȷ�����ʵ���ֵ
*********************************************************************************************************
*/
#define EVENTS_MAX			64		//EVENTS_MAX�������¼�������֮�ͣ��������䡢��Ϣ���С��ź�������������
#define TIMER_MAX       		20		//��ʱ��
#define MSG_QUEUE_MAX		16		//��Ϣ����
#define TASKS_MAX			14		//����
#define OS_TICK_RATE_HZ		100

/*
*********************************************************************************************************
*                                      �����ж��������ַ
* ���16k��ַ����bootloader�����ǵĴ�����Ҫ��16k��ʼ��������Ҫ������ʱ���ж���������ӳ�䵽16k��ַ
* Ŀǰֻ��F205��Ч��F103�汾��ʹ��bootloader
*********************************************************************************************************
*/
//#define IVECTOR_ADDR 					(16*1024)
#define IVECTOR_ADDR 					(0)		//�����ʹ��bootloaer��ͬʱ��IROM1��start��Ϊ0x8000000��

#endif
