#ifndef __BSP_BLDC_H__
#define __BSP_BLDC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct {
  __IO int32_t  uwStep ;  	  // �����ת״̬
  __IO int32_t  Dir ;        	// �����ת����
  __IO int32_t  Position;		  // ���λ��
  __IO int32_t  Speed;		    // ����ٶ�
  __IO uint16_t PWM_Duty; 	  // ���ռ�ձ�
  __IO int32_t  Hall_Period;  // ������������������
  __IO int32_t  Hall_PulNum;  // ����������������
  __IO int32_t  Lock_Time;    // �����תʱ��
}BLDC_Typedef;

/* �궨�� --------------------------------------------------------------------*/
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

#define BLDCMOTOR_TIM_PWM_FREQ                22000 // PWMƵ��

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/��BLDCMOTOR_TIMx_PRESCALER+1��
#define BLDCMOTOR_TIM_PRESCALER                 3   // ʵ��ʱ��Ƶ��Ϊ��168/4MHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������BLDCMOTOR_TIMx_PERIODֵ�����ظ������Ĵ���Ϊ0ʱ���¶�ʱ�������ɶ�Ӧ�¼����ж�
#define BLDCMOTOR_TIM_PERIOD                  (SystemCoreClock/(BLDCMOTOR_TIM_PRESCALER+1)/BLDCMOTOR_TIM_PWM_FREQ)

// ����߼���ʱ���ظ������Ĵ���ֵ��
#define BLDCMOTOR_TIM_REPETITIONCOUNTER       0


/* ##**-------- �����������ӿڶ��� ---------------------------------**## */
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

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��84MHz/��HALL_TIMx_PRESCALER+1��
#define HALL_TIM_PRESCALER               167      // 84M/168 = 500K Hz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������HALL_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define HALL_TIM_PERIOD                  0xFFFF   // ���ʱ����100ms  �㹻���������������źŵļ��

#define HALL_TIM_FREQ                    ((HAL_RCC_GetPCLK1Freq()*2)/(HALL_TIM_PRESCALER+1))

#define HALL_TIM_IRQn                    TIM4_IRQn
#define HALL_TIM_IRQHANDLER              TIM4_IRQHandler

#define CCW                              (int32_t)(1)
#define CW                               (int32_t)(-1)

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef      htimx_BLDC;
extern TIM_HandleTypeDef      htimx_HALL;
extern TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;

extern __IO uint32_t PWM_ChangeFlag ;

extern BLDC_Typedef BLDCMotor ;

extern __IO int32_t  uwStep ;

extern __IO uint32_t Lock_Time; // ��תʱ��
extern __IO float   PWM_Duty;
extern __IO int32_t Dir;
/* �������� ------------------------------------------------------------------*/
void HALL_TIMx_Init(void);
void BLDCMOTOR_TIMx_Init(void);
void Enable_BLDC(void );
void Disable_BLDC(void);
void BLDC_Ctrl(int32_t Dir,float PWM_Duty);

#endif	/* __STEPMOTOR_TIM_H__ */

