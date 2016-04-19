#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "lwip\sockets.h"
#include "lwip\netif.h"
#include "lwip\dns.h"
#include "lwip\api.h"
#include "lwip\tcp.h"

#include "cfg80211.h"
#include "defs.h"
#include "type.h"
#include "types.h"

#include "tcpapp.h"
#include "wifi.h"
#include "debug.h"
#include "lwip/dhcp.h"
#include "dhcpd.h"
#include "sys_misc.h"

#include "webserver.h"

//ȫ�ֱ�������
extern int errno; //lwip�����

/*
 * socket����������socket�����в�������Ҫ����
 * �����������lwip��ܲ��ȶ�����ϸԭ��û���о�
 * ��Ҫע�������send�������ܻ������ĺ�����������������ή������
 * �շ���Ч�ʣ������շ�����ʱ���÷�������ʽҪ��Щ(MSG_DONTWAIT)
 */
extern mutex_t socket_mutex;

//��������
void test_tcp_recv(void *_fd);

//socket �ļ����
int client_socket_fd =  - 1;
int server_socket_fd =  - 1;
int remote_socket_fd =  - 1;

struct sockaddr udp_remote_client __attribute__((aligned(4))); //���ڱ���Զ��udp�ͻ�����Ϣ

//thread ���
int server_accept_thread_fd =  - 1;


/*
 * @brief  ���ӵ�IP��ַΪ192.168.0.100���˿ں�4700��TCP������
 *
 */
void test_tcp_link()
{
	char *ip = "192.168.1.100";
	uint16_t port = 4700;
	p_dbg_enter;
	p_dbg("���ӵ�:%s, �˿ں�:%d", ip, port);
	if (!is_wifi_connected())
	{

		p_err("wifi not connect");
		return ;
	}

	client_socket_fd = tcp_link(ip, port);
	add_select_array(client_socket_fd);
	p_dbg_exit;
}

/*
 * @brief  ���ӵ�IP��ַΪ192.168.0.100���˿ں�4701��UDP������
 *
 */
void test_udp_link()
{
	char *ip = "192.168.1.100";
	uint16_t port = 4701;
	p_dbg_enter;
	p_dbg("���ӵ�:%s, �˿ں�:%d", ip, port);
	if (!is_wifi_connected())
	{

		p_err("wifi not connect");
		return ;
	}

	client_socket_fd = udp_link(ip, port);
	add_select_array(client_socket_fd);
	p_dbg_exit;
}

/*
 * @brief  ��ӵ��ಥ��
 * �鲥��ַΪ224.0.0.2���˿ں�4702
 */
void test_multicast_send_data(void);
void test_multicast_join()
{
	char *ip = "239.118.0.0";
	uint16_t port = 10000;
	p_dbg_enter;
	p_dbg("���ӵ�:%s, �˿ں�:%d", ip, port);
	if (!is_wifi_connected())
	{

		p_err("wifi not connect");
		return ;
	}
	wifi_set_multicast(IFF_ALLMULTI);
	client_socket_fd = udp_add_membership(ip, port);

	if(client_socket_fd != -1)
		test_multicast_send_data();
	add_select_array(client_socket_fd);
	p_dbg_exit;
}

/*
 * @brief ��ಥ�鷢������
 * �鲥��ַΪ224.0.0.2���˿ں�4702
 */
void test_multicast_send_data()
{
	char *ip = "224.0.0.2";
	uint16_t port = 4702;
	struct sockaddr_in multisnd_addr;
		
	p_dbg_enter;
	p_dbg("�������ݵ�:%s, �˿ں�:%d", ip, port);
	if (!is_wifi_connected())
	{
		p_err("wifi not connect");
		return ;
	}
	if(client_socket_fd == -1)
	{
		p_err("udp socket not create");
		return ;
	}
	memset(&multisnd_addr, 0, sizeof(struct sockaddr_in));
	multisnd_addr.sin_len = sizeof(struct sockaddr);
	multisnd_addr.sin_family = AF_INET;
	multisnd_addr.sin_port=htons(port);
	multisnd_addr.sin_addr.s_addr=inet_addr(ip);
	
	sendto(client_socket_fd,"this is test data", 20, 0,(struct sockaddr*)&multisnd_addr,sizeof(struct sockaddr));
	
	p_dbg_exit;
}


