#include "hallsw/bsp_hallsw.h"

#if 0
void HALL_JUDGE_Init(void)
{
     /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)LED���Ŷ�ӦIO�˿�ʱ�� */  
  HALL_FAIL_RCC_CLK_ENABLE();
 
  /* ����hall fail ���������ѹ */
  HAL_GPIO_WritePin(HALL_FAIL_GPIO, HALL_FAIL_GPIO_PIN, GPIO_PIN_SET);
  
   
  /* �趨hall fail ��Ӧ����IO��� */
  GPIO_InitStruct.Pin = HALL_FAIL_GPIO_PIN;
  /* �趨LED1��Ӧ����IOΪ���ģʽ */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* �趨LED1��Ӧ����IO�����ٶ� */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* ��ʼ��LED1��Ӧ����IO */
  HAL_GPIO_Init(HALL_FAIL_GPIO, &GPIO_InitStruct);


}

#endif     
