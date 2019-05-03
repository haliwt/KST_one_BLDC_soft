#ifndef __ADC_H__
#define	__ADC_H__

/* ¡ã¨¹o?¨ª¡¤???t ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ¨¤¨¤D¨ª?¡§¨°? ------------------------------------------------------------------*/
/* o¨º?¡§¨°? --------------------------------------------------------------------*/
// ¡Á¡é¨°a¡êo¨®?¡Á¡ÂADC2¨¦?¡¥¦Ì?IO¡À?D???¨®D?¡ä¨®?¡ê?¡¤??¨°2¨¦?¡¥¦Ì??1?¨¢¨®D¨®¡ã?¨¬
/********************ADC¨º?¨¨?¨ª¡§¦Ì¨¤¡ê¡§¨°y??¡ê?????**************************/
//#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
//#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()
//#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA2_CLK_ENABLE()
//#define ADCx                             ADC1
//#define ADCx_DMA_IRQx                    DMA2_Stream0_IRQn
//#define ADCx_DMA_IRQx_Handler            DMA2_Stream0_IRQHandler
//#define DMAx__Stream_x                   DMA2_Stream0
//#define DMAx__CHANNEL_x                  DMA_CHANNEL_0

//#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
//#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()
//#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA2_CLK_ENABLE()
//#define ADCx                             ADC2
//#define ADCx_DMA_IRQx                    DMA2_Stream2_IRQn
//#define ADCx_DMA_IRQx_Handler            DMA2_Stream2_IRQHandler
//#define DMAx__Stream_x                   DMA2_Stream2
//#define DMAx__CHANNEL_x                  DMA_CHANNEL_1
#if 1
#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()
#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx                             ADC2
#define ADCx_DMA_IRQx                    DMA2_Stream3_IRQn
#define ADCx_DMA_IRQx_Handler            DMA2_Stream3_IRQHandler
#define DMAx__Stream_x                   DMA2_Stream3
#define DMAx__CHANNEL_x                  DMA_CHANNEL_1


#define ADC_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO                         GPIOA
#define ADC_GPIO_PIN                     GPIO_PIN_5   //GPIO_PIN_0        
#define ADC_CHANNEL                      ADC_CHANNEL_5 //ADC_CHANNEL_0  

#endif

#if 0
#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC3_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC3_CLK_DISABLE()
#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx                             ADC3
#define ADCx_DMA_IRQx                    DMA2_Stream0_IRQn
#define ADCx_DMA_IRQx_Handler            DMA2_Stream0_IRQHandler
#define DMAx__Stream_x                   DMA2_Stream0
#define DMAx__CHANNEL_x                  DMA_CHANNEL_2


#define ADC_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO                         GPIOA
#define ADC_GPIO_PIN                     GPIO_PIN_0        
#define ADC_CHANNEL                      ADC_CHANNEL_0    
#endif                                
/* ¨¤??1¡À?¨¢? ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx;
extern DMA_HandleTypeDef hdma_adcx;

/* o¡¥¨ºy¨¦¨´?¡Â ------------------------------------------------------------------*/
void MX_ADCx_Init(void);
void MX_DMA_Init(void) ;
#endif /* __ADC_H__ */



