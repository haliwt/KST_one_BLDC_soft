/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-3-30
  * ��    ��: ���ض��������ײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "key/bsp_key.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���ذ���IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_key.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void KEY_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)KEY���Ŷ�ӦIO�˿�ʱ�� */  
  KEY1_RCC_CLK_ENABLE();
  KEY2_RCC_CLK_ENABLE(); 
  KEY3_RCC_CLK_ENABLE();
  KEY4_RCC_CLK_ENABLE();	
  KEY5_RCC_CLK_ENABLE(); 
  
  /* ����KEY1 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO, &GPIO_InitStruct);  
  
  /* ����KEY1 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY2_GPIO, &GPIO_InitStruct);  

  /* ����KEY1 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = KEY3_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY3_GPIO, &GPIO_InitStruct);
  
  /* ����KEY1 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = KEY4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY4_GPIO, &GPIO_InitStruct); 
  
  /* ����KEY1 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = KEY5_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY5_GPIO, &GPIO_InitStruct);  
  
}

/**
  * ��������: ����1״̬��ȡ����.
  * �������: ��
  * �� �� ֵ: ����������ڱ�����״̬�ĵ�ƽ������KEY_ON��
  *           ����������ڵ���״̬�ĵ�ƽ������KEY_OFF��
  * ˵    ������
  */
uint8_t GetPinStateOfKey1(void)
{
	if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWNSTATE)
	{
			return KEY_ON;
	}
	else
	{
			return KEY_OFF;
	}
}

/**
  * ��������: ����2״̬��ȡ����.
  * �������: ��
  * �� �� ֵ: ����������ڱ�����״̬�ĵ�ƽ������KEY_ON��
  *           ����������ڵ���״̬�ĵ�ƽ������KEY_OFF��
  * ˵    ������
  */
uint8_t GetPinStateOfKey2(void)
{
	if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWNSTATE)
	{
			return KEY_ON;
	}
	else
	{
			return KEY_OFF;
	}
}

/**
  * ��������: ����2״̬��ȡ����.
  * �������: ��
  * �� �� ֵ: ����������ڱ�����״̬�ĵ�ƽ������KEY_ON��
  *           ����������ڵ���״̬�ĵ�ƽ������KEY_OFF��
  * ˵    ������
  */
uint8_t GetPinStateOfKey3(void)
{
	if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWNSTATE)
	{
			return KEY_ON;
	}
	else
	{
			return KEY_OFF;
	}
}

/**
  * ��������: ����2״̬��ȡ����.
  * �������: ��
  * �� �� ֵ: ����������ڱ�����״̬�ĵ�ƽ������KEY_ON��
  *           ����������ڵ���״̬�ĵ�ƽ������KEY_OFF��
  * ˵    ������
  */
uint8_t GetPinStateOfKey4(void)
{
	if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWNSTATE)
	{
			return KEY_ON;
	}
	else
	{
			return KEY_OFF;
	}
}

/**
  * ��������: ����2״̬��ȡ����.
  * �������: ��
  * �� �� ֵ: ����������ڱ�����״̬�ĵ�ƽ������KEY_ON��
  *           ����������ڵ���״̬�ĵ�ƽ������KEY_OFF��
  * ˵    ������
  */
uint8_t GetPinStateOfKey5(void)
{
	if(HAL_GPIO_ReadPin(KEY5_GPIO,KEY5_GPIO_PIN)==KEY5_DOWNSTATE)
	{
			return KEY_ON;
	}
	else
	{
			return KEY_OFF;
	}
}



/***************************************�û��Զ��庯������***********************************/



/*
************************************************************************************************************************
*                              ������ʱ����
*
* �����������û����û�ж�ʱ���ã���û�ж���KEY_FIXED_PERIOD������Ҫ���¸���CPU��д�˺�����
*
*     ������xms        ���ݲ�ͬ����������ʱ�䣬һ����20ms
*
*   ����ֵ����
*
*     ע�⣺������ʱ
*
************************************************************************************************************************
*/
#ifndef  KEY_FIXED_PERIOD
void key_delay1ms(uint8_t xms)
{
    
}
#endif


/*					�������ݲ���Ҫ���ģ�ֱ��ʹ�ü���								*/
/*
************************************************************************************************************************
*                              ������������
*
* ����������ʹ�ð���ǰ��Ҫ����������
*
*     ������p_Key        ָ�򰴼����Ʊ�����ָ��
*           p_CallBack   ������ȡ������ƽ�Ļص�����ָ�룬��Ϊ��ͬCPU��ƽ��ȡ�ķ�ʽ��һ�����ⲿ����Ҫ�û��Լ���д
*                        α�������£�
*
*                          uint8_t GetPinStateOfKeyXX(void)
*                          {
*                            ����ܽŵĵ�ƽ�ǰ�������ʱ�ĵ�ƽ
*                            ����1
*
*                            ����ܽŵĵ�ƽ�ǰ�������ʱ�ĵ�ƽ
*                            ����0
*                          }
*
*   ����ֵ������������ڱ�����״̬�ĵ�ƽ������1��
*           ����������ڵ���״̬�ĵ�ƽ������0��
*
************************************************************************************************************************
*/
void KeyCreate(KEY *p_Key,KEY_CALLBACK_PTR p_CallBack)
{
  p_Key->GetStatePtr=p_CallBack;
  p_Key->Times=0;
  p_Key->State=KEY_UP;
  p_Key->StateChange=NOT_CHANGED;
#ifdef KEY_FIXED_PERIOD
	p_Key->Time_ms=0;          //����ʱ���ʼ��Ϊ0
#endif
  
#if USER_DATA_EN>0             
	p_Key->User_Data=0;		           //�û������������û�����ʹ��
#endif
  
}

