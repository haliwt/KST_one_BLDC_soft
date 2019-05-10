#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)  //KEY0����PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)  //KEY1����PE3
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) 	//KEY2����PE2
#define WK_UP       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //WKUP����PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
