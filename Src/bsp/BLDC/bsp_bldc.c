
#include "bldc/bsp_bldc.h" 
#include "main.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define PWM_DUTY          0.55f
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef      htimx_BLDC;

/* ��ʱ��PWM���ͨ���ṹ������ */
TIM_OC_InitTypeDef     sPWMConfig1, sPWMConfig2, sPWMConfig3;

/* Step Index */
 __IO uint32_t uwStep = 0;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʱ��Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����BLDCMOTOR_TIMx_Init��������
  */
void BLDCMOTOR_TIM_MspPostInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
  BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
  
  
  /* ��ʱ��ͨ��1��������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
  /* ��ʱ��ͨ��1����ͨ����������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH1N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1N_PORT, &GPIO_InitStruct);

  /* ��ʱ��ͨ��2��������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
  /* ��ʱ��ͨ��2����ͨ����������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH2N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2N_PORT, &GPIO_InitStruct);
  
  /* ��ʱ��ͨ��3��������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
  /* ��ʱ��ͨ��3����ͨ����������IO��ʼ�� */
  GPIO_InitStruct.Pin = BLDCMOTOR_TIM_CH3N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* ��ʱ���ƶ���������IO��ʼ�� */
  GPIO_InitStruct.Pin = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Alternate = GPIOx_AFx_TIMx;
  HAL_GPIO_Init(BLDCMOTOR_TIM_BKIN_PORT, &GPIO_InitStruct);
  
}

/*********************************************
  *
  * ��������: ������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  /* ������ʱ������ʱ��ʹ�� */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  BLDCMOTOR_TIM_MspPostInit();
  
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
  
  /* ������ɲ������ */
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 10;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BLDC, &sBreakDeadTimeConfig);

  /* ����Ƚ�ͨ�����ó�PWMģʽ */
  sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
  sPWMConfig1.Pulse = (uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY);
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
  
  
  /* ���û��ഥ����������������� */
  HAL_TIMEx_ConfigCommutationEvent_IT(&htimx_BLDC, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);
  
  /* ���ö�ʱ���ж����ȼ���ʹ�� */
  HAL_NVIC_SetPriority(BLDC_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(BLDC_TIM_IRQn);
  
  /* �ض���� */
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);

}
/**
  * �������ܣ���ʱ�������жϻص����� 
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
   /* ���嶨ʱ�����ͨ��1ΪA�� ���ͨ��2ΪB�࣬���ͨ��3ΪC�� */
  if (uwStep == 0)//  A+ C-
  {
    /*  �ض�B�� */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_2);
    
    /* ��һ��: A������ΪPWMģʽ1  */
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_1,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_1);
    
    /*  C������ΪPWMģʽ2������ͬռ�ձ�ʱ��AC�๲ͬ���˫����PWM���� */
    sPWMConfig3.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htimx_BLDC, &sPWMConfig3, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htimx_BLDC,TIM_CHANNEL_3,(uint16_t)(BLDCMOTOR_TIM_PERIOD * PWM_DUTY));
    HAL_TIM_PWM_Start(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htimx_BLDC, TIM_CHANNEL_3);
    
  }

  if (uwStep == 1)//  A+ B-
  {
    /* �ض�C�� */
    HAL_TIM_PWM_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htimx_BLDC, TIM_CHANNEL_3);
    
    /* ��һ��: */
    /* ����һ������ͬ������ */  
    
    /*  A������ΪPWMģʽ1��B������ΪPWMģʽ2�����˫����PWM����*/
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
    /* �ض�A�� */
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
    /* �ض�B�� */
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
    /* �ض�C�� */
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
    /* �ض�A�� */
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
    /* �ض�B�� */
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

