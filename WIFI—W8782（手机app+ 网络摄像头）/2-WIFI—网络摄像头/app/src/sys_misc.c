#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "test.h"
#include "lwip/netif.h"
#include <absacc.h>

timer_t ind1_timer;


void delay_1us()
{
	int i = 50;
	while (i--)
		;
}

void delay_us(uint32_t us)
{
	while (us--)
		delay_1us();
}

uint8_t led_value = 0;
uint8_t led_bright_value = 10;

void led_scan()
{
		if (led_value &(1 << 3))
			IND1_SET;
		else
			IND1_CLR;
		if (led_value &(1 << 4))
			IND2_SET;
		else
			IND2_CLR;
		if (led_value &(1 << 5))
			IND3_SET;
		else
			IND3_CLR;
}

void led_switch(uint8_t value)
{
	p_dbg("led_switch:%x", value);
	led_value = value;
	led_scan();
}

//����֧������
void led_bright(uint8_t value)
{
	p_dbg("led_bright:%d", value);
	if (value > 99)
		value = 99;

	led_bright_value = value / 10;
}


SWITCH_EVENT g_switch = SWITCH_EVENT_OFF;
void button_stat_callback()
{
	static uint8_t last_stat = 1;
	BUTTON_EVENT button_event = BUTTON_EVENT_IDLE;

	if (BUTTON_STAT && !last_stat)
	{
		last_stat = BUTTON_STAT;
		button_event = BUTTON_EVENT_UP;
		//p_dbg("button up");
	}

	if (!BUTTON_STAT && last_stat)
	{
		last_stat = BUTTON_STAT;
		button_event = BUTTON_EVENT_DOWN;
		//p_dbg("button down");
	}

	if (button_event == BUTTON_EVENT_UP)
	{
		if(g_switch == SWITCH_EVENT_OFF)
			g_switch = SWITCH_EVENT_ON;
		else
			g_switch = SWITCH_EVENT_ON;
#if SUPPORT_AUDIO
		//camera_button_event(g_switch);
		audio_button_event(g_switch);
#endif
	}
}


void assert_failed(uint8_t *file, uint32_t line)
{
	p_err("assert_failed in:%s,line:%d \n", file ? file : "n", line);
	while (1)
		;
}

void ind1_timer_callback(void *arg)
{
//	static bool led_stat = FALSE;

//	led_stat = !led_stat;

//	if(led_stat)
//		IND1_ON;
//	else
//		IND1_OFF;
}
void ind2_timer_callback(void *arg)
{
//	static bool led_stat = FALSE;

//	led_stat = !led_stat;

//	if(led_stat)
//		IND2_ON;
//	else
//		IND2_OFF;
}

void indicate_led_twink_start(int fre)
{
//	del_timer(ind1_timer);
//	mod_timer(ind1_timer, fre);
}

void indicate_led_twink_stop()
{
//	del_timer(ind1_timer);
//	IND2_OFF;
}

/**
 * @brief ����������ʱ������led����˸
 */
void misc_init()
{
//	ind1_timer = timer_setup(500, 1, ind1_timer_callback, NULL);
//	add_timer(ind1_timer);
	
	
}

void show_sys_info(struct netif *p_netif)
{
	p_dbg("mac:%02x-%02x-%02x-%02x-%02x-%02x", p_netif->hwaddr[0], p_netif->hwaddr[1], p_netif->hwaddr[2], p_netif->hwaddr[3], p_netif->hwaddr[4], p_netif->hwaddr[5]);
	show_tcpip_info(p_netif);
}

void soft_reset()
{
	p_err("system well reset\n");
	NVIC_SystemReset();
  	__DSB();  
	while (1);
}


#ifdef OS_UCOS
#if OS_APP_HOOKS_EN > 0u
void App_TaskCreateHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}

void App_TaskDelHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}

void App_TaskReturnHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}

void App_TCBInitHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}

void App_TaskSwHook(void){

}

void App_TimeTickHook(void){}

//uC/OS-II Stat�߳��е��ô˺�����ÿ100MSһ��
void App_TaskStatHook()
{
	#if USE_MEM_DEBUG
	mem_slide_check(0);
	#endif
	//button_stat_callback();
}

#endif
#endif

#ifdef OS_FREE_RTOS
void vApplicationIdleHook( void )
{
	#if USE_MEM_DEBUG
	mem_slide_check(0);
	#endif
}
#endif

extern uint32_t drvdbg;
extern uint32_t drvdbg_save;
void switch_dbg(void)
{
	if(dbg_level)
	{
		p_dbg("close dbg");
	}
	dbg_level = !dbg_level;
	if(dbg_level)
	{
		p_dbg("open dbg");
#ifdef DEBUG_LEVEL1
		drvdbg = drvdbg_save;
#endif
	}
#ifdef DEBUG_LEVEL1	
	else
		drvdbg = 0;
#endif
}

void usr_gpio_init()
{
	gpio_cfg((uint32_t)BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	gpio_cfg((uint32_t)IND1_GROUP, IND1_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)IND2_GROUP, IND2_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)IND3_GROUP, IND3_PIN, GPIO_Mode_Out_PP);


	IND1_OFF;
	IND2_OFF;
	IND3_OFF;


	
}


