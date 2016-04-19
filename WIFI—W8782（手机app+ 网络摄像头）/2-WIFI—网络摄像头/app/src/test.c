#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "test.h"
#include "dhcpd.h"
#include "webserver.h"
#include <cctype>


char command = 0;

uint8_t test_ir_data[6] = {0xb2, 0x4d, 0x3f, 0xc0, 0x40, 0xbf};

#define help_string   \
"���Թ���:\r\n"\
"	0:������Ϣ\r\n"\
"	1:ɨ��\r\n"\
"	2:����WIFI\r\n"\
"	3:�ر�WIFI����\r\n"\
"	4:����AP\r\n"\
"	5:����TCP\r\n"\
"	6:�ر�TCP����\r\n"\
"	7:send����\r\n"\
"	8:����TCP������\r\n"\
"	9:�ر�TCP������\r\n"\
"	a:������Ƶ����\r\n"\
"	b:�ر���Ƶ����\r\n"\
"	c:dns����\r\n"\
"	d:�Զ���ȡIP��ַ\r\n"\
"	e:��ӡ�ڴ���Ϣ\r\n"\
"	f:��ȡwifi������Ϣ\r\n"\
"	g:����UDP\r\n"\
"	h:����ಥ��\r\n"\
"	i:����UDP������\r\n"\
"	j:sendto����\r\n"\
"	k:�л���Ƶģʽ(mp3/pcm)\r\n"\
"	l:��������\r\n"\
"	m:DEBUG���\r\n"\
"	n:�ر�AP\r\n"\
"	o:��ȡsta��Ϣ\r\n"\
"	p:����adhoc����\r\n"\
"	q:�˳�adhoc����\r\n"\
"	r:ʹ��ʡ��ģʽ\r\n"\
"	s:�ر�ʡ��ģʽ\r\n"\
"	t:monitor���\r\n"\
"	u:������ͷ\r\n"\
"	v:�ر�����ͷ\r\n"\
"	w:�л�����ͷ�ֱ���\r\n"\
"	x:����\r\n"\
"	y:����machtalkԶ�̿���\r\n"\
"	z:��88W8782�̼�����SPI flash\r\n"\
"	?:����\r\n"\
"���������������ѡ����ԵĹ���:"


void print_help()
{
	p_dbg(help_string);
}

extern int test_create_adhoc(void);
void handle_cmd(char cmd)
{
	cmd = tolower(cmd);
	switch (cmd)
	{
		case '0':
			show_sys_info(p_netif);
			break;
		case '1':
			test_scan();
			break;
		case '2':
			test_wifi_connect();
			break;
		case '3':
			test_wifi_disconnect();
			break;
		case '4':
			test_create_ap();
			break;
		case '5':
			test_tcp_link();
			break;
		case '6':
			test_tcp_unlink();
			break;
		case '7':
			test_send("this is test data");
			break;
		case '8':
			test_tcp_server();
			break;
		case '9':
			test_close_tcp_server();
			break;
		case 'a':
			test_auto_get_ip(p_netif);
			break;
		case 'b':
			test_auto_get_ip(p_eth_netif);
			break;
		case 'c':
			test_dns("www.baidu.com");
			break;
		case 'd':
			show_sys_info(p_eth_netif);
			break;
		case 'e':
#if USE_MEM_DEBUG	
			mem_slide_check(1);
#endif
			break;
		case 'f':
			test_wifi_get_stats();
			break;
		case 'g':
			test_udp_link();
			break;
		case 'h':
			test_multicast_join();
			break;
		case 'i':
			test_udp_server();
			break;
		case 'j':
			test_sendto("this is test data");
			break;
#if SUPPORT_AUDIO
		case 'k':
			//switch_audio_mode();
			break;
		case 'l':
//			if(audio_cfg.volume > 0)
//				audio_cfg.volume -= 10;
//			else
//				audio_cfg.volume = 100;
//			p_dbg("��ǰ����:%d", audio_cfg.volume);
			break;
#endif
		case 'm':
			switch_dbg();
			break;
		case 'n':
			test_stop_ap();
			break;
		case 'o':
			test_get_station_list();
			break;
		case 'p':
			test_wifi_join_adhoc();
			break;
		case 'q':
			test_wifi_leave_adhoc();
			break;
		case 'r':
			test_power_save_enable();
			break;
		case 's':
			test_power_save_disable();
			break;
		case 't':
			monitor_switch();
			break;
		case 'u':
			open_camera();
			break;
		case 'v':
			close_camera();
			break;
		case 'w':
			switch_jpeg_size();
			break;
		case 'x':
			soft_reset();
			//test_mqtt_pub();
			break;
		case 'y':
			//st_connect_to_machtalk_server();
			//test_pub_mqtt_connect();
			test_write_firmware_to_spi_flash();
			break;
		case 'z':
//			RTC_Time_Set(15, 10, 1, 4, 22, 03, 00);
			//test_sub_mqtt_connect();
			
			break;
		case '?':
			print_help();
			break;
		default:
			p_err("unkown cmd");
			break;
	}
}
