#ifndef __BSP_OUTPUT_H__
#define __BSP_OUTPUT_H__
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define AIR_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define AIR_GPIO_PIN                 GPIO_PIN_2  //�յ����
#define AIR_GPIO                     GPIOA //WT.EDIT


#define WIPER_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define WIPER_GPIO_PIN                 GPIO_PIN_0 //������
#define WIPER_GPIO                     GPIOA  //WT.EDIT

#define PLC_RCC_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define PLC_GPIO_PIN                 GPIO_PIN_8 //PLC����ź�
#define PLC_GPIO                     GPIOD  //WT.EDIT




#define AIR_ON                       HAL_GPIO_WritePin(AIR_GPIO,AIR_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define AIR_OFF                      HAL_GPIO_WritePin(AIR_GPIO,LED1_AIR_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define AIR_TOGGLE                   HAL_GPIO_TogglePin(AIR_GPIO,AIR_GPIO_PIN)                // �����ת

#define WIPER_ON                     HAL_GPIO_WritePin(WIPER_GPIO,WIPER_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define WIPER_OFF                    HAL_GPIO_WritePin(WIPER_GPIO,WIPER_GPIO_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define WIPER_TOGGLE                 HAL_GPIO_TogglePin(WIPER_GPIO,WIPER_GPIO_PIN)                // �����ת

#define PLC_ON                     HAL_GPIO_WritePin(PLC_GPIO,PLC_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define PLC_OFF                    HAL_GPIO_WritePin(PLC_GPIO,PLC_GPIO_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define PLC_TOGGLE                 HAL_GPIO_TogglePin(PLC_GPIO,PLC_GPIO_PIN)                // �����ת






/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void OUTPUT_SIG_GPIO_Init(void);



#endif

