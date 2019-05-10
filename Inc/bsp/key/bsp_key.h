#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)  //KEY0按键PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)  //KEY1按键PE3
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) 	//KEY2按键PE2
#define WK_UP       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //WKUP按键PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
