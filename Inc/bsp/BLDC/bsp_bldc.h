#ifndef __BSP_BLDC_H__
#define __BSP_BLDC_H__
/*KeShengTe BLDC DRIVER*/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define BLDCMOTOR_TIMx                        TIM1
#define BLDCMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define BLDCMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE()   {__HAL_RCC_GPIOE_CLK_ENABLE();__HAL_RCC_GPIOE_CLK_ENABLE();}
#define BLDCMOTOR_TIM_CH1_PORT                GPIOE//GPIOA
#define BLDCMOTOR_TIM_CH1_PIN                 GPIO_PIN_9//GPIO_PIN_8
#define BLDCMOTOR_TIM_CH2_PORT                GPIOE//GPIOA
#define BLDCMOTOR_TIM_CH2_PIN                 GPIO_PIN_11//GPIO_PIN_9
#define BLDCMOTOR_TIM_CH3_PORT                GPIOE//GPIOA
#define BLDCMOTOR_TIM_CH3_PIN                 GPIO_PIN_13//GPIO_PIN_10

#define BLDCMOTOR_TIM_CH1N_PORT               GPIOE//GPIOB
#define BLDCMOTOR_TIM_CH1N_PIN                GPIO_PIN_8//GPIO_PIN_13
#define BLDCMOTOR_TIM_CH2N_PORT               GPIOE//GPIOB
#define BLDCMOTOR_TIM_CH2N_PIN                GPIO_PIN_10//GPIO_PIN_14
#define BLDCMOTOR_TIM_CH3N_PORT               GPIOE//GPIOB
#define BLDCMOTOR_TIM_CH3N_PIN                GPIO_PIN_12//GPIO_PIN_15

#define GPIOx_AFx_TIMx                        GPIO_AF1_TIM1

//TIM1_TRG_COM_TIM11_IRQHandler
#define BLDC_TIM_IRQHandler                   TIM1_TRG_COM_TIM11_IRQHandler
#define BLDC_TIM_IRQn                         TIM1_TRG_COM_TIM11_IRQn

#define BLDCMOTOR_TIM_BKIN_PORT               GPIOE//GPIOB  //WT.EDIT 
#define BLDCMOTOR_TIM_BKIN_PIN                GPIO_IN_15//GPIO_PIN_12

#define BLDCMOTOR_TIM_PWM_FREQ                 500 //18000 PWMƵ��

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/��BLDCMOTOR_TIMx_PRESCALER+1��
#define BLDCMOTOR_TIM_PRESCALER                 3// ʵ��ʱ��Ƶ��Ϊ��42MHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������BLDCMOTOR_TIMx_PERIODֵ�����ظ������Ĵ���Ϊ0ʱ���¶�ʱ�������ɶ�Ӧ�¼����ж�
#define BLDCMOTOR_TIM_PERIOD                  (SystemCoreClock/(BLDCMOTOR_TIM_PRESCALER+1)/BLDCMOTOR_TIM_PWM_FREQ)

// ����߼���ʱ���ظ������Ĵ���ֵ��
#define BLDCMOTOR_TIM_REPETITIONCOUNTER       0

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef      htimx_BLDC;
extern TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;
extern __IO uint32_t uwStep ;
/* �������� ------------------------------------------------------------------*/

void BLDCMOTOR_TIMx_Init(void);


#endif	/* __STEPMOTOR_TIM_H__ */




