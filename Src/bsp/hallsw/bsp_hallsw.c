#include "hallsw/bsp_hallsw.h"


void NO_HALL_Init(void)
{
     GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIOAʱ��
    
    
    GPIO_Initure.Pin=GPIO_PIN_12;            //PD12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);


}
    
