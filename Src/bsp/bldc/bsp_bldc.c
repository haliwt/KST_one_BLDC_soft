
/* 包含头文件 ----------------------------------------------------------------*/
#include "bldc/bsp_bldc.h" 
#include "main.h"
#include "led/bsp_led.h"

/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef      htimx_BLDC;
TIM_HandleTypeDef      htimx_HALL;
/* 定时器PWM输出通道结构体声明 */
TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;
BLDC_Typedef BLDCMotor = {0,CW,0,0,100,0,0,0};
__IO int32_t uwStep = 0;      // 6步引脚状态
__IO int32_t Dir = CCW;       // 旋转方向
__IO uint32_t Lock_Time = 0;  // 堵转时间
/* 扩展变量 ------------------------------------------------------------------*/

 /* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 定时器硬件初始化配置
  * 输入参数: htim：定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数由库函数调用
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance == BLDCMOTOR_TIMx)
  {
    /* 定时器通道功能引脚端口时钟使能 */
    BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
    
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
    /* 定时器通道1互补通道功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH1N_PORT, &GPIO_InitStruct);
    
    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
    /* 定时器通道2互补通道功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH2N_PORT, &GPIO_InitStruct);
    
    /* 定时器通道3功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
    /* 定时器通道3互补通道功能引脚IO初始化 */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
    
    /* 定时器制动功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_BKIN_PORT, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 基本定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  
  /* 基本定时器外设时钟使能 */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  
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

  /* 输出比较通道配置成PWM模式 */
  sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
  sPWMConfig1.Pulse = (uint16_t)(BLDCMOTOR_TIM_PERIOD * BLDCMotor.PWM_Duty);
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

  /* 配置定时器换相事件触发源 */
  HAL_TIMEx_ConfigCommutationEvent(&htimx_BLDC, TIM_TS_ITR3, TIM_COMMUTATION_SOFTWARE);
  
  /* 关断输出 */
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
}

/**
  * 函数功能: 定时器霍尔传感器接口硬件初始化配置
  * 输入参数: htim：定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数由库函数调用
  */
void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance == HALL_TIMx)
  {
    HALL_TIM_GPIO_CLK_ENABLE();
    
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = HALL_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = HALL_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH2_GPIO, &GPIO_InitStruct);
    
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = HALL_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH3_GPIO, &GPIO_InitStruct);
  }
}
/**
  * 函数功能: 定时器霍尔传感器接口初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HALL_TIMx_Init(void)
{
  TIM_HallSensor_InitTypeDef  sHallSensorConfig;  
  
  /* 基本定时器外设时钟使能 */
  HALL_TIM_RCC_CLK_ENABLE();
  
  /* 定时器基本功能配置 */
  htimx_HALL.Instance = HALL_TIMx;
  htimx_HALL.Init.Prescaler = HALL_TIM_PRESCALER;       // 预分频
  htimx_HALL.Init.CounterMode = TIM_COUNTERMODE_UP;     // 向上计数
  htimx_HALL.Init.Period = HALL_TIM_PERIOD;             // 计数周期
  htimx_HALL.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; // 时钟分频
  
  sHallSensorConfig.IC1Prescaler=TIM_ICPSC_DIV1;        // 输入捕获分频
  sHallSensorConfig.IC1Polarity=TIM_ICPOLARITY_BOTHEDGE;// 输入捕获极性
  sHallSensorConfig.IC1Filter= 10;                      // 输入滤波
  sHallSensorConfig.Commutation_Delay = 0U;             // 0 ,不使用延迟触发
  HAL_TIMEx_HallSensor_Init(&htimx_HALL,&sHallSensorConfig);
  
  HAL_NVIC_SetPriority(HALL_TIM_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);
}

/**
  * 函数功能: 使能无刷电机转动
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void Enable_BLDC(void )
{ 
  /* 使能霍尔传感器接口 和 PWM输出功能 */
  __HAL_TIM_ENABLE_IT(&htimx_HALL, TIM_IT_TRIGGER);      
  HAL_TIM_TriggerCallback(&htimx_HALL); 
  HAL_TIMEx_HallSensor_Start(&htimx_HALL); 
}

