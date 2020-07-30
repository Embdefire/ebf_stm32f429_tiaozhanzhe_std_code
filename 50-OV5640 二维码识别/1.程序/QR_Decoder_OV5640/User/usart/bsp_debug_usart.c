/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

unsigned int  uart_data_len = 0;     //���ڴ��������ݳ���
unsigned int  uart_data_index = 0;   //�����ѷ������ݸ���
unsigned char uart_send_state= 0; //����״̬��1��ʾ���ڷ��ͣ�0��ʾ����
unsigned char uart_tx_buf[UART_MAX_BUF_SIZE] = {0};//���ڷ������ݻ�����

 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
		
  RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�DEBUG_USART ģʽ */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
  USART_Cmd(DEBUG_USART, ENABLE);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
  //����USART1�ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

 /**
  * @brief  ��ȡ���ڷ���״̬
  * @param  ��
  * @retval 1��ʾ���ڷ��ͣ�0��ʾ����
  */
uint8_t get_send_sta()
{
	if(uart_send_state)
		return 1;
	return 0;
}
 /**
  * @brief  ������д��USART1���ͻ�����
  * @param  dat����ָ�룬len���ݳ���
  * @retval 0��ʾд��ɹ���1��ʾд��ʧ��
  */
uint8_t uart_send_buf(unsigned char *dat, unsigned int len)
{
	unsigned char addr = 0;
	
	if(uart_send_state)
		return 1;
	
	uart_data_len = len;
	uart_data_index = 0;
	uart_send_state = 1;
	
	for(; len > 0; len--)
		uart_tx_buf[addr++] = *(dat++);
	
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	return 0;
}
 /**
  * @brief  USART1�����ж���Ӧ����
  * @param  
  * @retval 
  */
void USART1_IRQ(void)
{
	//�����ж�
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
        if(uart_data_index < uart_data_len)
		{
			USART_SendData(USART1, uart_tx_buf[uart_data_index++]);
		}
		else
		{
			uart_send_state = 0;
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
				
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
    }
}

//�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
