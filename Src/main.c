
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "led/bsp_led.h"
//#include "BasicTIM/bsp_BasicTIM.h" 
//#include "usart/bsp_debug_usart.h"
#include "key/bsp_key.h"
//#include "GeneralTIM/bsp_GeneralTIM.h"
#include "bldc/bsp_bldc.h"
#include "usart/bsp_usartx.h"
#include "adc/bsp_adc.h"
//#include "malloc.h"	

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		256  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define KEYPROCESS_TASK_PRIO 3
//任务堆栈大小	
#define KEYPROCESS_STK_SIZE  256 
//任务句柄
TaskHandle_t Keyprocess_Handler;
//任务函数
void Keyprocess_task(void *pvParameters);


//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define MESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
QueueHandle_t Key_Queue;   		//按键值消息队列句柄
QueueHandle_t Message_Queue;	//信息队列句柄




/* 私有宏定义 ----------------------------------------------------------------*/
#define SENDBUFF_SIZE              100  // 串口DMA发送缓冲区大小



uint32_t IS_EnableMotor = 0;  // 使能电机标志
uint32_t Time_CNT = 0;
int32_t  flag = 0;
int32_t  start_flag = 0;

/* 私有变量 ------------------------------------------------------------------*/
uint8_t DMAaRxBuffer[8];                      // 接收数据 
uint8_t DMAaTxBuffer[SENDBUFF_SIZE];       // 串口DMA发送缓冲区


// 用于保存转换计算后的电压值	 
float ADC_ConvertedValueLocal;
uint32_t ADC_ConvertedValue;




typedef struct Msg
{
	uint8_t  ucMessageID;
	uint16_t usData[8];
	uint32_t ulData[8];
}MSG_T;

MSG_T   g_tMsg; /* 定义一个结构体用于消息队列 */

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint16_t CCR1_Val;







/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     //使能PWR时钟

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  //设置调压器输出电压级别1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        //打开HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    //打开PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            //PLL时钟源选择HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 //8分频MHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               //336倍频
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     //2分频，得到168MHz主时钟
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 //USB/SDIO/随机数产生器等的主PLL分频系数
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：168MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟： 168MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;               // APB1时钟：42MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;               // APB2时钟：84MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_RCC_EnableCSS();                                            // 使能CSS功能，优先使用外部晶振，内部时钟源为备用
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{
  
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
  
   MX_USARTx_Init();
	
   /* 初始化按键配置 */
  KEY_Init();
	
  /* 初始化LED */
  LED_GPIO_Init();
  /* 板子按键初始化 */
 // KEY_GPIO_Init();
  /* 基本定时器初始化：100us中断一次 */
 BLDCMOTOR_TIMx_Init();
  /* 启动定时器 */
  HAL_TIM_Base_Start(&htimx_BLDC);
  /* ADC 初始化 */
   MX_DMA_Init();
  /* ADC 初始化 */
  MX_ADCx_Init();

/* 启动AD转换并使能DMA传输和中断 */
  HAL_ADC_Start_DMA(&hadcx,&ADC_ConvertedValue,1); 

	 //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
	
 
  
 
}

/********************************************************************
  *
  * 函数功能: 接口消息处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  *
 ********************************************************************/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建消息队列
    Key_Queue=xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));        //创建消息Key_Queue
    Message_Queue=xQueueCreate(MESSAGE_Q_NUM,USART_REC_LEN); //创建消息Message_Queue,队列项长度是串口接收缓冲区长度
	
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
    //创建TASK2任务
    xTaskCreate((TaskFunction_t )Keyprocess_task,     
                (const char*    )"keyprocess_task",   
                (uint16_t       )KEYPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEYPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Keyprocess_Handler);
    
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

