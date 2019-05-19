#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define START              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)  //KEY0����PE4
#define BRAKE              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)  //KEY1����PE3
#define DIR                HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) 	//���򰴼�
#define STOP               HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //��ͣ����
#define WIPE               HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)  //WIPE WASH �����
#define AIR                HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)  //air conditioner  �յ�
#define CAR_DOOR           HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  //���ſ���
#define PUMP               HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)  //������
#define TURN_LIGHT         HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)  //ת���
#define HALL_SW            HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9)   //�����ź��л����л����޻���
#define GLOBAL             HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)   //ȫ�ֿ�� PE6
#define SPEED_ADD          HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)   // speed add
#define SPEED_REDUCE       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)   //speed reduce


#define START_PRES 	             1     //��������
#define BRAKE_PRES	             2    //ɲ������
#define DIR_PRES	             3    //���򰴼�
#define STOP_PRES                4    //��ͣ����
#define WIPE_PRES                5    //��ˢ����
#define AIR_PRES                 6    //�յ�����
#define CAR_DOOR_PRES            7    //����
#define PUMP_PRES                8    //������
#define TURN_LIGHT_PRES          9    //ת���
#define HALL_SW_PRES             0X0A   //�����л�������û�л���sensor��
//#define GLOBAL_PRES              0xb    //ȫ�ֿ���
#define SPEED_ADD_PRES           0xc    //speed add 
#define SPEED_REDUCE_PRES        0xd    //speed reduce


 

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
