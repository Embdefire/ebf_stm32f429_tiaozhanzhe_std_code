/***********************************************************************
�ļ����ƣ�TCP_Server.C
��    �ܣ�
��дʱ�䣺
�� д �ˣ�������Ƕ���������Ӽ����Ŷ�
ע    �⣺
***********************************************************************/
#include "drivers.h"
#include "app.h"
#include "api.h"
#include "sha1.h"
#include "cfg80211.h"
#include "defs.h"
#include "type.h"
#include "types.h"


#include "lwip\sockets.h"
#include "lwip\netif.h"
#include "lwip\dns.h"
#include "lwip\api.h"
#include "lwip\tcp.h"
#include "lwip\dhcp.h"
#include "lwip\ip_addr.h"

#include "dhcpd.h"
#include "sys_misc.h"

#include "webserver.h"

#include "usr_cfg.h"
#include "test.h"

/***************������ip����*************************/

char *BOARD_IP  	= "192.168.1.200";   		//������ip 
char *BOARD_NETMASK = "255.255.255.0";   		//��������������
char *BOARD_WG		= "192.168.1.1";   		    //������������

#define TCP_LOCAL_PORT			1030			//��TCP�������˿ں�

int tcp_server_sock_fd = -1;					//TCP������socket���
int a_new_client_sock_fd = -1;					//TCP�ͻ���socket���

char tcp_recv_flag = 0;
OS_SEM  sem_tcp_rec_flag ;				//tcp������һ�������ź�������

char tcp_recv_buff[2048];						//tcp���ݽ��ջ�����
int tcp_recv_len = 0;							//tcp���ݽ��ճ���




/***********************************************************************
�������ƣ�void Task_Temperature_Send_Stats(void *pdata)
��    �ܣ����client_sock��״̬�������Ѿ����ӵĿͻ��˷�����ʪ��,ÿ��1��
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void Task_Temperature_Send_Stats(void *pdata)
{
	DHT11_Data_TypeDef data;	
	OS_ERR  os_err;
	uint8_t ret;
	
	char cStr[100];
	
	while(1)
	{
	//OSSemPend(&sem_tcp_rec_flag,0,OS_OPT_PEND_BLOCKING,0,&os_err);				//�����ȴ�sem_tcp_rec_flag�ź�����Ч
    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_DLY,
                  &os_err); 

		ret = Read_DHT11(&data);
		if(ret ==1 )
		{

			sprintf(cStr,"temp = %d.%d,hum=%d.%d",data.temp_int,data.temp_deci,data.humi_int,data.humi_deci);

			TCP_Send_Data(a_new_client_sock_fd,cStr,strlen(cStr),MSG_DONTWAIT);//������ԭ������
		

		}
		else
		{

		}
	}
	
//	struct lwip_sock *sock;
//	float f_adc_value = 0;
//	DHT11_T tDHT;
//	uint8_t ret;
//	unsigned char str[] = "DHT11�¶ȣ�35��C;ʪ�ȣ�30%;CPU�¶ȣ�50.12��C\r\n<br>";//����<br>��Ϊ�˿�������ҳ�ϻ�����ʾ
//	int i = 0;
//	while(1)
//	{
//		f_adc_value = (ADC_RCVTab[0] * 3.3 / 4095 - 0.76) / 0.0025 + 25;//CPU�¶�ת��
//		Float_To_Str(f_adc_value,&str[36]);			//��һ��float������ת���ַ���	
//			
//		ret = DHT11_ReadData(&tDHT);
//		if (ret == 1)
//		{
//			str[11] = tDHT.Temp / 10 + '0';
//			str[12] = tDHT.Temp % 10 + '0';
//			str[23] = tDHT.Hum / 10 + '0';
//			str[24] = tDHT.Hum % 10 + '0';
//		}			
//		else
//		{
//			printf("\r\nδ����DHT11��ʪ�ȴ�����\r\n");
//		}
//		for(i = 0;i < MAX_TEMPER_CLIENT_NUM;i ++)//������ӵĿͻ�����MAX_TEMPER_CLIENT_NUM
//		{		
//			sock = get_socket(client_sock[i]);
//			if(sock->conn->pcb.tcp->state == ESTABLISHED)//������
//			{	
//				TCP_Send_Data(client_sock[i],str,strlen(str),MSG_DONTWAIT);//��ÿͻ��˷����¶�
//			}
//			else						//�ͻ��˶Ͽ�������
//			{
//				client_sock[i] = -1;	//������������Ϊ��Ч
//			}
//		}
//		OSTimeDlyHMSM(0, 0, 1, 0);		//����1s
//	}
}

/***********************************************************************
�������ƣ�void ZQWL_W8782_IRQ_Handler(int socket, uint8_t *buff, int size)
��    �ܣ��������ݴ�������������������ʱ���ú������Զ����ã�������������ݾ������ﴦ��
���������sΪsocket�ţ�buff����ָ�룬size�����ݳ��� remote_addr Զ�̵�ַ��Ϣ
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void ZQWL_W8782_IRQ_Handler(int s, uint8_t *buff, int size)
{
	struct lwip_sock *sock;
  OS_ERR os_err;
  
	sock = get_socket(s);
	if(sock->conn->pcb.tcp->local_port == TCP_LOCAL_PORT)//�뿪����TCP�˿ں�һ��
	{
		if (sock->conn->type == NETCONN_TCP)//��TCPЭ��
		{
			memcpy(tcp_recv_buff,buff,size);//��������
			tcp_recv_flag = 1;
			tcp_recv_len = size;
			a_new_client_sock_fd = s;
			OSSemPost(&sem_tcp_rec_flag,OS_OPT_POST_ALL,&os_err);  //�׳�һ���ź�����ʾtcp�Ѿ��������һ֡����
		}
	}  

}
/***********************************************************************
�������ƣ�void Task_TCP_server(void *pdata)
��    �ܣ�TCP�������������ڴ˿���ʵ�־��幦�ܣ����������ǽ��յ�������ԭ�����ء�
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
extern void CMD_Ctrl( char *data);
void Task_TCP_Server(void *pdata)
{
	OS_ERR  os_err;
	
	tcp_server_sock_fd = test_tcp_server(TCP_LOCAL_PORT);//��ʼ��һ��TCP����socket �˿�ΪTCP_LOCAL_PORT����ʼ����

	OSSemCreate(&sem_tcp_rec_flag,"tcp_rec_flag",0,&os_err); 						//����һ���ź���,
  
  while(1)
	{
		OSSemPend(&sem_tcp_rec_flag,0,OS_OPT_PEND_BLOCKING,0,&os_err);				//�����ȴ�sem_tcp_rec_flag�ź�����Ч

		CMD_Ctrl(tcp_recv_buff);//����һ���򵥵��ֻ�����LED��ʵ��
    
	}
}
