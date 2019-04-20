#ifndef __BSP_BLDC_H__
#define __BSP_BLDC_H__
/*KeShengTe BLDC DRIVER*/
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
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

#define BLDCMOTOR_TIM_PWM_FREQ                 500 //18000 PWM频率

// 定义定时器预分频，定时器实际时钟频率为：168MHz/（BLDCMOTOR_TIMx_PRESCALER+1）
#define BLDCMOTOR_TIM_PRESCALER                 3// 实际时钟频率为：42MHz

// 定义定时器周期，当定时器开始计数到BLDCMOTOR_TIMx_PERIOD值并且重复计数寄存器为0时更新定时器并生成对应事件和中断
#define BLDCMOTOR_TIM_PERIOD                  (SystemCoreClock/(BLDCMOTOR_TIM_PRESCALER+1)/BLDCMOTOR_TIM_PWM_FREQ)

// 定义高级定时器重复计数寄存器值，
#define BLDCMOTOR_TIM_REPETITIONCOUNTER       0

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef      htimx_BLDC;
extern TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;
extern __IO uint32_t uwStep ;
/* 函数声明 ------------------------------------------------------------------*/

void BLDCMOTOR_TIMx_Init(void);


#endif	/* __STEPMOTOR_TIM_H__ */