/*
************************************************************************************************************************
*                              ����״̬���º���
*
* ������������������������ֻ�ʺ�STM32��Ƭ���⿪�����������͵ĵ�Ƭ�����������޸ġ�
*
*     ������p_Key        ָ�򰴼����Ʊ�����ָ��
*
*   ����ֵ����
*
*     ע�⣺����Ƶ����Ҫ����20Hz
*
************************************************************************************************************************
*/
void Key_RefreshState(KEY* p_Key)
{
  switch(p_Key->State)
  {
    case KEY_UP:
    {
      if((*(p_Key->GetStatePtr))())//ִ�лص������жϰ����ܽ�״̬
      {
#ifdef  KEY_FIXED_PERIOD   
        p_Key->Time_ms = 0;
        p_Key->State = KEY_DOWN_WOBBLE;//����������ʱ
#else
        p_Key->State = KEY_DOWN_WOBBLE;
        key_delay1ms(KEY_WOBBLE_TIME);				
        if((*(p_Key->GetStatePtr))())
        {
          p_Key->StateChange=CHANGED;
          p_Key->State = KEY_DOWN;
        }
#endif
      }
    }break;
    
    #ifdef  KEY_FIXED_PERIOD
    case KEY_DOWN_WOBBLE:
    {
      p_Key->Time_ms += KEY_FIXED_PERIOD;
      if( p_Key->Time_ms >=KEY_WOBBLE_TIME )
      {
        if((*(p_Key->GetStatePtr))())
        {
          p_Key->StateChange=CHANGED;
          p_Key->State = KEY_DOWN;
        }
        else
        {
          p_Key->State = KEY_UP;
        }
      }
    }break;
    #endif

    case KEY_DOWN:
    {
      if( (*(p_Key->GetStatePtr))() == 0 )
      {
#ifdef  KEY_FIXED_PERIOD
        p_Key->Time_ms = 0;
        p_Key->State = KEY_UP_WOBBLE;//����������ʱ
#else
        key_delay1ms(KEY_WOBBLE_TIME);
        if( (*(p_Key->GetStatePtr))() == 0 )
        {
          p_Key->StateChange=CHANGED;
          p_Key->State = KEY_UP;
          p_Key->Times++;
          if( p_Key->Times > 250)
            p_Key->Times = 250;//���������250��û����
        }
#endif
      }
    }break;

#ifdef  KEY_FIXED_PERIOD
    case KEY_UP_WOBBLE:
    {
      p_Key->Time_ms += KEY_FIXED_PERIOD;
      if( p_Key->Time_ms >= KEY_WOBBLE_TIME )
      {
        if( (*(p_Key->GetStatePtr))()==0)
        {
          p_Key->StateChange=CHANGED;
          p_Key->State = KEY_UP;
          p_Key->Times++;
          if( p_Key->Times > 250)
            p_Key->Times = 250;//���������250��û����
        }
        else
        {
          p_Key->State = KEY_DOWN;
        }
      }
    }break;
#endif
  }
}

/*
************************************************************************************************************************
*                              ���������´������ʺ���
*
* �����������԰��������´����ķ���
*
*     ������p_Key        ָ�򰴼����Ʊ�����ָ��
*           option	     ѡ����ڰ�������һ��ֻ�õ�һ�����ֲ��������������ľ�û�а�������
*                        KEY_ACCESS_READ         ��ȡ���������µĴ���
*                        KEY_ACCESS_WRITE_CLEAR  ���㰴�������µĴ���
*   ����ֵ��->times��ֵ��
*
*     ע�⣺���ʹ���û�����ź��������������ͬһ����������µ���Key_AccessTimes��Key_RefreshState
*           ��û��ʲô��ϵ�ģ�����ϵͳ������������������������о�Ҫ�����ٽ�α����ˣ�������������
*           һ�㶼Ҫ��һ��������ʹ�ã���Ϊÿ�ε���Key_RefreshState��Times���п��ܱ����£����ں���
*           ��ѯ���
*
************************************************************************************************************************
*/

uint8_t Key_AccessTimes(KEY* p_Key,ACCESS_TYPE opt)
{
	uint8_t times_temp;
	
	if((opt&KEY_ACCESS_WRITE_CLEAR) == KEY_ACCESS_WRITE_CLEAR)
	{
			p_Key->Times = 0;					
	}
		
	times_temp = p_Key->Times;
	
	return times_temp;
}

/*
************************************************************************************************************************
*                              ����״̬���ʺ���
*
* �����������԰���������״̬�ķ���
*
*     ������p_Key        ָ�򰴼����Ʊ�����ָ��

*   ����ֵ��->State��ֵ��
*
*     ע�⣺1.���ʹ���û�����ź��������������ͬһ����������µ���Key_AccessTimes��Key_RefreshState
*           ��û��ʲô��ϵ�ģ�����ϵͳ������������������������о�Ҫ�����ٽ�α����ˣ�������������
*           һ�㶼Ҫ��һ��������ʹ�ã���Ϊÿ�ε���Key_RefreshState��Times���п��ܱ����£����ں���
*           ��ѯ���
************************************************************************************************************************
*/
uint8_t Key_AccessState(KEY* p_Key,KEY_STATE *p_State)
{
  uint8_t StateChange=p_Key->StateChange;
  //��ȡ״̬
  *p_State = p_Key->State;
  //��ȡ�����״̬�Ƿ񱻸ı�
  p_Key->StateChange=NOT_CHANGED;
  //����״̬�Ƿ�ı�
  return StateChange;
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
