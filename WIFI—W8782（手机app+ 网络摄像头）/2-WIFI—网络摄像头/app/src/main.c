/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   WIFI ����ͷ����
  ******************************************************************************
  * @attention
  *��ʵ��ǰ�����޸ı��ļ���ssid��pwd�������ֱ�ΪҪ���ӵ�·���������룩
	*	tcp���Ӷ˿ں�Ϊ��4800
	*
  * ʵ��ƽ̨:����  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */



#define DEBUG

#include "drivers.h"  
#include "app.h"
#include "api.h"

#include "test.h"
#include "dhcpd.h"
#include "webserver.h"
#include <cctype>

/*
* �汾˵����docĿ¼
*/
#define VIRSION		"V1.8"



//ʵ��ǰ�����޸ı��ļ���ssid��pwd�������ֱ�ΪҪ���ӵ�·����������
//tcp �˿ں�Ĭ��Ϊ1030
char ssid[]="A305";
char pwd[]="wildfire";


void main_thread(void *pdata)
{
	int ret;
  OS_ERR   err;

	
	#ifdef DEBUG
	RCC_ClocksTypeDef RCC_ClocksStatus;
	#endif
	
	driver_misc_init(); //��ʼ��һЩ����(�������)
	usr_gpio_init(); //��ʼ��GPIO

	OSStatTaskCPUUsageInit((OS_ERR*)&ret);

	Debug_USART_Config(); //��ʼ������1
	
	//��ӡMCU����ʱ��
#ifdef DEBUG
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	p_dbg("SYSCLK_Frequency:%d,HCLK_Frequency:%d,PCLK1_Frequency:%d,PCLK2_Frequency:%d,ADCCLK_Frequency:%d\n", 
		RCC_ClocksStatus.SYSCLK_Frequency, 
		RCC_ClocksStatus.HCLK_Frequency, 
		RCC_ClocksStatus.PCLK1_Frequency, 
		RCC_ClocksStatus.PCLK2_Frequency, 
		0);
#endif

	w25q128_init(); //��ʼ��SPI-flash

	init_work_thread();	//��ʼ�������߳�	
	load_web_cfg(&web_cfg);
		
	init_lwip(); //��ʼ��lwipЭ��ջ
	
	ret = init_wifi();//��ʼ��WIFIоƬ
	if(ret == 0)
	{
		init_monitor(); //��ʼ��monitorģ��,������init_wifi֮�����
		lwip_netif_init();
		init_udhcpd(); //��ʼ��dhcp������
		enable_dhcp_server(); // ����dhcp������,���������staģʽ,���Բ�����dhcpserver

	}else
		p_err("init wifi faild!"); 


	misc_init(); //��ʼ��һЩ����(app���)

	init_key_dev();
	
	open_camera();

	p_dbg("code version:%s", VIRSION);
	p_dbg("startup time:%d.%d S", os_time_get() / 1000, os_time_get() % 1000);
	
#if	SUPPORT_WEBSERVER
	web_server_init();
#endif
//	test_power_save_enable();  //����뽵��wifiģ�鹦�ģ���ִ������
	
	//����������������tcp�˿����ӡ�
	test_tcp_server();
	
	print_help();
	
	
	Wifi_Connect(ssid,pwd);//����AP��·�ɣ�������Ϊ�ʺš�����

//	work_process();

	//ר�����ڴ�������¼�������
  thread_create(Task_Event_Process, 0, TASK_MAIN_PRIO, 0, 512, "Task_Event_Process");

	
	while (1)//�ڴ˿���ָ��һ��CPU����ָʾ����˸������ϵͳ����״̬
	{

    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_DLY,
                  &err); 
	}
}

int main(void)
{

	OS_ERR err;
	OSInit(&err);

	_mem_init(); //��ʼ���ڴ����
	msg_q_init();//
	thread_create(main_thread, 0, TASK_MAIN_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");

	OSStart(&err);


	return 0;
}