/*
 * @brief  ����udp������
 * �˿ں�4703
 */
void test_udp_server()
{
	uint16_t port = 4703;
	p_dbg_enter;
	p_dbg("test_udp_server, �˿ں�:%d", port);
	if (!is_wifi_connected())
	{

		p_err("wifi not connect");
		return ;
	}

	server_socket_fd = udp_create_server(port);
	add_select_array(server_socket_fd);
	p_dbg_exit;
}


/*
 * @brief  �Ͽ�TCP����
 *
 */
void test_tcp_unlink()
{
	p_dbg_enter;
	if (client_socket_fd !=  - 1)
	{
		close_socket(client_socket_fd);
		client_socket_fd =  - 1;
	}
	p_dbg_exit;
}


DECLARE_MONITOR_ITEM("tcp totol send", tcp_totol_send);

/*
 * @brief  �������ݵ�Զ�̷�����
 *
 */
void test_send(char *pstr)
{
	int ret, len = strlen(pstr);
	p_dbg_enter;
	if (remote_socket_fd !=  - 1)
	{
		mutex_lock(socket_mutex);
		ret = send(remote_socket_fd, pstr, len, 0);
		mutex_unlock(socket_mutex);
		if (ret != len)
		{
			p_err("send data err:%d", ret);
			close_socket(remote_socket_fd);
			remote_socket_fd =  - 1;
		}
	}
	if (client_socket_fd !=  - 1)
	{
		mutex_lock(socket_mutex);
		ret = send(client_socket_fd, pstr, len, 0);
		mutex_unlock(socket_mutex);
		if (ret != len)
		{
			p_err("send data er1r:%d", ret);
			close_socket(client_socket_fd);
			client_socket_fd =  - 1;
		}
	}
	ADD_MONITOR_VALUE(tcp_totol_send, len);
	p_dbg_exit;
}

/*
 * @brief TCP���ݷ����߳�
 * tcp_send_stop�����û����ƽ�������
 *
 */
int tcp_send_stop = 0;
#define TEST_PACKET_SIZE 	1024
void tcp_send_thread(void *arg)
{
	int i;
	char *send_buff;

	send_buff = (char*)malloc(TEST_PACKET_SIZE);
	if(!send_buff)
		goto end;

	if(client_socket_fd == -1){
		p_err("���������������TCP����");
		goto end;
	}

	for(i = 0; i < TEST_PACKET_SIZE; i++)
		send_buff[i] = i;
	p_dbg("tcp send start");
	while(1)
	{

		mutex_lock(socket_mutex);
		//����������ʽ����,���ַ�ʽ���ͻ������൱��ʱ��
		i = send(client_socket_fd, send_buff, TEST_PACKET_SIZE, /*MSG_DONTWAIT*/0);
		mutex_unlock(socket_mutex);

		ADD_MONITOR_VALUE(tcp_totol_send, TEST_PACKET_SIZE);
		if(tcp_send_stop)
		{
			p_dbg("stop tcp send test");
			goto end;
		}
		//sleep(10); //�������������ٶȣ�������������ʱһ��
	}

end:
	p_dbg("tcp send end");
	if(send_buff)
		mem_free(send_buff);
	thread_exit(thread_myself());
}
/*
 * @brief �����ɷ��ͣ����Ƚ���TCP����
 * �����������߳�
 *
 */
void test_full_speed_send()
{
	p_dbg_enter;
	tcp_send_stop = 0;
	thread_create(tcp_send_thread, 0, TASK_TCP_SEND_PRIO, 0, TASK_TCP_SEND_STACK_SIZE, "tcp_send_thread");
	p_dbg_exit;
}

