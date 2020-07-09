
/*
	ʹ�üĴ����ķ�������LED��
  */
#include "stm32f4xx.h" 


/**
  *   ������
  */
int main(void)
{	
	/*���� GPIOH ʱ�ӣ�ʹ������ʱ��Ҫ�ȿ�������ʱ��*/
	RCC_AHB1ENR |= (1<<7);	
	
	/* LED �˿ڳ�ʼ�� */
	
	/*GPIOH MODER10���*/
	GPIOH_MODER  &= ~( 0x03<< (2*10));	
	/*PH10 MODER10 = 01b ���ģʽ*/
	GPIOH_MODER |= (1<<2*10);
	
	/*GPIOH OTYPER10���*/
	GPIOH_OTYPER &= ~(1<<1*10);
	/*PH10 OTYPER10 = 0b ����ģʽ*/
	GPIOH_OTYPER |= (0<<1*10);
	
	/*GPIOH OSPEEDR10���*/
	GPIOH_OSPEEDR &= ~(0x03<<2*10);
	/*PH10 OSPEEDR10 = 0b ����2MHz*/
	GPIOH_OSPEEDR |= (0<<2*10);
	
	/*GPIOH PUPDR10���*/
	GPIOH_PUPDR &= ~(0x03<<2*10);
	/*PH10 PUPDR10 = 01b ����ģʽ*/
	GPIOH_PUPDR |= (1<<2*10);
	
	/*PH10 BSRR�Ĵ����� BR10��1��ʹ��������͵�ƽ*/
	GPIOH_BSRR |= (1<<16<<10);
	
	/*PH10 BSRR�Ĵ����� BS10��1��ʹ��������ߵ�ƽ*/
	//GPIOH_BSRR |= (1<<10);

  /*GPIOH MODER11���*/
	GPIOH_MODER  &= ~( 0x03<< (2*11));	
	/*PH11 MODER11 = 01b ���ģʽ*/
	GPIOH_MODER |= (1<<2*11);
	
	/*GPIOH OTYPER11���*/
	GPIOH_OTYPER &= ~(1<<1*11);
	/*PH11 OTYPER11 = 0b ����ģʽ*/
	GPIOH_OTYPER |= (0<<1*11);
	
	/*GPIOH OSPEEDR11���*/
	GPIOH_OSPEEDR &= ~(0x03<<2*11);
	/*PH11 OSPEEDR11 = 0b ����2MHz*/
	GPIOH_OSPEEDR |= (0<<2*11);
	
	/*GPIOH PUPDR11���*/
	GPIOH_PUPDR &= ~(0x03<<2*11);
	/*PH11 PUPDR11 = 01b ����ģʽ*/
	GPIOH_PUPDR |= (1<<2*11);
	
	/*PH11 BSRR�Ĵ����� BR11��1��ʹ��������͵�ƽ*/
	GPIOH_BSRR |= (1<<16<<11);
	
	
  /*GPIOH MODER12���*/
	GPIOH_MODER  &= ~( 0x03<< (2*12));	
	/*PH12 MODER12 = 01b ���ģʽ*/
	GPIOH_MODER |= (1<<2*12);
	
	/*GPIOH OTYPER12���*/
	GPIOH_OTYPER &= ~(1<<1*12);
	/*PH12 OTYPER12 = 0b ����ģʽ*/
	GPIOH_OTYPER |= (0<<1*12);
	
	/*GPIOH OSPEEDR12���*/
	GPIOH_OSPEEDR &= ~(0x03<<2*12);
	/*PH12 OSPEEDR12 = 0b ����2MHz*/
	GPIOH_OSPEEDR |= (0<<2*12);
	
	/*GPIOH PUPDR12���*/
	GPIOH_PUPDR &= ~(0x03<<2*12);
	/*PH12 PUPDR12 = 01b ����ģʽ*/
	GPIOH_PUPDR |= (1<<2*12);
	
	/*PH12 BSRR�Ĵ����� BR12��1��ʹ��������͵�ƽ*/
	GPIOH_BSRR |= (1<<16<<12);
	while(1);

}

// ����Ϊ�գ�Ŀ����Ϊ��ƭ��������������
void SystemInit(void)
{	
}






/*********************************************END OF FILE**********************/

