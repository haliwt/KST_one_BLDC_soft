#ifndef __BSP_OUTPUT_H__
#define __BSP_OUTPUT_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define AIR_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define AIR_GPIO_PIN                 GPIO_PIN_2  //空调输出
#define AIR_GPIO                     GPIOA //WT.EDIT


#define WIPER_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define WIPER_GPIO_PIN                 GPIO_PIN_0 //雨刮输出
#define WIPER_GPIO                     GPIOA  //WT.EDIT

#define PLC_RCC_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define PLC_GPIO_PIN                 GPIO_PIN_8 //PLC输出信号
#define PLC_GPIO                     GPIOD  //WT.EDIT




#define AIR_ON                       HAL_GPIO_WritePin(AIR_GPIO,AIR_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define AIR_OFF                      HAL_GPIO_WritePin(AIR_GPIO,LED1_AIR_PIN,GPIO_PIN_RESET)  // 输出低电平
#define AIR_TOGGLE                   HAL_GPIO_TogglePin(AIR_GPIO,AIR_GPIO_PIN)                // 输出反转

#define WIPER_ON                     HAL_GPIO_WritePin(WIPER_GPIO,WIPER_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define WIPER_OFF                    HAL_GPIO_WritePin(WIPER_GPIO,WIPER_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define WIPER_TOGGLE                 HAL_GPIO_TogglePin(WIPER_GPIO,WIPER_GPIO_PIN)                // 输出反转

#define PLC_ON                     HAL_GPIO_WritePin(PLC_GPIO,PLC_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define PLC_OFF                    HAL_GPIO_WritePin(PLC_GPIO,PLC_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define PLC_TOGGLE                 HAL_GPIO_TogglePin(PLC_GPIO,PLC_GPIO_PIN)                // 输出反转






/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void OUTPUT_SIG_GPIO_Init(void);



#endif

