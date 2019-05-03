
/* �㨹o?����???t ----------------------------------------------------------------*/
#include "adc/bsp_adc.h"
#include "usart/bsp_debug_usart.h"

/* ??��D����D��?����? --------------------------------------------------------------*/
/* ??��Do��?����? ----------------------------------------------------------------*/
/* ??��D��?��? ------------------------------------------------------------------*/
ADC_HandleTypeDef hadcx;
DMA_HandleTypeDef hdma_adcx;

/* ��??1��?��? ------------------------------------------------------------------*/
/* ??��Do����y?-D? --------------------------------------------------------------*/
/* o����y��? --------------------------------------------------------------------*/
/**
  * o����y1|?��: AD��a??3?��??��
  * ��?��?2?��y: ?T
  * ���� ?? ?��: ?T
  * ?��    ?�¡�o?T
  */
void MX_ADCx_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  hadcx.Instance = ADCx;
  hadcx.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadcx.Init.Resolution = ADC_RESOLUTION_12B;
  hadcx.Init.ScanConvMode = DISABLE;
  hadcx.Init.ContinuousConvMode = ENABLE;
  hadcx.Init.DiscontinuousConvMode = DISABLE;
  hadcx.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadcx.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx.Init.NbrOfConversion = 1;
  hadcx.Init.DMAContinuousRequests = ENABLE;
  hadcx.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  HAL_ADC_Init(&hadcx);
  
  /* ���ò���ͨ�� */
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;  
  HAL_ADC_ConfigChannel(&hadcx, &sConfig);
}

void MX_DMA_Init(void) 
{
  
  DMAx_RCC_CLK_ENABLE();
  
  /* ��a����?D??��??��??????o����1?��?D?? */
 // HAL_NVIC_SetPriority(ADCx_DMA_IRQx, 4, 0);
 // HAL_NVIC_EnableIRQ(ADCx_DMA_IRQx); 
}

/**
  * o����y1|?��: ADC��a����3?��??��????
  * ��?��?2?��y: hadc��oAD��a����??��������D��????
  * ���� ?? ?��: ?T
  * ?��    ?��: ??o����y��?HAL?a?��2?�̡¨�?
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADCx)
  {
    /* ��a��������?����1?�� */
    ADCx_RCC_CLK_ENABLE();
    
    /* AD��a??�����̨���y??����?����1?�� */
    ADC_GPIO_ClK_ENABLE();

    
    /* AD��a??�����̨���y??3?��??�� */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);

    hdma_adcx.Instance = DMAx__Stream_x;
    hdma_adcx.Init.Channel = DMAx__CHANNEL_x;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adcx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adcx);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);


  }
}

/**
  * o����y1|?��: ADC��a��������3?��??��????
  * ��?��?2?��y: hadc��oAD��a����??��������D��????
  * ���� ?? ?��: ?T
  * ?��    ?��: ??o����y��?HAL?a?��2?�̡¨�?
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADCx)
  {
    /* ??��?ADC��a��������?�� */
    ADCx_RCC_CLK_DISABLE();
  
    /* AD��a??�����̨���y??����3?��??�� */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN);

    /* ??��?��a����?D?? */
    HAL_DMA_DeInit(hadc->DMA_Handle);
  //  HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
  }
} 



