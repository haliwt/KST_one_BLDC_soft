
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bldc/bsp_bldc.h" 
#include "main.h"
#include "led/bsp_led.h"

/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef      htimx_BLDC;
TIM_HandleTypeDef      htimx_HALL;
/* ��ʱ��PWM���ͨ���ṹ������ */
TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;
BLDC_Typedef BLDCMotor = {0,CW,0,0,100,0,0,0};
__IO int32_t uwStep = 0;      // 6������״̬
__IO int32_t Dir = CCW;       // ��ת����
__IO uint32_t Lock_Time = 0;  // ��תʱ��
/* ��չ���� ------------------------------------------------------------------*/

 /* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʱ��Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú����ɿ⺯������
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance == BLDCMOTOR_TIMx)
  {
    /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
    BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
    /* ��ʱ��ͨ��1����ͨ����������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH1N_PORT, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��2��������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
    /* ��ʱ��ͨ��2����ͨ����������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH2N_PORT, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��3��������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
    /* ��ʱ��ͨ��3����ͨ����������IO��ʼ�� */
    GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3N_PIN;
    HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
    
    /* ��ʱ���ƶ���������IO��ʼ�� */
    GPIO_InitStruct.Pin = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(BLDCMOTOR_TIM_BKIN_PORT, &GPIO_InitStruct);
  }
}

/**
  * ��������: ������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  
  /* ������ʱ������ʱ��ʹ�� */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  
  /* ������ʱ���������� */
  htimx_BLDC.Instance = BLDCMOTOR_TIMx;
  htimx_BLDC.Init.Prescaler = BLDCMOTOR_TIM_PRESCALER;
  htimx_BLDC.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_BLDC.Init.Period = BLDCMOTOR_TIM_PERIOD;
  htimx_BLDC.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  htimx_BLDC.Init.RepetitionCounter = BLDCMOTOR_TIM_REPETITIONCOUNTER;
  HAL_TIM_PWM_Init(&htimx_BLDC);
  
  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimx_BLDC, &sClockSourceConfig);

  /* ����Ƚ�ͨ�����ó�PWMģʽ */
  sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
  sPWMConfig1.Pulse = (uint16_t)(BLDCMOTOR_TIM_PERIOD * BLDCMotor.PWM_Duty);
  sPWMConfig1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sPWMConfig1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sPWMConfig1.OCFastMode = TIM_OCFAST_DISABLE;
  sPWMConfig1.OCIdleState = TIM_OCIDLESTATE_SET;
  sPWMConfig1.OCNIdleState = TIM_OCNIDLESTATE_SET;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);

  /* ��ʱ��CH2���� */
  sPWMConfig2 = sPWMConfig1;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig2, TIM_CHANNEL_2);

  /* ��ʱ��CH3���� */
  sPWMConfig3 = sPWMConfig1;
  HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);

  /* ���ö�ʱ�������¼�����Դ */
  HAL_TIMEx_ConfigCommutationEvent(&htimx_BLDC, TIM_TS_ITR3, TIM_COMMUTATION_SOFTWARE);
  
  /* �ض���� */
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
}

/**
  * ��������: ��ʱ�������������ӿ�Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú����ɿ⺯������
  */
void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance == HALL_TIMx)
  {
    HALL_TIM_GPIO_CLK_ENABLE();
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = HALL_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = HALL_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH2_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = HALL_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(HALL_TIM_CH3_GPIO, &GPIO_InitStruct);
  }
}
/**
  * ��������: ��ʱ�������������ӿڳ�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HALL_TIMx_Init(void)
{
  TIM_HallSensor_InitTypeDef  sHallSensorConfig;  
  
  /* ������ʱ������ʱ��ʹ�� */
  HALL_TIM_RCC_CLK_ENABLE();
  
  /* ��ʱ�������������� */
  htimx_HALL.Instance = HALL_TIMx;
  htimx_HALL.Init.Prescaler = HALL_TIM_PRESCALER;       // Ԥ��Ƶ
  htimx_HALL.Init.CounterMode = TIM_COUNTERMODE_UP;     // ���ϼ���
  htimx_HALL.Init.Period = HALL_TIM_PERIOD;             // ��������
  htimx_HALL.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; // ʱ�ӷ�Ƶ
  
  sHallSensorConfig.IC1Prescaler=TIM_ICPSC_DIV1;        // ���벶���Ƶ
  sHallSensorConfig.IC1Polarity=TIM_ICPOLARITY_BOTHEDGE;// ���벶����
  sHallSensorConfig.IC1Filter= 10;                      // �����˲�
  sHallSensorConfig.Commutation_Delay = 0U;             // 0 ,��ʹ���ӳٴ���
  HAL_TIMEx_HallSensor_Init(&htimx_HALL,&sHallSensorConfig);
  
  HAL_NVIC_SetPriority(HALL_TIM_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);
}

