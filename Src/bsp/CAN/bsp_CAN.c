/**
  ******************************************************************************
  * �ļ�����: bsp_debug_usart.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ص��Դ��ڵײ���������Ĭ��ʹ��USART1
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "CAN/bsp_CAN.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
CAN_HandleTypeDef hCAN;
static CanTxMsgTypeDef        TxMessage;
static CanRxMsgTypeDef        RxMessage;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: CAN�����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void MX_CAN_Init(void)
{
  CAN_FilterConfTypeDef  sFilterConfig;
  
  /*CAN��Ԫ��ʼ��*/
  hCAN.Instance = CANx;             /* CAN���� */
  hCAN.pTxMsg = &TxMessage;
  hCAN.pRxMsg = &RxMessage;
  
  hCAN.Init.Prescaler = 3;          /* BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.9Mbps */
  hCAN.Init.Mode = CAN_MODE_NORMAL; /* ��������ģʽ */
  hCAN.Init.SJW = CAN_SJW_1TQ;      /* BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ */
  hCAN.Init.BS1 = CAN_BS1_6TQ;      /* BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ */
  hCAN.Init.BS2 = CAN_BS2_7TQ;      /* BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ */
  hCAN.Init.TTCM = DISABLE;         /* MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ�� */
  hCAN.Init.ABOM = ENABLE;          /* MCR-ABOM  �Զ����߹��� */
  hCAN.Init.AWUM = ENABLE;          /* MCR-AWUM  ʹ���Զ�����ģʽ */
  hCAN.Init.NART = DISABLE;         /* MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش� */
  hCAN.Init.RFLM = DISABLE;         /* MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б��� */
  hCAN.Init.TXFP = DISABLE;         /* MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� */
  HAL_CAN_Init(&hCAN);
  
  /*CAN��������ʼ��*/
  sFilterConfig.FilterNumber = 0;                    /* ��������0 */
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  /* �����ڱ�ʶ������λģʽ */
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; /* ������λ��Ϊ����32λ��*/
  /* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */
  
  sFilterConfig.FilterIdHigh         = (((uint32_t)0x1314<<3)&0xFFFF0000)>>16;				/* Ҫ���˵�ID��λ */
  sFilterConfig.FilterIdLow          = (((uint32_t)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; /* Ҫ���˵�ID��λ */
  sFilterConfig.FilterMaskIdHigh     = 0xFFFF;			/* ��������16λÿλ����ƥ�� */
  sFilterConfig.FilterMaskIdLow      = 0xFFFF;			/* ��������16λÿλ����ƥ�� */
  sFilterConfig.FilterFIFOAssignment = 0;           /* ��������������FIFO 0 */
  sFilterConfig.FilterActivation = ENABLE;          /* ʹ�ܹ����� */ 
  sFilterConfig.BankNumber = 14;
  HAL_CAN_ConfigFilter(&hCAN, &sFilterConfig);
}

/**
  * ��������: CAN���ų�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan->Instance==CANx)
  {
    /* Peripheral clock enable */
    CANx_CLK_ENABLE();
    CANx_GPIO_CLK_ENABLE();
    
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = CANx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate=GPIO_AF9_CAN1;
    HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CANx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate=GPIO_AF9_CAN1;
    HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStruct);


    /* ��ʼ���ж����ȼ� */
    HAL_NVIC_SetPriority(CANx_RX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
  }
}

/**
  * ��������: CAN���ŷ���ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if(hcan->Instance==CANx)
  {
    CANx_FORCE_RESET();
    CANx_RELEASE_RESET();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(CANx_TX_GPIO_PORT, CANx_TX_PIN);
    HAL_GPIO_DeInit(CANx_RX_GPIO_PORT, CANx_RX_PIN);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CANx_RX_IRQn);
  }
} 



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
