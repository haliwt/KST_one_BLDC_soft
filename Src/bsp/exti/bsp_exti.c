#include "exti/bsp_exti.h"

#include "led/bsp_led.h"
#include "key/bsp_key.h"
#include "bldc/bsp_bldc.h" 


uint8_t IS_EnableMotor =0;
uint8_t start_flag =0 ;
uint8_t Global_sw = 0 ; 


/****************************************************************
  *
  *外部中断初始化
  *
  *
  *
*****************************************************************/
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();               //开启GPIOA时钟
  //  __HAL_RCC_GPIOC_CLK_ENABLE();               //开启GPIOC时钟
  //  __HAL_RCC_GPIOH_CLK_ENABLE();               //开启GPIOH时钟
    
    GPIO_Initure.Pin=GPIO_PIN_6;                //PE6
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//下拉
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
 #if 0
    GPIO_Initure.Pin=GPIO_PIN_13;               //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;     //PH2,3  下降沿触发，上拉
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
#endif 
    //中断线0
    //HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //中断组2 ，抢占优先级为2，子优先级为0
    //HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0 ，中断组2， 2位抢占优先级，2位响应优先级
    
    //中断线2
    //HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //抢占优先级为2，子优先级为1
    //HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
    
    //中断线3
    //HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //抢占优先级为2，子优先级为2
    //HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
    
    //中断线6
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //使能中断线6
    
    //中断线13
   // HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);   //抢占优先级为2，子优先级为3
   // HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //使能中断线13  
}

/***************************************************
  *
  *中断服务函数
  *
  *
  *
  *
****************************************************/

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //调用中断处理公用函数
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //调用中断处理公用函数
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //调用中断处理公用函数
}

void EXTI9_5_IRQHandler(void) 
{

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);   //调用中断处理公用函数

}
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //调用中断处理公用函数
}

/**********************************************************
    *
    //中断服务程序中需要做的事情
    //在HAL库中所有的外部中断服务函数都会调用此函数
    //GPIO_Pin:中断引脚号
    *
**********************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   // static uint8_t i=0; 
    HAL_Delay(10);     //消抖
    switch(GPIO_Pin)
    {
        case GPIO_PIN_6:
            if(GLOBAL == 1 )	//控制LED0,LED1互斥点亮
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
