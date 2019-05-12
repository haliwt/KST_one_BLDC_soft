#ifndef __BSP_HALLSW_H__
#define __BSP_HALLSW_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#define NO_HALL_U         HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)   //霍尔信号切换，切换成无霍尔



void NO_HALL_Init(void);

#endif











