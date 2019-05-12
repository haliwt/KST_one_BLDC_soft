

/* 包含头文件 ----------------------------------------------------------------*/
#include "key/bsp_key.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 板载按键IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_key.h
  *           文件相关宏定义就可以方便修改引脚。
  */
//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();     //开启GPIOE时钟
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;            //PC4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_12
		              |GPIO_PIN_14; //PB0 1
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_3;            //PA 4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_8;            //PC4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
    
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(START==0||BRAKE==0||KEY2==0
        ||KEY3==0||WIPE==0||AIR==0)||CAR_DOOR==0
        ||PUMP==0||TURN_LIGHT==0||HALL_SW==0||GLOBAL==0)
    {
        HAL_Delay(10);
        key_up=0;
        if(START==0)              return  START_PRES;
        else if(BRAKE==0)         return  BRAKE_PRES;
        else if(KEY2==0)          return  KEY2_PRES;
        else if(KEY3==0)          return  KEY3_PRES;  
        else if(WIPE== 0)         return  WIPE_PRES;
        else if(AIR== 0)          return  AIR_PRES;
		else if(CAR_DOOR == 0)    return  CAR_DOOR_PRES;
		else if(PUMP ==0 )        return  PUMP_PRES;
		else if(TURN_LIGHT ==0)   return  TURN_LIGHT_PRES;
		else if(HALL_SW==0)       return  HALL_SW_PRES ;
		else if(GLOBAL==0)        return  GLOBAL_PRES;
    }else if(START==1&&BRAKE==1&&KEY2==1&&KEY3==1&&WIPE==1&&AIR==1&&CAR_DOOR==1
             &&PUMP==1 && TURN_LIGHT==1 && HALL_SW==1 && GLOBAL == 1)key_up=1;
    return 0;   //无按键按下
}
