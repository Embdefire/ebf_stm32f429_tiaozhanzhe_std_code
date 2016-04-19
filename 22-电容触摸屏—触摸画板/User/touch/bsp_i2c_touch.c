/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ݴ�������ר��iic����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt9xx.h"
#include "./usart/bsp_debug_usart.h"
/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2C_OWN_ADDRESS7      0x0A


__IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief  ʹ�ܴ������ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQEnable(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;  
  /*���� INT Ϊ�������� */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);  
  
  /* ���� EXTI �ж�Դ ��INT ���� */
  SYSCFG_EXTILineConfig(GTP_INT_EXTI_PORTSOURCE, GTP_INT_EXTI_PINSOURCE);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = GTP_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  
  /* �����ж����ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
	/*ʹ���ж�*/
  NVIC_InitStructure.NVIC_IRQChannel = GTP_INT_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  �رմ������ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQDisable(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /*���� INT Ϊ�������� */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

  /* ���� EXTI �ж�Դ ��INT ���� */
  SYSCFG_EXTILineConfig(GTP_INT_EXTI_PORTSOURCE, GTP_INT_EXTI_PINSOURCE);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = GTP_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* �����ж����ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* �ر��ж� */
  NVIC_InitStructure.NVIC_IRQChannel = GTP_INT_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  ������ I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
   
  /*ʹ��I2Cʱ�� */
  GTP_I2C_CLK_INIT(GTP_I2C_CLK, ENABLE);
  
  /*ʹ�ܴ�����ʹ�õ����ŵ�ʱ��*/
  RCC_AHB1PeriphClockCmd(GTP_I2C_SCL_GPIO_CLK|
                       	 GTP_I2C_SDA_GPIO_CLK|
	                       GTP_RST_GPIO_CLK|GTP_INT_GPIO_CLK, 
	                       ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
  /* ����I2C_SCLԴ*/
  GPIO_PinAFConfig(GTP_I2C_SCL_GPIO_PORT, 
	                 GTP_I2C_SCL_SOURCE, GTP_I2C_SCL_AF);
  /* ����I2C_SDA Դ*/
  GPIO_PinAFConfig(GTP_I2C_SDA_GPIO_PORT, 
	                 GTP_I2C_SDA_SOURCE, GTP_I2C_SDA_AF);  
  
  /*����SCL���� */   
  GPIO_InitStructure.GPIO_Pin = GTP_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GTP_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*����SDA���� */
  GPIO_InitStructure.GPIO_Pin = GTP_I2C_SDA_PIN;
  GPIO_Init(GTP_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
 
 
  /*����RST���ţ������������ */   
  GPIO_InitStructure.GPIO_Pin = GTP_RST_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GTP_RST_GPIO_PORT, &GPIO_InitStructure);
  
  /*���� INT���ţ�������������������ʼ�� */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//����Ϊ�����������ʼ��
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @brief  ��GT91xxоƬ���и�λ
  * @param  ��
  * @retval ��
  */
void I2C_ResetChip(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

  /*���� INT���ţ�������������������ʼ�� */   
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;       //����Ϊ�����������ʼ��
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

	  /*��ʼ��GT9157,rstΪ�ߵ�ƽ��intΪ�͵�ƽ����gt9157���豸��ַ������Ϊ0xBA*/

	  /*��λΪ�͵�ƽ��Ϊ��ʼ����׼��*/
	  GPIO_ResetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*����һ��ʱ�䣬���г�ʼ��*/
	  GPIO_SetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*��INT��������Ϊ��������ģʽ���Ա���մ����ж��ź�*/
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ģʽ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		                    
  I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                          /* ͨ������ */
  I2C_Init(GTP_I2C, &I2C_InitStructure);	                                  /* I2C1 ��ʼ�� */
  I2C_Cmd(GTP_I2C, ENABLE);  	                                              /* ʹ�� I2C1 */

  I2C_AcknowledgeConfig(GTP_I2C, ENABLE);

}


/**
  * @brief  I2C ����(GT91xx)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_Touch_Init(void)
{
  I2C_GPIO_Config(); 
 
  I2C_Mode_Config();

  I2C_ResetChip();

  I2C_GTP_IRQEnable();
}



/**
  * @brief   ʹ��IIC��ȡ����
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
  *		@arg NumByteToRead:��ȡ�����ݳ���
  * @retval  ��
  */
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{  
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(GTP_I2C, I2C_FLAG_BUSY))   
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
    }

		/* Send STRAT condition  */
		I2C_GenerateSTART(GTP_I2C, ENABLE);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
   }
  /* Send GT91xx address for read */
  I2C_Send7bitAddress(GTP_I2C, ClientAddr, I2C_Direction_Receiver);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
   }
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(GTP_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(GTP_I2C, ENABLE);
    }


		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 	
		{
		  /* Read a byte from the device */
      *pBuffer = I2C_ReceiveData(GTP_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;
		}			

		
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(GTP_I2C, ENABLE);
  
  return 0;
}


/**
  * @brief   ʹ��IICд������
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:������ָ��
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(GTP_I2C, I2C_FLAG_BUSY))  
   {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(GTP_I2C, ENABLE);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  /* Send GT91xx address for write */
  I2C_Send7bitAddress(GTP_I2C, ClientAddr, I2C_Direction_Transmitter);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(GTP_I2C, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(GTP_I2C, ENABLE);
  
  return 0;
}





/**
  * @brief  IIC�ȴ���ʱ���ñ��������������Ϣ
  * @param  None.
  * @retval ����0xff����ʾIIC��ȡ����ʧ��
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  GTP_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0xFF;
}

/*********************************************END OF FILE**********************/
