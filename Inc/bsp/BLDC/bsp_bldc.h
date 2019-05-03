#ifndef __BSP_BLDC_H__
#define __BSP_BLDC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct {
  __IO int32_t  uwStep ;  	  // 电机旋转状态
  __IO int32_t  Dir ;        	// 电机旋转方向
  __IO int32_t  Position;		  // 电机位置
  __IO int32_t  Speed;		    // 电机速度
  __IO uint16_t PWM_Duty; 	  // 电机占空比
  __IO int32_t  Hall_Period;  // 霍尔传感器触发周期
  __IO int32_t  Hall_PulNum;  // 霍尔传感器脉冲数
  __IO int32_t  Lock_Time;    // 电机堵转时间
}BLDC_Typedef;

/* 宏定义 --------------------------------------------------------------------*/
#define BLDCMOTOR_TIMx                        TIM1
#define BLDCMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define BLDCMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE()   {__HAL_RCC_GPIOE_CLK_ENABLE();} //;__HAL_RCC_GPIOB_CLK_ENABLE();}
#define BLDCMOTOR_TIM_CH1_PORT                GPIOE
#define BLDCMOTOR_TIM_CH1_PIN                 GPIO_PIN_9    // U
#define BLDCMOTOR_TIM_CH2_PORT                GPIOE
#define BLDCMOTOR_TIM_CH2_PIN                 GPIO_PIN_11    // V
#define BLDCMOTOR_TIM_CH3_PORT                GPIOE
#define BLDCMOTOR_TIM_CH3_PIN                 GPIO_PIN_13   // W

#define BLDCMOTOR_TIM_CH1N_PORT               GPIOE
#define BLDCMOTOR_TIM_CH1N_PIN                GPIO_PIN_8  // U
#define BLDCMOTOR_TIM_CH2N_PORT               GPIOE
#define BLDCMOTOR_TIM_CH2N_PIN                GPIO_PIN_10   // V
#define BLDCMOTOR_TIM_CH3N_PORT               GPIOE
#define BLDCMOTOR_TIM_CH3N_PIN                GPIO_PIN_12   // W

#define BLDC_TIM_IRQHandler                   TIM1_TRG_COM_TIM11_IRQHandler
#define BLDC_TIM_IRQn                         TIM1_TRG_COM_TIM11_IRQn

#define BLDCMOTOR_TIM_BKIN_PORT               GPIOE//GPIOB             // Break in
#define BLDCMOTOR_TIM_BKIN_PIN                GPIO_PIN_15//WT.EDIT GPIO_PIN_12

#define BLDCMOTOR_TIM_PWM_FREQ                22000 // PWM频率

// 定义定时器预分频，定时器实际时钟频率为：168MHz/（BLDCMOTOR_TIMx_PRESCALER+1）
#define BLDCMOTOR_TIM_PRESCALER                 3   // 实际时钟频率为：168/4MHz

// 定义定时器周期，当定时器开始计数到BLDCMOTOR_TIMx_PERIOD值并且重复计数寄存器为0时更新定时器并生成对应事件和中断
#define BLDCMOTOR_TIM_PERIOD                  (SystemCoreClock/(BLDCMOTOR_TIM_PRESCALER+1)/BLDCMOTOR_TIM_PWM_FREQ)

// 定义高级定时器重复计数寄存器值，
#define BLDCMOTOR_TIM_REPETITIONCOUNTER       0


/* ##**-------- 霍尔传感器接口定义 ---------------------------------**## */
#define HALL_TIMx                        TIM4
#define HALL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM4_CLK_ENABLE()
#define HALL_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM4_CLK_DISABLE()

#define HALL_TIM_GPIO_CLK_ENABLE()       {__HAL_RCC_GPIOD_CLK_ENABLE();__HAL_RCC_GPIOB_CLK_ENABLE();}
#define HALL_TIM_CH1_PIN                 GPIO_PIN_12    // U
#define HALL_TIM_CH1_GPIO                GPIOD
#define HALL_TIM_CH2_PIN                 GPIO_PIN_13    // V
#define HALL_TIM_CH2_GPIO                GPIOD
#define HALL_TIM_CH3_PIN                 GPIO_PIN_14//GPIO_PIN_8     // W
#define HALL_TIM_CH3_GPIO                GPIOD //GPIOB

// 定义定时器预分频，定时器实际时钟频率为：84MHz/（HALL_TIMx_PRESCALER+1）
#define HALL_TIM_PRESCALER               167      // 84M/168 = 500K Hz

// 定义定时器周期，当定时器开始计数到HALL_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define HALL_TIM_PERIOD                  0xFFFF   // 溢出时间是100ms  足够霍尔传感器连续信号的间隔

#define HALL_TIM_FREQ                    ((HAL_RCC_GetPCLK1Freq()*2)/(HALL_TIM_PRESCALER+1))

#define HALL_TIM_IRQn                    TIM4_IRQn
#define HALL_TIM_IRQHANDLER              TIM4_IRQHandler

#define CCW                              (int32_t)(1)
#define CW                               (int32_t)(-1)

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef      htimx_BLDC;
extern TIM_HandleTypeDef      htimx_HALL;
extern TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;

extern __IO uint32_t PWM_ChangeFlag ;

extern BLDC_Typedef BLDCMotor ;

extern __IO int32_t  uwStep ;

extern __IO uint32_t Lock_Time; // 堵转时间
extern __IO float   PWM_Duty;
extern __IO int32_t Dir;
/* 函数声明 ------------------------------------------------------------------*/
void HALL_TIMx_Init(void);
void BLDCMOTOR_TIMx_Init(void);
void Enable_BLDC(void );
void Disable_BLDC(void);
void BLDC_Ctrl(int32_t Dir,float PWM_Duty);

#endif	/* __STEPMOTOR_TIM_H__ */

