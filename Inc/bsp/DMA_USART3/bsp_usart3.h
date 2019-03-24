#ifndef __BSP_USART3_H__
#define __BSP_USART3_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define HMI_USARTx                                 USART3//USART1  WTI.EDIT
#define HMI_USARTx_BAUDRATE                        115200
#define HMI_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define HMI_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define HMI_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define HMI_USARTx_Tx_GPIO_PIN                     GPIO_PIN_8
#define HMI_USARTx_Tx_GPIO                         GPIOD
#define HMI_USARTx_Rx_GPIO_PIN                     GPIO_PIN_9
#define HMI_USARTx_Rx_GPIO                         GPIOD

#define HMI_USARTx_AFx                             GPIO_AF7_USART3

#define HMI_USART_IRQn                             USART3_IRQn
/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_usart3;

/* �������� ------------------------------------------------------------------*/
void MX_USART3_Init(void);


#endif  
