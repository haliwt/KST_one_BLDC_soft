#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define USARTx                                 USART3 //USART1 // WT.EDIT
#define USARTx_BAUDRATE                        115200
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define USARTx_GPIO_ClK_ENABLE()                __HAL_RCC_GPIOC_CLK_ENABLE()//__HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_Tx_GPIO_PIN                     GPIO_PIN_10//WT.EDIT GPIO_PIN_8
#define USARTx_Tx_GPIO                         GPIOC//GPIOD
#define USARTx_Rx_GPIO_PIN                     GPIO_PIN_11//GPIO_PIN_9  
#define USARTx_Rx_GPIO                          GPIOC// GPIOD

#define USARTx_AFx                             GPIO_AF7_USART3

#define USARTx_IRQHANDLER                      USART3_IRQHandler
#define USARTx_IRQn                            USART3_IRQn

// ����DMA���
#define USARTx_DMAx_STREAMn                    DMA1_Stream3
#define USARTx_DMAx_CHANNELn                   DMA_CHANNEL_4
#define USARTx_RCC_DMAx_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_DMAx_CHANNELn_IRQn              DMA1_Stream3_IRQn
#define USARTx_DMAx_CHANNELn_IRQHANDLER        DMA1_Stream3_IRQHandler

/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;

/* �������� ------------------------------------------------------------------*/
void MX_USARTx_Init(void);


#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
