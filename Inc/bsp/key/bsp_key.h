#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� --------------------------------------------------------------*/
#define KEY_ON                        1
#define KEY_OFF                       0

#define KEY1_DOWNSTATE                0          /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */
#define KEY2_DOWNSTATE                0          /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */
#define KEY3_DOWNSTATE                0          /* ����ԭ��ͼ��ƣ�KEY3����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */
#define KEY4_DOWNSTATE                0          /* ����ԭ��ͼ��ƣ�KEY4����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */
#define KEY5_DOWNSTATE                0          /* ����ԭ��ͼ��ƣ�KEY5����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

/* �궨�� --------------------------------------------------------------------*/
#define KEY1_RCC_CLK_ENABLE           __HAL_RCC_GPIOB_CLK_ENABLE
#define KEY1_GPIO_PIN                 GPIO_PIN_1
#define KEY1_GPIO                     GPIOB

#define KEY2_RCC_CLK_ENABLE           __HAL_RCC_GPIOB_CLK_ENABLE
#define KEY2_GPIO_PIN                 GPIO_PIN_0
#define KEY2_GPIO                     GPIOB

#define KEY3_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define KEY3_GPIO_PIN                 GPIO_PIN_5
#define KEY3_GPIO                     GPIOC

#define KEY4_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define KEY4_GPIO_PIN                 GPIO_PIN_4
#define KEY4_GPIO                     GPIOC

#define KEY5_RCC_CLK_ENABLE           __HAL_RCC_GPIOA_CLK_ENABLE
#define KEY5_GPIO_PIN                 GPIO_PIN_6
#define KEY5_GPIO                     GPIOA

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
uint8_t GetPinStateOfKey1(void);
uint8_t GetPinStateOfKey2(void);
uint8_t GetPinStateOfKey3(void);
uint8_t GetPinStateOfKey4(void);
uint8_t GetPinStateOfKey5(void);


/* ------------------------�û������������� ----------------------------------*/

/*ʹ���û����ݣ�ÿһ��������һ���������Թ��û�����ʹ�ã��������
 *��������������ڸĳ�0���ɣ����Ҫ�þ͸ĳ�1����������˷�һ����
 *�ڵ��ڴ�
 */
#define USER_DATA_EN			0

#define KEY_WOBBLE_TIME		10      //��������ʱ�䡣Ҳ��������ʱ��,��λms
 
#define KEY_FIXED_PERIOD	10      //�̶�Ƶ�ʵ��ð���״̬���º���,������Ϊ�������ڣ����ڵ�λΪms

//����ص���������
typedef  uint8_t    (*KEY_CALLBACK_PTR)(void);

#define KEY_TIMES_MAX (0XFF)
typedef enum{
	KEY_ACCESS_READ = 0,   //Ĭ���Ƕ�ȡ
	KEY_ACCESS_WRITE_CLEAR = 0x01
}ACCESS_TYPE;

/************�������ݾ�����Ҫ���ģ�ֱ��ʹ�ü���*******************************/
typedef enum
{
	KEY_DOWN 				= 1,
	KEY_UP	 				= 2,
	KEY_UP_WOBBLE 	= 3,//ȷ�ϵ�������״̬
	KEY_DOWN_WOBBLE = 4 //ȷ�ϰ�������״̬
}KEY_STATE;

typedef enum
{
	CHANGED 				    = 1,
	NOT_CHANGED	 				= 2,
}KEY_STATE_CHAGE;

typedef struct
{
	KEY_CALLBACK_PTR    GetStatePtr;	//���ڻ�ȡ����״̬�ĺ��� 
	uint8_t     				Times;				//���²��������1��ʹ�ú���Ӧ�ó����1
  KEY_STATE		        State;
  KEY_STATE_CHAGE     StateChange;                      
#ifdef KEY_FIXED_PERIOD
	unsigned char				Time_ms;			//���ڹ̶����ڵ���״̬���º����ļ�ʱ
#endif
  
#if USER_DATA_EN>0             
	uint8_t				      User_Data;		//�û������������û�����ʹ��
#endif
  
}KEY;

/*			����������	*/
void KeyCreate(KEY *p_Key,KEY_CALLBACK_PTR p_CallBack);
void Key_RefreshState(KEY* theKey);
uint8_t Key_AccessTimes(KEY* p_Key,ACCESS_TYPE opt);
uint8_t Key_AccessState(KEY* p_Key,KEY_STATE *p_State);

#endif  // __BSP_KEY_H__


