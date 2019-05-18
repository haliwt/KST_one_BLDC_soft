#include "output/bsp_output.h"

void OUTPUT_SIG_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)LED���Ŷ�ӦIO�˿�ʱ�� */  
  AIR_RCC_CLK_ENABLE();
  WIPER_RCC_CLK_ENABLE();
  PLC_RCC_CLK_ENABLE();
  
  
  /* ���ÿյ����������ѹ */
  HAL_GPIO_WritePin(AIR_GPIO, AIR_GPIO_PIN, GPIO_PIN_RESET);
  /* ������ˢ���������ѹ */
  HAL_GPIO_WritePin(WIPER_GPIO, WIPER_GPIO_PIN, GPIO_PIN_RESET);
  /* ����PLC���������ѹ */
  HAL_GPIO_WritePin(PLC_GPIO, PLC_GPIO_PIN, GPIO_PIN_RESET);  
  
   /* �趨AIR�յ������Ӧ����IO��� */
  GPIO_InitStruct.Pin = AIR_GPIO_PIN;
  /* �趨LED1��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED1��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED1��Ӧ����IO */
  HAL_GPIO_Init(AIR_GPIO, &GPIO_InitStruct);
  
  /* �趨WIPER��ˢ����Ӧ����IO��� */
  GPIO_InitStruct.Pin = WIPER_GPIO_PIN;
  /* �趨LED2��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED2��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED2��Ӧ����IO */
  HAL_GPIO_Init(WIPER_GPIO, &GPIO_InitStruct);
  
   /* �趨PLC ��Ӧ����*/
  GPIO_InitStruct.Pin = PLC_GPIO_PIN;
  /* �趨LED2��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED2��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED2��Ӧ����IO */
  HAL_GPIO_Init(PLC_GPIO, &GPIO_InitStruct);
  
 
  
}
