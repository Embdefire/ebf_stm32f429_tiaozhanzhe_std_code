#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32f4xx.h"


#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


#define GTP_I2C                          I2C1
#define GTP_I2C_CLK                      RCC_APB1Periph_I2C1

#define GTP_I2C_SCL_PIN                  GPIO_Pin_6                  
#define GTP_I2C_SCL_GPIO_PORT            GPIOB                       
#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define GTP_I2C_SCL_SOURCE               GPIO_PinSource6
#define GTP_I2C_SCL_AF                   GPIO_AF_I2C1

#define GTP_I2C_SDA_PIN                  GPIO_Pin_7                  
#define GTP_I2C_SDA_GPIO_PORT            GPIOB                     
#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define GTP_I2C_SDA_SOURCE               GPIO_PinSource7
#define GTP_I2C_SDA_AF                   GPIO_AF_I2C1

#define GTP_RST_GPIO_PORT                GPIOD
#define GTP_RST_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_RST_GPIO_PIN                 GPIO_Pin_13

#define GTP_INT_GPIO_PORT                GPIOD
#define GTP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_INT_GPIO_PIN                 GPIO_Pin_12
#define GTP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
#define GTP_INT_EXTI_PINSOURCE           EXTI_PinSource12
#define GTP_INT_EXTI_LINE                EXTI_Line12
#define GTP_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define GTP_IRQHandler                   EXTI15_10_IRQHandler


static uint32_t I2C_TIMEOUT_UserCallback(void);

void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);
void I2C_GTP_IRQDisable(void);
void I2C_GTP_IRQEnable(void);

#endif /* __I2C_TOUCH_H */
