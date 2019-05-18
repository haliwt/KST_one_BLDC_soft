#ifndef __BSP_HALLSW_H__
#define __BSP_HALLSW_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"



//#define HALL_FAIL        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)   //�������ϼ��

#define HALL_FAIL_RCC_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define HALL_FAIL_GPIO_PIN                 GPIO_PIN_10 ////�������ϼ��
#define HALL_FAIL_GPIO                     GPIOD //WT.EDIT


#define HALL_FAIL_OFF                     HAL_GPIO_WritePin(HALL_FAIL_GPIO,HALL_FAIL_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define HALL_FAIL_ON                      HAL_GPIO_WritePin(HALL_FAIL_GPIO,HALL_FAIL_GPIO_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
               // �����ת




void HALL_JUDGE_Init(void);


#endif











