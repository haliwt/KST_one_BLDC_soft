#include "hallsw/bsp_hallsw.h"

#if 0
void HALL_JUDGE_Init(void)
{
     /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能(开启)LED引脚对应IO端口时钟 */  
  HALL_FAIL_RCC_CLK_ENABLE();
 
  /* 配置hall fail 引脚输出电压 */
  HAL_GPIO_WritePin(HALL_FAIL_GPIO, HALL_FAIL_GPIO_PIN, GPIO_PIN_SET);
  
   
  /* 设定hall fail 对应引脚IO编号 */
  GPIO_InitStruct.Pin = HALL_FAIL_GPIO_PIN;
  /* 设定LED1对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED1对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* 初始化LED1对应引脚IO */
  HAL_GPIO_Init(HALL_FAIL_GPIO, &GPIO_InitStruct);


}

#endif     
