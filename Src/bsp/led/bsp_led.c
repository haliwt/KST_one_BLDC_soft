/**
  ******************************************************************************
  * �ļ�����: bsp_led.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-3-30
  * ��    ��: ���ڵ�HAL��Ĺ���ģ��
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
#include "led/bsp_led.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����LED��IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_led.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void LED_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)LED���Ŷ�ӦIO�˿�ʱ�� */  
  LED1_RCC_CLK_ENABLE();
  LED2_RCC_CLK_ENABLE();
  LED3_RCC_CLK_ENABLE();
  /* ����LED1���������ѹ */
  HAL_GPIO_WritePin(LED1_GPIO, LED1_GPIO_PIN, GPIO_PIN_RESET);
  /* ����LED2���������ѹ */
  HAL_GPIO_WritePin(LED2_GPIO, LED2_GPIO_PIN, GPIO_PIN_RESET);
  /* ����LED3���������ѹ */
  HAL_GPIO_WritePin(LED2_GPIO, LED3_GPIO_PIN, GPIO_PIN_RESET);  
  /* �趨LED1��Ӧ����IO��� */
  GPIO_InitStruct.Pin = LED1_GPIO_PIN;
  /* �趨LED1��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED1��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED1��Ӧ����IO */
  HAL_GPIO_Init(LED1_GPIO, &GPIO_InitStruct);
  
  /* �趨LED2��Ӧ����IO��� */
  GPIO_InitStruct.Pin = LED2_GPIO_PIN;
  /* �趨LED2��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED2��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED2��Ӧ����IO */
  HAL_GPIO_Init(LED2_GPIO, &GPIO_InitStruct);
  
  /* �趨LED3��Ӧ����IO��� */
  GPIO_InitStruct.Pin = LED3_GPIO_PIN;
  /* �趨LED3��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED3��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED3��Ӧ����IO */
  HAL_GPIO_Init(LED3_GPIO, &GPIO_InitStruct);
  
}

/**
  * ��������: ���ð���LED�Ƶ�״̬
  * �������: LEDx:����x������Ϊ(1,2,3)����ѡ���Ӧ��LED��
  * ���������state:����LED�Ƶ����״̬��
  *             ��ѡֵ��LED_OFF��LED����
  *                     LED_ON�� LED������
  *                     LED_TOGGLE����תLED
  * �� �� ֵ: ��
  * ˵    �����ú���ʹ�����Ʊ�׼�⺯���ı�̷�������������׼�⺯�����˼�롣
  */
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state)
{
  /* �����������Ƿ�Ϸ� */
  assert_param(IS_LED_TYPEDEF(LEDx));
  assert_param(IS_LED_STATE(state));
  
  /* �ж����õ�LED��״̬���������ΪLED���� */
  if(state==LED_OFF)
  {
    if(LEDx & LED1)            
      LED1_OFF;/* LED1�� */
    
    if(LEDx & LED2)
      LED2_OFF;/* LED2�� */
    
    if(LEDx & LED3)
      LED3_OFF;/* LED3�� */    
  }
  else if(state==LED_ON) /* ����LED��Ϊ�� */
  {
    if(LEDx & LED1)
      LED1_ON;/* LED1�� */
    
    if(LEDx & LED2)
      LED2_ON;/* LED2�� */
    
    if(LEDx & LED3)
      LED3_ON;/* LED3�� */ 
  }
  else
  {
    if(LEDx & LED1)
      LED1_TOGGLE;/* �������������ת */
    
    if(LEDx & LED2)
      LED2_TOGGLE;/* �������������ת */
    
    if(LEDx & LED3)
      LED3_TOGGLE;/* �������������ת */ 
  }
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
