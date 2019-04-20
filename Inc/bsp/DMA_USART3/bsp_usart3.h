#ifndef __BSP_USART3_H__
#define __BSP_USART3_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define HMI_USARTx                                 USART3//USART1  WTI.EDIT
#define HMI_USARTx_BAUDRATE                        115200
#define HMI_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define HMI_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define HMI_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define HMI_USARTx_Tx_GPIO_PIN                     GPIO_PIN_10//GPIO_PIN_8 //WT.EDIT 
#define HMI_USARTx_Tx_GPIO                         GPIOC//GPIOD
#define HMI_USARTx_Rx_GPIO_PIN                     GPIO_PIN_11//GPIO_PIN_9
#define HMI_USARTx_Rx_GPIO                         GPIOC//GPIOD

#define HMI_USARTx_AFx                             GPIO_AF7_USART3

#define HMI_USART_IRQn                             USART3_IRQn
/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_usart3;

/* 函数声明 ------------------------------------------------------------------*/
void MX_USART3_Init(void);


#endif  