/**
  * ��������: ʹ����ˢ���ת��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void Enable_BLDC(void )
{ 
  /* ʹ�ܻ����������ӿ� �� PWM������� */
  __HAL_TIM_ENABLE_IT(&htimx_HALL, TIM_IT_TRIGGER);      
  HAL_TIM_TriggerCallback(&htimx_HALL); 
  HAL_TIMEx_HallSensor_Start(&htimx_HALL); 
}

/**
  * ��������: ֹͣ���ת��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void Disable_BLDC(void)
{
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  
  __HAL_TIM_DISABLE_IT(&htimx_HALL, TIM_IT_TRIGGER);  // �����ж� ���жϻص������л���
  HAL_TIMEx_HallSensor_Stop(&htimx_HALL); 
}

/******************************************************
  * ��������: BLDC���ƺ���
  * �������: Dir :�������, Duty:PWMռ�ձ�
  * �� �� ֵ: ��
  * ˵    ��: ����BLDCMotor���ٶȺͷ���
 *****************************************************/
void BLDC_Ctrl(int32_t Dir,float PWM_Duty)
{
  BLDCMotor.Dir = Dir;// ����
  BLDCMotor.PWM_Duty = PWM_Duty; //ռ�ձ�
}
/**
  * ��������: ��ȡ��������������״̬
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
uint32_t HallSensor_GetPinState(void)
{
  __IO static uint32_t State ;
  State  = 0;
  if(HAL_GPIO_ReadPin(HALL_TIM_CH1_GPIO,HALL_TIM_CH1_PIN) != GPIO_PIN_RESET)  //����������״̬��ȡ
  {
    State |= 0x01U;
  }
  if(HAL_GPIO_ReadPin(HALL_TIM_CH2_GPIO,HALL_TIM_CH2_PIN) != GPIO_PIN_RESET)  //����������״̬��ȡ
  {
    State |= 0x02U;
  }
  if(HAL_GPIO_ReadPin(HALL_TIM_CH3_GPIO,HALL_TIM_CH3_PIN) != GPIO_PIN_RESET)  //����������״̬��ȡ
  {
    State |= 0x04U;
  }
  return State;
}
/**
  * �������ܣ�������������������
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* ��ȡ��������������״̬,��Ϊ��������� */
  BLDCMotor.uwStep = HallSensor_GetPinState();
   __IO uint32_t tmp = 0;
  uwStep = HallSensor_GetPinState();
  if(Dir == CW)
  {
    uwStep = (uint32_t)7 - uwStep;        // ����˳���Ĺ��� CW = 7 - CCW;
  }
#if 0
  if( PWM_ChangeFlag == 1)
  {
    if(BLDCMotor.Dir == CW)
    {      
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // ����˳���Ĺ��� CW = 7 - CCW;
    }
  }
  else 
  {
    if(BLDCMotor.Dir == CW)
    {
      BLDCMotor.Hall_PulNum ++;                                           // �������ۼ�
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // ����˳���Ĺ��� CW = 7 - CCW;
    }
    else
      BLDCMotor.Hall_PulNum --;                                           // �������ۼ�
    
  }
  
#endif   
  /*---- ���嶨ʱ��OC1ΪA(U)�� OC2ΪB(V)�࣬OC3ΪC(W)�� ---- */
  /*---- ����uWStep��3λΪ��������������״̬,IC1(001,U),IC2(010,V),IC3(100,W) ----*/
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
  /* ���̴������� */
  HAL_TIM_GenerateEvent(&htimx_BLDC, TIM_EVENTSOURCE_COM);
  __HAL_TIM_CLEAR_IT(htim, TIM_FLAG_COM);
  BLDCMotor.Lock_Time = 0;
}