void test_full_speed_send_stop()
{
	p_dbg_enter;
	tcp_send_stop = 1;
	p_dbg_exit;
}


/*
 *������Է���udp���ݵ�Զ�̶�
 *�����½�һ��socket���������ݵ�192.168.1.101:4703
 */
void test_sendto1(char *pstr)
{
	int fd;
	struct sockaddr_in addr;
	int len = strlen(pstr);
	
	p_dbg_enter;

	p_dbg("enter %s\n", __FUNCTION__);
	addr.sin_family = AF_INET;
	addr.sin_len = sizeof(struct sockaddr_in);
	addr.sin_port = htons(4703);
	addr.sin_addr.s_addr = inet_addr("192.168.1.100");

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		p_err("get socket err:%d", errno);
		return;
	} 

	mutex_lock(socket_mutex);
	len = sendto(fd, (u8*)pstr, len, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	mutex_unlock(socket_mutex);
	p_dbg("ret:%d", len);
	close(fd);
	
	p_dbg_exit;
}

/*
 *������Է���udp���ݵ�Զ�̶�
 *fd ʹ��server_socket_fd����test_udp_server����������socket
 */
void test_sendto(char *pstr)
{
#if 1
	struct lwip_sock *sock;
	struct sockaddr_in addr;
	uint32_t remote_addr;
	uint16_t remote_port;
	int len = strlen(pstr);
	p_dbg_enter;
	if (server_socket_fd ==  - 1)
		return ;

	memcpy(&addr, &udp_remote_client, sizeof(struct sockaddr_in));
	remote_addr = addr.sin_addr.s_addr;
	remote_port = addr.sin_port;

	sock = get_socket(server_socket_fd);
	if (!sock || !sock->conn)
	{
		return ;
	}

	if (sock->conn->type != NETCONN_UDP)
	{
		p_err("is not udp socket");
		return ;
	}
	p_dbg("�������ݵ�:%x, �˿ں�:%d", remote_addr, remote_port); //������ʾ�Ĵ��ģʽ����ֵ

	udp_data_send(server_socket_fd, pstr, len, remote_port, remote_addr);
	p_dbg_exit;
#endif
	test_sendto1(pstr);	
}


/*
 * @brief  �رձ��ط�����
 *
 */
void test_close_tcp_server()
{
	p_dbg_enter;
	if (remote_socket_fd !=  - 1)
	{
		close_socket(remote_socket_fd);
		remote_socket_fd =  - 1;
	}

	if (server_socket_fd !=  - 1)
	{
		close_socket(server_socket_fd);
		server_socket_fd =  - 1;
	}

	if (server_accept_thread_fd !=  - 1)
	{
		thread_exit(server_accept_thread_fd);
		server_accept_thread_fd =  - 1;
	}

	p_dbg_exit;
}

/*
 * @brief ���ط����������߳�
 *
 */
void tcp_accept_task(void *server_fd)
{
	int sockaddr_len, new_socket, opt;
	struct sockaddr_in addr;

	sockaddr_len = sizeof(struct sockaddr);

	while (1)
	{
		p_dbg("waiting for remote connect");
		new_socket = accept((int)server_fd, (struct sockaddr*) &addr, (socklen_t*) &sockaddr_len);
		if (new_socket ==  - 1)
		{
			p_err("accept err");
			break;
		} p_dbg("accept a new client");
		remote_socket_fd = new_socket;
		add_select_array(remote_socket_fd);
		opt = 1;
		if (setsockopt(new_socket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int)) ==  - 1)
			p_err("SO_KEEPALIVE err\n");

		start_capture_img(); //��ʼ����ͷ���񣬷������ؿͻ���

	}
	thread_exit(server_accept_thread_fd);
}


