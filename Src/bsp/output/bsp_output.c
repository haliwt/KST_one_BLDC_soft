#include "output/bsp_output.h"

void OUTPUT_SIG_GPIO_Init(void)
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能(开启)LED引脚对应IO端口时钟 */  
  AIR_RCC_CLK_ENABLE();
  WIPER_RCC_CLK_ENABLE();
  PLC_RCC_CLK_ENABLE();
  
  
  /* 配置空调引脚输出电压 */
  HAL_GPIO_WritePin(AIR_GPIO, AIR_GPIO_PIN, GPIO_PIN_RESET);
  /* 配置雨刷引脚输出电压 */
  HAL_GPIO_WritePin(WIPER_GPIO, WIPER_GPIO_PIN, GPIO_PIN_RESET);
  /* 配置PLC引脚输出电压 */
  HAL_GPIO_WritePin(PLC_GPIO, PLC_GPIO_PIN, GPIO_PIN_RESET);  
  
   /* 设定AIR空调输出对应引脚IO编号 */
  GPIO_InitStruct.Pin = AIR_GPIO_PIN;
  /* 设定LED1对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED1对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* 初始化LED1对应引脚IO */
  HAL_GPIO_Init(AIR_GPIO, &GPIO_InitStruct);
  
  /* 设定WIPER雨刷器对应引脚IO编号 */
  GPIO_InitStruct.Pin = WIPER_GPIO_PIN;
  /* 设定LED2对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED2对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* 初始化LED2对应引脚IO */
  HAL_GPIO_Init(WIPER_GPIO, &GPIO_InitStruct);
  
   /* 设定PLC 对应引脚*/
  GPIO_InitStruct.Pin = PLC_GPIO_PIN;
  /* 设定LED2对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED2对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* 初始化LED2对应引脚IO */
  HAL_GPIO_Init(PLC_GPIO, &GPIO_InitStruct);
  
 
  
}
