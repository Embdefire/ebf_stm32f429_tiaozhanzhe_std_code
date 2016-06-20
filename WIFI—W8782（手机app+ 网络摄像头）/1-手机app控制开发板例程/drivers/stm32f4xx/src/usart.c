/**
  ******************************************************************************
  * @file    bsp_usart_dma.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����c��printf������usart�˿�,ʹ��DMAģʽ��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
//#define DEBUG
#include "drivers.h"
#include "app.h"
#include "api.h"


UART_RECV 	u1_recv;

UART_SEND 	u1_send;

wait_event_t uart1_rx_waitq = 0;

volatile unsigned int uart1_cnt = 0,uart1_cnt_ex = 0;

USART_TypeDef *dbg_uart = DEBUG_USART;

/**
  * @brief  ���ô���DMA����
  * @param  BufferDST: Ҫ���͵�����ָ��
  * @param  BufferSize: ���ݴ�С
  * @retval None
  */
static void DMA_TxConfiguration(uint32_t *BufferSRC, uint32_t BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/*����ǰ�ر��ж�*/
	USART_ITConfig(DEBUG_USART, USART_IT_TXE, DISABLE);
	USART_ITConfig(DEBUG_USART, USART_IT_TC, DISABLE);  //�رշ�����ɺͷ��Ϳ��ж�
	
	DMA_ITConfig(DEBUG_USART_DMA_TX_STREAM, DMA_IT_TC | DMA_IT_TE | DMA_IT_HT,DISABLE);
	DMA_ClearFlag(DEBUG_USART_DMA_TX_STREAM, DMA_FLAG_TCIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_HTIF7);
	
	/* ����ǰ�ر�DMA*/
	DMA_Cmd(DEBUG_USART_DMA_TX_STREAM, DISABLE);
	DMA_StructInit(&DMA_InitStructure);
	
	/* ����DMA��������ģʽ */
	DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_TX_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DEBUG_USART->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferSRC;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	//DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	//DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	//DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	//DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DEBUG_USART_DMA_TX_STREAM, &DMA_InitStructure);
	
	//ʹ�ܴ�����ɺͳ����ж�	
	DMA_ITConfig(DEBUG_USART_DMA_TX_STREAM, DMA_IT_TC | DMA_IT_TE,ENABLE);  
	
	/* ʹ�ܴ���DMA��������*/
	DMA_Cmd(DEBUG_USART_DMA_TX_STREAM, ENABLE);	
	USART_DMACmd(DEBUG_USART,USART_DMAReq_Tx,ENABLE);
	
	NVIC_EnableIRQ(DEBUG_USART_DMA_TX_IRQn);
}

/**
  * @brief  ���ô���DMA����
  * @param  BufferDST: ������ݻ�������ָ��
  * @param  BufferSize: ��������С
  * @retval None
  */
static void DMA_RxConfiguration(uint32_t *BufferDST, uint32_t BufferSize)
{

	DMA_InitTypeDef DMA_InitStructure;

	/*����ǰ�ر������ж�*/
	DMA_ITConfig(DEBUG_USART_DMA_RX_STREAM, DMA_IT_TC | DMA_IT_TE | DMA_IT_HT,DISABLE);
	DMA_ClearFlag(DEBUG_USART_DMA_RX_STREAM, DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_HTIF5);
   
	/* DMA2 Channel4 disable */
	DMA_Cmd(DEBUG_USART_DMA_RX_STREAM, DISABLE);

	DMA_StructInit(&DMA_InitStructure);
	
	/* DMA2 Channel4 Config */
	DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_RX_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferDST;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DEBUG_USART_DMA_RX_STREAM, &DMA_InitStructure);
	u1_recv.pos = 0;   //��ǰ��ȡλ������
	
	/* DMA2 Channel4 enable */
	DMA_ITConfig(DEBUG_USART_DMA_RX_STREAM, DMA_IT_TE,ENABLE); //ʹ�ܳ����ж�
	DMA_Cmd(DEBUG_USART_DMA_RX_STREAM, ENABLE);
	
	/*ʹ�ܴ���DMA����*/
	USART_DMACmd(DEBUG_USART,USART_DMAReq_Rx,ENABLE);
	NVIC_EnableIRQ(DEBUG_USART_DMA_RX_IRQn);
}

/**
  * @brief  ���ô���115200 8-N-1,DMA���ռ�����
  * @param  none
  * @retval None
  */
void Debug_USART_Config(void)
{	
	
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	memset(&u1_recv,0,sizeof(UART_RECV));
	
	u1_recv.pkg.pending.val = 1;  //��ֹ�û�����

		
  RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK, ENABLE);

	RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK,ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
					
  /* USART mode config */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
  USART_Cmd(DEBUG_USART, ENABLE);
	
	
	u1_send.wait = init_event();
	if(u1_send.wait  == 0)
	{
		p_err("uart_init sys_sem_new1 err\n");
		return;
	}

	
	/*�����ж�*/
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_Priority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*����DMA*/
	DMA_RxConfiguration((uint32_t *)u1_recv.c_buff, UART_RECV_BUFF_SIZE);  //ѭ��������
	
	USART_ClearITPendingBit(DEBUG_USART, USART_IT_TC);
	USART_ClearITPendingBit(DEBUG_USART, USART_IT_RXNE);
	NVIC_EnableIRQ(USART1_IRQn);

	/*DMA�����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_DMA_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_DMA_RX_IRQn_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*DMA�����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_DMA_TX_IRQn_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}




/**
  * @brief  DMA�����ж�
  * @param  none
  * @retval None
  */
