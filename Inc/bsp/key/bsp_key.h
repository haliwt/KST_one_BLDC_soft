#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define KEY1_RCC_CLK_ENABLE           __HAL_RCC_GPIOB_CLK_ENABLE
#define KEY1_GPIO_PIN                 GPIO_PIN_0  //WT.EDIT START
#define KEY1_GPIO                     GPIOB
#define KEY1_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ1 */

#define KEY2_RCC_CLK_ENABLE           __HAL_RCC_GPIOB_CLK_ENABLE
#define KEY2_GPIO_PIN                 GPIO_PIN_1 //WT.EDIT  DIR
#define KEY2_GPIO                     GPIOB
#define KEY2_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY3_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define KEY3_GPIO_PIN                 GPIO_PIN_4 //WT.EDIT BRAKE
#define KEY3_GPIO                     GPIOC
#define KEY3_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY4_RCC_CLK_ENABLE           __HAL_RCC_GPIOE_CLK_ENABLE
#define KEY4_GPIO_PIN                 GPIO_PIN_3
#define KEY4_GPIO                     GPIOE
#define KEY4_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY5_RCC_CLK_ENABLE           __HAL_RCC_GPIOE_CLK_ENABLE
#define KEY5_GPIO_PIN                 GPIO_PIN_4
#define KEY5_GPIO                     GPIOE
#define KEY5_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
KEYState_TypeDef START_StateRead(void);
KEYState_TypeDef DIR_StateRead(void);
KEYState_TypeDef BRAKE_StateRead(void);
KEYState_TypeDef KEY4_StateRead(void);
KEYState_TypeDef KEY5_StateRead(void);

#endif  // __BSP_KEY_H__

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
