/***********************************************************************
�ļ����ƣ�LED_Ctrl.C
��    �ܣ�
��дʱ�䣺
�� д �ˣ�������Ƕ���������Ӽ����Ŷ�
ע    �⣺

***********************************************************************/
#include "drivers.h"
#include "app.h"

#include "lwip\sockets.h"

/***********************************************************************
�������ƣ�void LED_Ctrl(unsigned char *data)
��    �ܣ�����data�������������led������
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
			���	led1_on				LED1����
					led2_on				LED2����
					led3_on				LED3����
					led4_on				LED4����
									
					led1_off			LED1����
					led2_off			LED2����
					led3_off			LED3����
					led4_off			LED4����
***********************************************************************/
extern int a_new_client_sock_fd ;					//TCP�ͻ���socket���
extern int TCP_Send_Data(int s,char *data,int len,int flags);

void CMD_Ctrl(char *data)
{
     
 char cStr [ 100 ] = { 0 }, cCh;
 	static uint8_t ucLed1Status = 0, ucLed2Status = 0, ucLed3Status = 0, ucBuzzerStatus = 0;
	static DHT11_Data_TypeDef dht11_data;	


  char * pCh, * pCh1;
 	uint8_t ucLen;

  if ( ( pCh = strstr ( (const char*)data, "CMD_LED_" ) ) != 0 ) //LED������ CMD_LED_2_1 ����LED2��
  {
    cCh = * ( pCh + 8 );
    
    switch ( cCh )
    {
      case '1':
        cCh = * ( pCh + 10 );
        switch ( cCh )
        {
          case '0':
            IND1_OFF;
            ucLed1Status = 0;
            break;
          case '1':
            IND1_ON ;
            ucLed1Status = 1;
            break;
          default :
            break;
        }
        break;
        
      case '2':
        cCh = * ( pCh + 10 );
        switch ( cCh )
        {
          case '0':
            IND2_OFF ;
            ucLed2Status = 0;
            break;
          case '1':
            IND2_ON ;
            ucLed2Status = 1;
            break;
          default :
            break;
        }
        break;

      case '3':
        cCh = * ( pCh + 10 );
        switch ( cCh )
        {
          case '0':
            IND3_OFF;
            ucLed3Status = 0;
            break;
          case '1':
            IND3_ON;
            ucLed3Status = 1;
            break;
          default :
            break;
        }
        break;
        
      default :
        break;					
        
    }
		
		//ִ���������Ҫ����״̬
		sprintf ( cStr, "CMD_LED_%d_%d_%d_ENDLED_END", ucLed1Status, ucLed2Status, ucLed3Status );

		
	 }
	
	else if ( ( pCh = strstr (  (const char*)data, "CMD_BUZZER_" ) ) != 0 ) //���Ʒ��������� CMD_BUZZER_1 ��������
	{
		cCh = * ( pCh + 11 );
		
		switch ( cCh )
		{
			case '0':
				BEEP_OFF ();
				ucBuzzerStatus = 0;
				break;
			case '1':
				BEEP_ON ();
				ucBuzzerStatus = 1;
				break;
			default:
				break;
		}
		
		//ִ���������Ҫ����״̬
		sprintf ( cStr, "CMD_BUZZER_%d_ENDBUZZER_END", ucBuzzerStatus );
		
	}
	else if ( ( ( pCh  = strstr ( (const char*)data, "CMD_UART_" ) ) != 0 ) && 
					( ( pCh1 = strstr ( (const char*)data, "_ENDUART_END" ) )  != 0 ) ) 
	{
		if ( pCh < pCh1)
		{
			ucLen = pCh1 - pCh + 12;
			memcpy ( cStr, pCh, ucLen );
			cStr [ ucLen ] = '\0';
		}
	}
	else if ( strstr ( (const char*)data,  "CMD_READ_ALL_END" ) != 0 ) //APP�������д�����״̬
	{
		Read_DHT11 ( & dht11_data );
		sprintf ( cStr, "CMD_LED_%d_%d_%d_ENDLED_DHT11_%d.%d_%d.%d_ENDDHT11_BUZZER_%d_ENDBUZZER_END", 
						ucLed1Status, ucLed2Status, ucLed3Status, dht11_data .temp_int, 
						dht11_data .temp_deci, dht11_data .humi_int, dht11_data .humi_deci,
						ucBuzzerStatus );
	}
	
	
	TCP_Send_Data(a_new_client_sock_fd,cStr,strlen(cStr),MSG_DONTWAIT);//������ԭ������



}


