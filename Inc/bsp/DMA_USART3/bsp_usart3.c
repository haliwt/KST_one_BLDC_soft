/**
  ******************************************************************************
  * �ļ�����: bsp_usartx.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ���ش��ڵײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "DMA_usart3/bsp_usart3.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husartx;
DMA_HandleTypeDef hdma_usartx_tx;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����Ӳ����ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USARTx)
  {
    /* ��������ʱ��ʹ�� */
    USART_RCC_CLK_ENABLE();
    /* ��������DMAʱ��ʹ�� */
    USARTx_RCC_DMAx_CLK_ENABLE();
    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USARTx_AFx;
    HAL_GPIO_Init(USARTx_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USARTx_Rx_GPIO_PIN;
    HAL_GPIO_Init(USARTx_Rx_GPIO, &GPIO_InitStruct);  
    /* DMA����ͨ�������� */
    hdma_usartx_tx.Instance = USARTx_DMAx_STREAMn;
    hdma_usartx_tx.Init.Channel = USARTx_DMAx_CHANNELn;
    hdma_usartx_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usartx_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usartx_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usartx_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usartx_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usartx_tx.Init.Mode = DMA_CIRCULAR;
    hdma_usartx_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usartx_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_usartx_tx);

    __HAL_LINKDMA(huart,hdmatx,hdma_usartx_tx);
  
  }
}

/**
  * ��������: ����Ӳ������ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USARTx)
  {
    /* ��������ʱ�ӽ��� */
    USART_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(USARTx_Tx_GPIO, USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(USARTx_Rx_GPIO, USARTx_Rx_GPIO_PIN);
    /* DMA���跴��ʼ�� */
    HAL_DMA_DeInit(huart->hdmatx);    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(USARTx_IRQn);
  }
}

/**
  * ��������: NVIC����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void MX_NVIC_USARTx_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USARTx_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
  
    
  HAL_NVIC_SetPriority(USARTx_DMAx_CHANNELn_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMAx_CHANNELn_IRQn);  
}

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_USARTx_Init(void)
{
  /* ʹ�ܴ��ڹ�������GPIOʱ�� */
  USARTx_GPIO_ClK_ENABLE();
  
  husartx.Instance = USARTx;
  husartx.Init.BaudRate = USARTx_BAUDRATE;
  husartx.Init.WordLength = UART_WORDLENGTH_8B;
  husartx.Init.StopBits = UART_STOPBITS_1;
  husartx.Init.Parity = UART_PARITY_NONE;
  husartx.Init.Mode = UART_MODE_TX_RX;
  husartx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx);
  
  /* ���ô����жϲ�ʹ�ܣ���Ҫ����HAL_UART_Init������ִ���޸Ĳ���Ч */
  MX_NVIC_USARTx_Init();
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
