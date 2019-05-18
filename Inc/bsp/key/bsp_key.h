#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* °üº¬Í·ÎÄ¼þ ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//ÏÂÃæµÄ·½Ê½ÊÇÍ¨¹ýÖ±½Ó²Ù×÷HAL¿âº¯Êý·½Ê½¶ÁÈ¡IO
#define START              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)  //KEY0°´¼üPE4
#define BRAKE              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)  //KEY1°´¼üPE3
#define KEY2               HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) 	//KEY2°´¼üPE2
#define KEY3               HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //WKUP°´¼üPA0
#define WIPE               HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)  //WIPE WASH Óê¹ÎÆ÷
#define AIR                HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)  //air conditioner  ¿Õµ÷
#define CAR_DOOR           HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  //³µÃÅ¿ØÖÆ
#define PUMP               HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)  //´òÆø±Ã
#define TURN_LIGHT         HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)  //×ªÏòµÆ
#define HALL_SW            HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9)   //»ô¶ûÐÅºÅÇÐ»»£¬ÇÐ»»³ÉÎÞ»ô¶û
#define GLOBAL             HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)   //È«¾Ö¿ª¹   Ø 
#define SPEED_ADD          HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)   // speed add
#define SPEED_REDUCE       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)   //speed reduce


#define START_PRES 	             1     //Æô¶¯°´¼ü
#define BRAKE_PRES	             2    //É²³µ°´¼ü
#define KEY2_PRES	             3    //
#define KEY3_PRES                4
#define WIPE_PRES                5    //ÓêË¢¿ª¹Ø
#define AIR_PRES                 6    //¿Õµ÷¿ª¹Ø
#define CAR_DOOR_PRES            7    //³µÃÅ
#define PUMP_PRES                8    //´òÆø±Ã
#define TURN_LIGHT_PRES          9    //×ªÏòµÆ
#define HALL_SW_PRES             0X0A   //»ô¶ûÇÐ»»£¬ÇÐÈëÃ»ÓÐ»ô¶ûsensor¡£
#define GLOBAL_PRES              0xb    //È«¾Ö¿ª¹Ø
#define SPEED_ADD_PRES           0xc    //speed add 
#define SPEED_REDUCE_PRES        0xd    //speed reduce


 

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
