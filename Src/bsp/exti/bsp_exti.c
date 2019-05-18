#include "exti/bsp_exti.h"

#include "led/bsp_led.h"
#include "key/bsp_key.h"
#include "bldc/bsp_bldc.h" 


uint8_t IS_EnableMotor =0;
uint8_t start_flag =0 ;
uint8_t Global_sw = 0 ; 


/****************************************************************
  *
  *�ⲿ�жϳ�ʼ��
  *
  *
  *
*****************************************************************/
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();               //����GPIOAʱ��
  //  __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
  //  __HAL_RCC_GPIOH_CLK_ENABLE();               //����GPIOHʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_6;                //PE6
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
 #if 0
    GPIO_Initure.Pin=GPIO_PIN_13;               //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;				//����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;     //PH2,3  �½��ش���������
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
#endif 
    //�ж���0
    //HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //�ж���2 ����ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    //HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0 ���ж���2�� 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    
    //�ж���2
    //HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    //HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
    
    //�ж���3
    //HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    //HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //ʹ���ж���2
    
    //�ж���6
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //ʹ���ж���6
    
    //�ж���13
   // HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);   //��ռ���ȼ�Ϊ2�������ȼ�Ϊ3
   // HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //ʹ���ж���13  
}

/***************************************************
  *
  *�жϷ�����
  *
  *
  *
  *
****************************************************/

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //�����жϴ����ú���
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //�����жϴ����ú���
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //�����жϴ����ú���
}

void EXTI9_5_IRQHandler(void) 
{

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);   //�����жϴ����ú���

}
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //�����жϴ����ú���
}

/**********************************************************
    *
    //�жϷ����������Ҫ��������
    //��HAL�������е��ⲿ�жϷ�����������ô˺���
    //GPIO_Pin:�ж����ź�
    *
**********************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   // static uint8_t i=0; 
    HAL_Delay(10);     //����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_6:
            if(GLOBAL == 1 )	//����LED0,LED1�������
            {
               
                
                  
                    LED2_OFF;
                    HAL_Delay(200);
                    LED2_ON;
                    start_flag = 0;
                    Disable_BLDC();
                    IS_EnableMotor = 0;  
                   
                    Global_sw = 1; 
                                  
            }
            break;
      
    }
}