void test_dns(char *hostname)
{
	uint32_t addr;
	int ret;

	p_dbg_enter;
	ret = get_host_by_name(hostname, &addr);

	if (ret == 0)
	{
		p_dbg("get %s, ipaddr:: %d.%d.%d.%d\n", hostname, ip4_addr1(&addr), ip4_addr2(&addr), ip4_addr3(&addr), ip4_addr4(&addr));

	}
	p_dbg_exit;
}

/*
 * @brief  �ڱ��ؽ���һ�����������ȴ�����
 * �������˿ں�4800
 *
 */
void test_tcp_server()
{

	int socket_s =  - 1, err = 0;
	uint16_t port = 4800;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(struct sockaddr_in));
	p_dbg_enter;

	p_dbg("����������, �˿ں�:%d", port);
	test_close_tcp_server();

	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htons(INADDR_ANY);

	socket_s = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_s ==  - 1)
	{
		goto err;
	}
	err = bind(socket_s, (struct sockaddr*) &serv, sizeof(struct sockaddr_in));
	if (err ==  - 1)
	{
		goto err;
	} err = listen(socket_s, 4);
	if (err ==  - 1)
	{
		goto err;
	}

	server_socket_fd = socket_s;

	server_accept_thread_fd = thread_create(tcp_accept_task, (void*)server_socket_fd, TASK_TCP_ACCEPT_PRIO, 0, TASK_ACCEPT_STACK_SIZE, "tcp_accept_task");

	return ;
	err: if (err < 0)
		p_err("err:%d", err);
	if (socket_s !=  - 1)
		close_socket(socket_s);
	p_dbg_exit;
}


void show_tcpip_info(struct netif *p_netif)
{
	ip_addr_t dns_server;

	dns_server = dns_getserver(0);

	p_dbg("ipaddr: %d.%d.%d.%d", ip4_addr1(&p_netif->ip_addr.addr), ip4_addr2(&p_netif->ip_addr.addr), ip4_addr3(&p_netif->ip_addr.addr), ip4_addr4(&p_netif->ip_addr.addr));

	p_dbg("netmask: %d.%d.%d.%d", ip4_addr1(&p_netif->netmask.addr), ip4_addr2(&p_netif->netmask.addr), ip4_addr3(&p_netif->netmask.addr), ip4_addr4(&p_netif->netmask.addr));

	p_dbg("gw: %d.%d.%d.%d", ip4_addr1(&p_netif->gw.addr), ip4_addr2(&p_netif->gw.addr), ip4_addr3(&p_netif->gw.addr), ip4_addr4(&p_netif->gw.addr));


	p_dbg("dns_server: %d.%d.%d.%d", ip4_addr1(&dns_server.addr), ip4_addr2(&dns_server.addr), ip4_addr3(&dns_server.addr), ip4_addr4(&dns_server.addr));
}


/*
 * @brief  �Զ���ȡIP����
 *
 *
 */
int auto_get_ip(struct netif *p_netif);
void test_auto_get_ip(struct netif *p_netif)
{
	int i, wait_time = 10;

	auto_get_ip(p_netif);

	for (i = 0; i < wait_time; i++)
	{
		p_dbg("%d", i);
		if (p_netif->ip_addr.addr)
			break;
		sleep(1000);
	}

	if (p_netif->ip_addr.addr)
	{
		show_tcpip_info(p_netif);
	}

}

char loopback_enable = 0;
void switch_loopback_test()
{
	loopback_enable = !loopback_enable;
	if(loopback_enable)
		p_dbg("ʹ�ܻط�����");
	else
		p_dbg("�رջط�����");
}

/**
 * @brief  ������յ���socketͨ�����ݣ�Ŀǰֻ��Ϊ��������ʹ��
 * @���磬tcp�ͻ������ӵ��˿����壬�ͻ��˷����������ݽ������ﴦ��
 * @
 */
void handle_test_recv(int socket, uint8_t *data, int len)
{
	if(loopback_enable)
	{
		send(socket, data, len, 0);
	}else{
		p_dbg("socket:%d, recv:%d byte", socket, len);
		dump_hex("data", data, len);
		send_cmd_to_self(data[0]);
	}
}

