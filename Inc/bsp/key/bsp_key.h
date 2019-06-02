#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//下面的方式是通过直接操作HAL库函数方式读取IO
#define START              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)  //KEY0按键PE4
#define BRAKE              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)  //KEY1按键PE3
#define DIR                HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) 	//方向按键
#define STOP               HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //启停按键
#define WIPE               HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)  //WIPE WASH 雨刮器
#define AIR                HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_1)  //air conditioner  空调
#define CAR_DOOR           HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  //车门控制
#define PUMP               HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)  //打气泵
#define TURN_LIGHT         HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)  //转向灯
#define HALL_SW            HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9)   //霍尔信号切换，切换成无霍尔
#define GLOBAL             HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)   //全局开� PE6
#define SPEED_ADD          HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)   // speed add
#define SPEED_REDUCE       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)   //speed reduce


#define START_PRES 	             1     //启动按键
#define BRAKE_PRES	             2    //刹车按键
#define DIR_PRES	             3    //方向按键
#define STOP_PRES                4    //启停按键
#define WIPE_PRES                5    //雨刷开关
#define AIR_PRES                 6    //空调开关
#define CAR_DOOR_PRES            7    //车门
#define PUMP_PRES                8    //打气泵
#define TURN_LIGHT_PRES          9    //转向灯
#define HALL_SW_PRES             0X0A   //霍尔切换，切入没有霍尔sensor。
//#define GLOBAL_PRES              0xb    //全局开关
#define SPEED_ADD_PRES           0xc    //speed add 
#define SPEED_REDUCE_PRES        0xd    //speed reduce


 

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