/**
  * 函数功能: 停止电机转动
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void Disable_BLDC(void)
{
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  
  __HAL_TIM_DISABLE_IT(&htimx_HALL, TIM_IT_TRIGGER);  // 捕获中断 在中断回调函数中换相
  HAL_TIMEx_HallSensor_Stop(&htimx_HALL); 
}

/******************************************************
  * 函数功能: BLDC控制函数
  * 输入参数: Dir :电机方向, Duty:PWM占空比
  * 返 回 值: 无
  * 说    明: 控制BLDCMotor的速度和方向
 *****************************************************/
void BLDC_Ctrl(int32_t Dir,float PWM_Duty)
{
  BLDCMotor.Dir = Dir;// 方向
  BLDCMotor.PWM_Duty = PWM_Duty; //占空比
}
/**
  * 函数功能: 获取霍尔传感器引脚状态
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
uint32_t HallSensor_GetPinState(void)
{
  __IO static uint32_t State ;
  State  = 0;
  if(HAL_GPIO_ReadPin(HALL_TIM_CH1_GPIO,HALL_TIM_CH1_PIN) != GPIO_PIN_RESET)  //霍尔传感器状态获取
  {
    State |= 0x01U;
  }
  if(HAL_GPIO_ReadPin(HALL_TIM_CH2_GPIO,HALL_TIM_CH2_PIN) != GPIO_PIN_RESET)  //霍尔传感器状态获取
  {
    State |= 0x02U;
  }
  if(HAL_GPIO_ReadPin(HALL_TIM_CH3_GPIO,HALL_TIM_CH3_PIN) != GPIO_PIN_RESET)  //霍尔传感器状态获取
  {
    State |= 0x04U;
  }
  return State;
}
/**
  * 函数功能：霍尔传感器触发换相
  * 输入参数：无
  * 返 回 值：无
  * 说    明：无
  */
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* 获取霍尔传感器引脚状态,作为换相的依据 */
  BLDCMotor.uwStep = HallSensor_GetPinState();
   __IO uint32_t tmp = 0;
  uwStep = HallSensor_GetPinState();
  if(Dir == CW)
  {
    uwStep = (uint32_t)7 - uwStep;        // 根据顺序表的规律 CW = 7 - CCW;
  }
#if 0
  if( PWM_ChangeFlag == 1)
  {
    if(BLDCMotor.Dir == CW)
    {      
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // 根据顺序表的规律 CW = 7 - CCW;
    }
  }
  else 
  {
    if(BLDCMotor.Dir == CW)
    {
      BLDCMotor.Hall_PulNum ++;                                           // 脉冲数累加
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // 根据顺序表的规律 CW = 7 - CCW;
    }
    else
      BLDCMotor.Hall_PulNum --;                                           // 脉冲数累减
    
  }
  
#endif   
  /*---- 定义定时器OC1为A(U)相 OC2为B(V)相，OC3为C(W)相 ---- */
  /*---- 定义uWStep低3位为霍尔传感器引脚状态,IC1(001,U),IC2(010,V),IC3(100,W) ----*/
  switch(uwStep)//switch(BLDCMotor.uwStep)
  {
    case 1://C+ A-
      /*  Channe2 configuration  */ 
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3); 

      /*  Channel configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
      break;
    
    case 2: //A+  B-
      /*  Channe3 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    
      /*  Channel configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
      
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
      break;
    
    case 3:// C+ B-
      /*  Channel configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
 
      /*  Channe3 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
  
      /*  Channe2 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_2);
      break;
    
    case 4:// B+ C-
      /*  Channel configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);

      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,BLDCMOTOR_TIM_PERIOD +1);    
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
      break;
    
    case 5: // B+ A-
      /*  Channe3 configuration */       
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_2,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_2);
      
      /*  Channel configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
      break;
    
    case 6: // A+ C-
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
      /*  Channel configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,BLDCMotor.PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1); 
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,BLDCMOTOR_TIM_PERIOD +1);  
      HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
      break;
  }
  /* 立刻触发换相 */
  HAL_TIM_GenerateEvent(&htimx_BLDC, TIM_EVENTSOURCE_COM);
  __HAL_TIM_CLEAR_IT(htim, TIM_FLAG_COM);
  BLDCMotor.Lock_Time = 0;
}


