#ifndef __BSP_HALLSW_H__
#define __BSP_HALLSW_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#define NO_HALL_U         HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)   //�����ź��л����л����޻���



void NO_HALL_Init(void);

#endif











