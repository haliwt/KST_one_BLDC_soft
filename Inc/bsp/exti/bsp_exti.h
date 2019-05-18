#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H
//#include "stm32f4xx.h"
//#include "core_cm4.h"
#include "stm32f4xx_hal.h"

 

extern uint8_t IS_EnableMotor;
extern   uint8_t start_flag ;
extern   uint8_t Global_sw ;    




 
 

void EXTI_Init(void); //外部中断初始化

#endif
