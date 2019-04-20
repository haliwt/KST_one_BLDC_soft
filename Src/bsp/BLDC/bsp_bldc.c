
#include "bldc/bsp_bldc.h" 
#include "main.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define PWM_DUTY          0.55f
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef      htimx_BLDC;

/* 定时器PWM输出通道结构体声明 */
TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;

/* Step Index */
 __IO uint32_t uwStep = 0;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 定时器硬件初始化配置
  * 输入参数: htim：定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被BLDCMOTOR_TIMx_Init函数调用
  */
void BLDCMOTOR_TIM_MspPostInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* 定时器通道功能引脚端口时钟使能 */
  BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
  
  
  /* 定时器通道1功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
  /* 定时器通道1互补通道功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1N_PORT, &GPIO_InitStruct);

  /* 定时器通道2功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
  /* 定时器通道2互补通道功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2N_PORT, &GPIO_InitStruct);
  
  /* 定时器通道3功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
  /* 定时器通道3互补通道功能引脚IO初始化 */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* 定时器制动功能引脚IO初始化 */
  GPIO_InitStruct.Pin = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_BKIN_PORT, &GPIO_InitStruct);
  
}

/*********************************************
  *
  * 函数功能: 基本定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  *
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  /* 基本定时器外设时钟使能 */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  BLDCMOTOR_TIM_MspPostInit();
  
  /* 基本定时器功能配置 */
  htimx_BLDC.Instance = BLDCMOTOR_TIMx;
  htimx_BLDC.Init.Prescaler = BLDCMOTOR_TIM_PRESCALER;
  htimx_BLDC.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_BLDC.Init.Period = BLDCMOTOR_TIM_PERIOD;
  htimx_BLDC.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  htimx_BLDC.Init.RepetitionCounter = BLDCMOTOR_TIM_REPETITIONCOUNTER;
  HAL_TIM_PWM_Init(&htimx_BLDC);
  
  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimx_BLDC, &sClockSourceConfig);
  
  /* 死区和刹车配置 */
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 10;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BLDC, &sBreakDeadTimeConfig);

  /* 输出比较通道配置成PWM模式 */
  sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
  sPWMConfig1.Pulse = (uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY);
  sPWMConfig1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sPWMConfig1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sPWMConfig1.OCFastMode = TIM_OCFAST_DISABLE;
  sPWMConfig1.OCIdleState = TIM_OCIDLESTATE_SET;
  sPWMConfig1.OCNIdleState = TIM_OCNIDLESTATE_SET;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
  
  /* 定时器CH2配置 */
  sPWMConfig2 = sPWMConfig1;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);
  
  /* 定时器CH3配置 */
  sPWMConfig3 = sPWMConfig1;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
  
  
  /* 配置换相触发：由软件触发换相 */
  HAL_TIMEx_ConfigCommutationEvent_IT(&htimx_BLDC, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);
  
  /* 配置定时器中断优先级并使能 */
  HAL_NVIC_SetPriority(BLDC_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(BLDC_TIM_IRQn);
  
  /* 关断输出 */
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);

}
/**
  * 函数功能：定时器换相中断回调函数 
  * 输入参数：无
  * 返 回 值：无
  * 说    明：无
  */
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
   /* 定义定时器输出通道1为A相 输出通道2为B相，输出通道3为C相 */
  if (uwStep == 0)//  A+ C-
  {
    /*  关断B相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
    /* 下一步: A相配置为PWM模式1  */
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    
    /*  C相配置为PWM模式2，在相同占空比时，AC相共同输出双极性PWM脉冲 */
    sPWMConfig3.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
    
  }

  if (uwStep == 1)//  A+ B-
  {
    /* 关断C相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    
    /* 下一步: */
    /* 跟上一步是相同的配置 */  
    
    /*  A相配置为PWM模式1，B相配置为PWM模式2，输出双极性PWM脉冲*/
//    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    
    /* Channel2 Configuration -------------------------------------- */
    sPWMConfig2.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
    
  }
  
  else if (uwStep == 2)//  C+ B-
  {
    /* 关断A相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
    
    /* Same configuration as the previous step */
//    sPWMConfig2.OCMode = TIM_OCMODE_PWM2;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
    
    /* ----------------------------------------------------------------------------- */
    sPWMConfig3.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
 
  }
  
  else if (uwStep == 3)//  C+ A-
  {
    /* 关断B相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
    /*  Channel1 configuration */
    sPWMConfig1.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    
    /* ----------------------------------------------------------------------------- */
    /* Same configuration as the previous step */
//    sPWMConfig3.OCMode = TIM_OCMODE_PWM1;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
    
    
  }
  else if (uwStep == 4)//  B+ A-
  {
    /* 关断C相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    
    /* Same configuration as the previous step */
    /*  Channel1 configuration */
    
//    sPWMConfig1.OCMode = TIM_OCMODE_PWM2;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    /* ----------------------------------------------------------------------------- */
    sPWMConfig2.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
  }
  
  else if (uwStep == 5)//  B+ C-
  {
    /* 关断A相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
    
    /* Same configuration as the previous step */
    
    /*  Channel2 configuration */
//    sPWMConfig2.OCMode = TIM_OCMODE_PWM1;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
    
    /* ----------------------------------------------------------------------------- */
    sPWMConfig3.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
    
  }
  
  else if(uwStep == 6)//  A+ C-
  {
    /* 关断B相 */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
    /*  Channel1 configuration */
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
    
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    
    /* ----------------------------------------------------------------------------- */
    /* Same configuration as the previous step */
//    sPWMConfig3.OCMode = TIM_OCMODE_PWM2;
//    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
//    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
//    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
//    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
    
  }
}

