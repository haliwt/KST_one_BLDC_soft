
/* 包含头文件 ----------------------------------------------------------------*/
#include "adc/bsp_adc.h"
#include "usart/bsp_usartx.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
ADC_HandleTypeDef hadcx,hadcx1;
DMA_HandleTypeDef hdma_adcx,hdma_adcx1;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: AD转换初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MX_ADCx_Init(void)
{
 ADC_ChannelConfTypeDef sConfig;

  hadcx.Instance = ADCx;
  hadcx.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadcx.Init.Resolution = ADC_RESOLUTION_12B;
  hadcx.Init.ScanConvMode = ENABLE;
  hadcx.Init.ContinuousConvMode = ENABLE;
  hadcx.Init.DiscontinuousConvMode = DISABLE;
  hadcx.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadcx.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx.Init.NbrOfConversion = ADC_CHANNEL_NUMBER;
  hadcx.Init.DMAContinuousRequests = ENABLE;
  hadcx.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  HAL_ADC_Init(&hadcx);
	
	// 配置采样通道
  sConfig.Channel = ADC_CHANNEL2;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;  
  HAL_ADC_ConfigChannel(&hadcx, &sConfig);
  
  sConfig.Channel = ADC_CHANNEL4;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadcx, &sConfig);
  
}

void MX_DMA_Init(void) 
{
  
  DMAx_RCC_CLK_ENABLE();
  
  /* 外设中断优先级配置和使能中断 */
 // HAL_NVIC_SetPriority(ADCx_DMA_IRQx, 4, 0);
 // HAL_NVIC_EnableIRQ(ADCx_DMA_IRQx); 
}


/**
  * 函数功能: ADC外设初始化配置
  * 输入参数: hadc：AD外设句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADCx)
  {
    /* 外设时钟使能 */
    ADCx_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
    
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	  
	HAL_GPIO_Init(ADC_GPIO2, &GPIO_InitStruct);
	  
	GPIO_InitStruct.Pin = ADC_GPIO_PIN4;
    HAL_GPIO_Init(ADC_GPIO4, &GPIO_InitStruct);
    #if 0
    HAL_GPIO_Init(ADC_GPIO1, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADC_GPIO_PIN2;
    HAL_GPIO_Init(ADC_GPIO2, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN3;
    HAL_GPIO_Init(ADC_GPIO3, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN4;
    HAL_GPIO_Init(ADC_GPIO4, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN5;
    HAL_GPIO_Init(ADC_GPIO5, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN6;
    HAL_GPIO_Init(ADC_GPIO6, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN7;
    HAL_GPIO_Init(ADC_GPIO7, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ADC_GPIO_PIN8;
    HAL_GPIO_Init(ADC_GPIO8, &GPIO_InitStruct);    
  #endif 
    hdma_adcx.Instance = DMAx_Streamn;
    hdma_adcx.Init.Channel = DMA_CHANNEL;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adcx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adcx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adcx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adcx.Init.PeriphBurst = DMA_PBURST_SINGLE;    
    HAL_DMA_Init(&hdma_adcx);
    
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
  }
}