/********************************************************
  *
  * 函数名称：
  * 函数功能: 输出队列的信息,xQueue2 ,结构体地址&ptMsg.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  *
*********************************************************/
void task1_task(void *pvParameters)
{
	uint8_t key,i=0;
    BaseType_t err;
	while(1)
	{
		key=KEY_Scan(0);            //扫描按键
        if((Key_Queue!=NULL)&&(key))   //消息队列Key_Queue创建成功,并且按键被按下
        {
            err=xQueueSend(Key_Queue,&key,10);
            if(err==errQUEUE_FULL)   //发送按键值
            {
                printf("队列Key_Queue已满，数据发送失败!\r\n");
            }
        }
        i++;
        if(i==50)
        {
            i=0;
            LED1_TOGGLE ;
        }
        vTaskDelay(10);                           //延时10ms，也就是10个时钟节拍	
	}
}

/***************************************************************
  *
  * 函数名称：
  * 函数功能: 输出队列1的数据，xQueue1,接收队列1的数据。
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  *
***************************************************************/
void Keyprocess_task(void *pvParameters)
{
	uint8_t key;
	while(1)
	{
        if(Key_Queue!=NULL)
        {
            if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))//请求消息Key_Queue
            {
                switch(key)
                {
                    case START_PRES:		//KEY_UP控制LED1
                          Enable_BLDC();
                          /* 先以恒定的转空比启动,然后再来调速 */
                         //BLDCMotor.PWM_Duty = (int32_t)(BLDCMOTOR_TIM_PERIOD*5/100);
                         //HAL_Delay(100);
                         start_flag  = 1;
                         IS_EnableMotor = 1;
                        LED1_TOGGLE;
                        break;
                    case BRAKE_PRES:		//KEY2控制蜂鸣器
                          start_flag = 0;
                         Disable_BLDC();
                         IS_EnableMotor = 0;  
                         LED2_ON;        
                         break;
                  
                   case KEY2_PRES : //方向键
                          Dir = -Dir;
                          LED1_ON;
             
                     break;
                   case KEY3_PRES :
                     ADC_ConvertedValueLocal =(double)ADC_ConvertedValue*3.3/4096; 	
                    HAL_Delay(10); 
                    printf("AD转换原始值 = 0x%04X \r\n", ADC_ConvertedValue); 
                    printf("计算得出电压值 = %f V \r\n",ADC_ConvertedValueLocal); 
                    
                  //  HAL_UART_Receive(&husartx,DMAaRxBuffer,8,0XFF);
                  //  HAL_UART_Transmit(&husartx,DMAaRxBuffer,8,0XFF);
            
                        LED1_OFF;
                        LED2_OFF;
                   break;
         case WIPE_PRES:
             
             LED2_OFF;
             HAL_Delay(500);
             LED2_ON;
         
             break;
         case AIR_PRES:
             LED1_OFF;;
             LED2_ON;;
             HAL_Delay(500);
             LED1_ON;;
             LED2_OFF;;
             break;
		 case CAR_DOOR_PRES:
             LED1_ON;
			 HAL_Delay(200);
             LED2_OFF;
             HAL_Delay(200);
             LED1_OFF;
			 HAL_Delay(200);
             LED2_ON; 
		     break;
         case PUMP_PRES:
		 	 LED1_ON;
             HAL_Delay(500);
             LED1_OFF;
             HAL_Delay(500);
			 LED1_ON;
			 break;
		  case TURN_LIGHT_PRES:
		  	  LED2_OFF;
              HAL_Delay(200);
              LED2_ON;
			  HAL_Delay(200);
			  LED2_OFF;
			  break;
          case GLOBAL_PRES :
		  	  LED1_OFF;
              LED2_OFF;
              HAL_Delay(200);
              LED1_ON;
              LED2_ON;
			  HAL_Delay(200);
			  LED1_OFF;
              LED2_OFF;
		  	  break;
                }
            }
        } 
		vTaskDelay(10);      //延时10ms，也就是10个时钟节拍	
	}
}

 


void HAL_SYSTICK_Callback()
{
  if(IS_EnableMotor)
  {
    BLDCMotor.Lock_Time++;//Lock_Time++;
    /* 超过100ms 则认为是堵转,停止PWM输出 */
    if(Lock_Time >= 100)  //100ms
    {
      Disable_BLDC();
      LED2_TOGGLE;
      BLDCMotor.Lock_Time = 0;//Lock_Time = 0;
    }
  }
}
