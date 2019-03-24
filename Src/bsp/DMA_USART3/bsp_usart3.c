
/* 包含头文件 ----------------------------------------------------------------*/
#include "dma_usart3/bsp_usart3.h"

UART_HandleTypeDef husart_usart3;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
#if 0
/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance==HMI_USARTx)
  {
    /* 串口外设时钟使能 */
    DEBUG_USART_RCC_CLK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = DEBUG_USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = HMI_USARTx_AFx;
    HAL_GPIO_Init(DEBUG_USARTx_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = HMI_USARTx_Rx_GPIO_PIN;  
    HAL_GPIO_Init(HMI_USARTx_Tx_GPIO, &GPIO_InitStruct);       
  }  
}

/**
  * 函数功能: 串口硬件反初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==HMI_USARTx)
  {
    /* 串口外设时钟禁用 */
    HMI_USART_RCC_CLK_DISABLE();
  
    /* 串口外设功能GPIO配置 */
    HAL_GPIO_DeInit(HMI_USARTx_Tx_GPIO, HMI_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(HMI_USARTx_Rx_GPIO, HMI_USARTx_Rx_GPIO_PIN);
    
    /* 串口中断禁用 */
    HAL_NVIC_DisableIRQ(HMI_USART_IRQn);
  }
}
#endif 
/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MX_USART3_Init(void)
{
  /* 使能串口功能引脚GPIO时钟 */
  HMI_USARTx_GPIO_ClK_ENABLE();
  
  husart_usart3.Instance = HMI_USARTx;
  husart_usart3.Init.BaudRate = HMI_USARTx_BAUDRATE;
  husart_usart3.Init.WordLength = UART_WORDLENGTH_8B;
  husart_usart3.Init.StopBits = UART_STOPBITS_1;
  husart_usart3.Init.Parity = UART_PARITY_NONE;
  husart_usart3.Init.Mode = UART_MODE_TX_RX;
  husart_usart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart_usart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart_usart3);
  
}

#if 0
/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart_usart3, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart_usart3,&ch, 1, 0xffff);
  return ch;
}
#endif 

