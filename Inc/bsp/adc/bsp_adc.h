#ifndef __ADC_H__
#define	__ADC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC输入通道（引脚）配置**************************/
//#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
//#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()
//#define ADCx                             ADC1

//#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
//#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()
//#define ADCx                             ADC2

#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()//__HAL_RCC_ADC3_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()//__HAL_RCC_ADC3_CLK_DISABLE()
#define ADCx                             ADC2 //ADC3


#define ADC_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO                         GPIOA
#define ADC_GPIO_PIN                     GPIO_PIN_5 //WT.EDIT GPIO_PIN_0        
#define ADC_CHANNEL                      ADC_CHANNEL_5//WT.EDIT //ADC_CHANNEL_0    
                                 
/* 扩展变量 ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx;

/* 函数声明 ------------------------------------------------------------------*/
void MX_ADCx_Init(void);

#endif /* __ADC_H__ */


