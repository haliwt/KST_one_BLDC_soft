
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "dma_usart3/bsp_usart3.h"

UART_HandleTypeDef husart_usart3;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
#if 0
/**
  * ��������: ����Ӳ����ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance==HMI_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    DEBUG_USART_RCC_CLK_ENABLE();
  
    /* �������蹦��GPIO���� */
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
  * ��������: ����Ӳ������ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==HMI_USARTx)
  {
    /* ��������ʱ�ӽ��� */
    HMI_USART_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(HMI_USARTx_Tx_GPIO, HMI_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(HMI_USARTx_Rx_GPIO, HMI_USARTx_Rx_GPIO_PIN);
    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(HMI_USART_IRQn);
  }
}
#endif 
/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_USART3_Init(void)
{
  /* ʹ�ܴ��ڹ�������GPIOʱ�� */
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
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart_usart3, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart_usart3,&ch, 1, 0xffff);
  return ch;
}
#endif 