void DEBUG_USART_DMA_TX_IRQHandler()  //�����ж�
{	
	uint32_t irq_flag;
	enter_interrupt();

	DMA_Cmd(DEBUG_USART_DMA_TX_STREAM, DISABLE);
	NVIC_DisableIRQ(DEBUG_USART_DMA_TX_IRQn);
	irq_flag = local_irq_save(); 

	if(DMA_GetFlagStatus(DEBUG_USART_DMA_TX_STREAM, DMA_FLAG_TEIF7))
	{
		
		p_err("uart dma err");
	}

	DMA_ITConfig(DEBUG_USART_DMA_TX_STREAM, DMA_IT_TC | DMA_IT_TE | DMA_IT_HT,DISABLE);
	DMA_ClearFlag(DEBUG_USART_DMA_TX_STREAM, DMA_FLAG_TCIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_HTIF7);
   
	u1_send.cnt = DEBUG_USART_DMA_TX_STREAM->NDTR;   //ȡʣ������,Ӧ��Ϊ0
	local_irq_restore(irq_flag);
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, DISABLE);
	p_dbg("u_send irq\n");
	wake_up(u1_send.wait);   //���ѵȴ��߳�
	
	exit_interrupt(1);
}

/**
  * @brief  DMA�����ж�
  * @param  none
  * @retval None
  */
void DEBUG_USART_DMA_RX_IRQHandler()  //�����ж�
{
	DMA_ClearFlag(DEBUG_USART_DMA_RX_STREAM,  DMA_FLAG_TEIF5); //��������־
}

///�ض���c�⺯��printf��USART
int fputc(int ch, FILE *f)
{
	uint32_t timer = 10*1000, irq_flag;
	while(u1_send.sending.val)   //ѭ���ȴ�
	{
		if(timer-- == 0)
			break;
	}

	irq_flag = local_irq_save();
	if(!u1_send.sending.val)
		u1_send.sending.val = TRUE;
	else{
		local_irq_restore(irq_flag);
		return -1;
	}
	while(!(DEBUG_USART->SR & USART_FLAG_TXE));
	DEBUG_USART->DR = ch;	//��������
	u1_send.sending.val = FALSE;
	local_irq_restore(irq_flag);
	return (ch);
}

/**
  * @brief  ���ͻ������е�����
  * @param  buff��Ҫ���͵����� 
	* @param	size�����ݴ�С
  * @retval None
  */
int uart1_send(u8_t *buff,u32_t size)
{
	int ret;
	if((size == 0) || (buff == 0))
		return -1;
	p_dbg("uart1 send start\n");
	clear_wait_event(u1_send.wait);

	u1_send.cnt = size;
	
	/*ʹ��DMA����*/
 	DMA_TxConfiguration((uint32_t*)buff, size);
	
again:
	ret = wait_event_timeout(u1_send.wait, 2000);
	if(ret != 0)
		goto again;
	
	p_dbg("uart1 send end\n");

	if(u1_send.cnt)
		p_err("uart1_send err:%d,%d,%d\n",ret,u1_send.cnt,size);
	else
		p_dbg("uart1 send a  pkg:%d\n", size);
	
	return 0;
}


//import from main.c
extern char command;
/**
  * @brief  �Ӵ��ڽ��յ�������
  * @param  buff�����ڽ��յ������� 
  * @retval None
  */
void uart1_read_data(u8_t *buff, int size)
{
	//TODO
	command = buff[0];
	send_work_event(TEST_EVENT);
}

/**
  * @brief  ������������
  * @param  cmd������
  * @retval None
  */
void send_cmd_to_self(char cmd)
{
  command = cmd;
	send_work_event(TEST_EVENT);
}

//��������1.uart�ж������;
int handle_uart1_rcv()
{
	__IO u32_t cur_pos, i, size, ret, cnt;
	do
	{
		 //ȡ��ǰ����������λ��
		cur_pos = UART_RECV_BUFF_SIZE - DEBUG_USART_DMA_RX_STREAM->NDTR;  
		//���û�н��յ��� cur_pos == u1_recv.pos
		if(cur_pos == u1_recv.pos)
		{
			u1_recv.pkg.pending.val = 0;
			return 0;
		}
		if(cur_pos > u1_recv.pos)
		{
			size = cur_pos - u1_recv.pos;

			uart1_read_data(&u1_recv.c_buff[u1_recv.pos], size);

			u1_recv.pos = cur_pos;
		}
		else
		{
			cnt = 0;
			if(cur_pos < u1_recv.pos)
			{
				size = UART_RECV_BUFF_SIZE - u1_recv.pos;

				uart1_read_data(&u1_recv.c_buff[u1_recv.pos], size);
				
				u1_recv.pos = 0;

				size = cur_pos;

				uart1_read_data(u1_recv.c_buff, size);

				u1_recv.pos = cur_pos;
			}
		}
	}while(0);
	return 0;

}

/**
  * @brief  ���ڽ����ж�
  * @param  None
  * @retval None
  */
void DEBUG_USART_IRQnHandler()
{
	__IO u32_t sr;

	int need_sched = 0;

	enter_interrupt();
	
 	sr = DEBUG_USART->SR;
	if(handle_uart1_rcv())
		need_sched = 1;
	
	exit_interrupt(need_sched);
}


