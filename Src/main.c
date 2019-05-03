
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "led/bsp_led.h"
//#include "BasicTIM/bsp_BasicTIM.h" 
#include "usart/bsp_debug_usart.h"
#include "key/bsp_key.h"
//#include "GeneralTIM/bsp_GeneralTIM.h"
#include "bldc/bsp_bldc.h"
#include "dma_usart3/bsp_usart3.h"
#include "adc/bsp_adc.h"



/* 私有宏定义 ----------------------------------------------------------------*/
#define SENDBUFF_SIZE              100  // 串口DMA发送缓冲区大小

/* 私有类型定义 --------------------------------------------------------------*/
//__IO int32_t  Dir;
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
//__IO int32_t  uwStep ;

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


static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED1 = NULL;
static TaskHandle_t xHandleTaskLED2 = NULL;
static TaskHandle_t xHandleTaskLED3 = NULL;
static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;

typedef struct Msg
{
	uint8_t  ucMessageID;
	uint16_t usData[8];
	uint32_t ulData[8];
}MSG_T;

MSG_T   g_tMsg; /* 定义一个结构体用于消息队列 */

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint16_t CCR1_Val;

/* 私有函数原形 --------------------------------------------------------------*/
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED1(void *pvParameters);
static void vTaskLED2(void *pvParameters);
static void vTaskLED3(void *pvParameters);
static void AppTaskCreate (void);
static void AppObjCreate (void);

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
  
  MX_DEBUG_USART_Init();
	

	MX_USART3_Init();
  /* 初始化LED */
  LED_GPIO_Init();
  /* 板子按键初始化 */
  KEY_GPIO_Init();
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

	
	/* 创建任务 */
	AppTaskCreate();
  /* 创建任务通信机制 */
	AppObjCreate();	
  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
  
  /* 无限循环 */
  while (1)
  {
		
  }
}

/**
  * 函数功能: 接口消息处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
//static uint8_t g_uiCount[8] = 0; /* 设置为全局静态变量，方便数据更新 */
static void vTaskTaskUserIF(void *pvParameters)
{
  uint8_t pcWriteBuffer[500];
  uint8_t g_uiCount;
    /* 创建按键 */
  
 
  printf("KEY1、KEY2和KEY3对应不同任务情况\n");

    while(1)
    {
     
        HAL_UART_Receive(&husart_usart3,DMAaRxBuffer,8,0xff);
	    printf("DMAaRxBuffer[0]=%#x \n",DMAaRxBuffer[0]);
		printf("DMAaRxBuffer[1]=%#x \n",DMAaRxBuffer[1]);
      
      if(START_StateRead()==KEY_DOWN)//if(Key_AccessTimes(&key1,KEY_ACCESS_READ)!=0)//按键被按下过
      {
        printf("=================================================\r\n");
        printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        vTaskList((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
       
        printf("\r\n任务名       运行计数         使用率\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        //Key_AccessTimes(&key1,KEY_ACCESS_WRITE_CLEAR);
      }
     // if(DIR_StateRead()==KEY_DOWN)
     if(DMAaRxBuffer[0]==0xa6)////if(Key_AccessTimes(&key2,KEY_ACCESS_READ)!=0)//按键被按下过
      {         
       // printf("KEY2按下，启动单次定时器中断，50ms后在定时器中断给任务vTaskMsgPro发送消息\r\n");
		  
	     MSG_T   *ptMsg;
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      
      /* 初始化结构体指针 */
      ptMsg = &g_tMsg;
      
      /* 初始化数组 */
      
      ptMsg->ulData[0]=DMAaRxBuffer[0];
	  ptMsg->ulData[1]=DMAaRxBuffer[1];
	  ptMsg->ulData[2]=DMAaRxBuffer[2];
	  ptMsg->ulData[3]=DMAaRxBuffer[3];
	  ptMsg->ulData[4]=DMAaRxBuffer[4];
	  ptMsg->ulData[5]=DMAaRxBuffer[5];
	  ptMsg->ulData[6]=DMAaRxBuffer[6];
	  ptMsg->ulData[7]=DMAaRxBuffer[7];
    
		 /* 向消息队列发数据 */
       xQueueSendFromISR(xQueue1,
                  (void *)&ptMsg,
                   &xHigherPriorityTaskWoken);  
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);  
       xQueueSendFromISR(xQueue2,
                  (void *)&ptMsg,
                   &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        //if(KEY1_StateRead()==KEY_DOWN)//Key_AccessTimes(&key2,KEY_ACCESS_WRITE_CLEAR);
		  }
      
      if(DMAaRxBuffer[0]==0xa1)//if(Key_AccessTimes(&key3,KEY_ACCESS_READ)!=0)//按键被按下过
      {         
        printf("KEY3按下，启动单次定时器中断，50ms后在定时器中断给任务vTaskMsgPro发送消息\r\n");
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
         
       if(DMAaRxBuffer[1]==0x01)
		  {
			   g_uiCount=DMAaRxBuffer[2];
			   printf("aRxBuffer[2]= %#x\n",DMAaRxBuffer[2]);
			  /* 向消息队列发数据 */
			     xQueueSendFromISR(xQueue1,
					  (void *)&g_uiCount,
					  &xHigherPriorityTaskWoken);

			 /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
			 portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					
		
		 }
		 if(DMAaRxBuffer[2]==0x02)
		 {
		       
			   printf("aRxBuffer[3]= %#x\n",DMAaRxBuffer[3]);
			   g_uiCount=DMAaRxBuffer[3];
			   printf("g_uiCount= %#x\n",g_uiCount);
			 /* 向消息队列发数据 */
			  xQueueSendFromISR(xQueue1,
					  (void *)&g_uiCount,
					  &xHigherPriorityTaskWoken);
			 portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

			 /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
			 //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			//HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_2);
			 //Key_AccessTimes(&key3,KEY_ACCESS_WRITE_CLEAR);
         
           }
      
     //vTaskDelay(1000);
    }
	  vTaskDelay(100);
  }
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
static void vTaskLED1(void *pvParameters)
{
	MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
	

  while(1)
  {
    xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
                            (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
                            (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */


    if(xResult == pdPASS)
    {
      /* 成功接收，并通过串口将数据打印出来 */
	
		  //HAL_Delay(500);
		
	   printf("--------------------This is vTaskLED1 --------------\n");	
      printf("vTaskLED1 ptMsg->ucMessageID = %#x\r\n", ptMsg->ucMessageID);
      printf("接收到消息队列数据ptMsg->ulData[0] = %#x\r\n", ptMsg->ulData[0]);
	  printf("接收到消息队列数据ptMsg->ulData[1] = %#x\r\n", ptMsg->ulData[1]);
	  printf("接收到消息队列数据ptMsg->ulData[2] = %#x\r\n", ptMsg->ulData[2]);
	  printf("接收到消息队列数据ptMsg->ulData[3] = %#x\r\n", ptMsg->ulData[3]);
	  printf("接收到消息队列数据ptMsg->ulData[4] = %#x\r\n", ptMsg->ulData[4]);
	  printf("接收到消息队列数据ptMsg->ulData[5] = %#x\r\n", ptMsg->ulData[5]);
	  printf("接收到消息队列数据ptMsg->ulData[6] = %#x\r\n", ptMsg->ulData[6]);
	  printf("接收到消息队列数据ptMsg->ulData[7] = %#x\r\n", ptMsg->ulData[7]);
      printf("接收到消息队列数据ptMsg->usData[3] = %#x\r\n", ptMsg->usData[3]);
	  	
		
    }
    else
    {
      LED1_TOGGLE;
       
    }
	vTaskDelay(100);
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

static void vTaskLED2(void *pvParameters)
{
    MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
    
  while(1)
  {
        xResult = xQueueReceive(xQueue1,                   /* 消息队列句柄 */
                            (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
                            (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
      
    if(xResult == pdPASS)
    {
      printf("This is vTaskLED2 \n");
	
	 /* 3.3为AD转换的参考电压值，stm32的AD转换为12bit，2^12=4096，
       即当输入为3.3V时，AD转换结果为4096 */
       ADC_ConvertedValueLocal =(double)ADC_ConvertedValue*3.3/4096; 	
		printf("AD转换原始值 = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("计算得出电压值 = %f V \r\n",ADC_ConvertedValueLocal); 
   
	  
    }
    vTaskDelay(200);
  }
}

/**
  * 函数功能: LED3任务
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void vTaskLED3(void *pvParameters)
{
    
	while(1)
    {
        
        taskENTER_CRITICAL();  
           /* 使能电机 */
    //if(START_StateRead() == KEY_DOWN)
    {
      Enable_BLDC();
      /* 先以恒定的转空比启动,然后再来调速 */
//      BLDCMotor.PWM_Duty = (int32_t)(BLDCMOTOR_TIM_PERIOD*5/100);
//      HAL_Delay(100);
      start_flag  = 1;
      IS_EnableMotor = 1;
      LED2_OFF;
    }
     /* 电机换向 */
    if(DIR_StateRead() == KEY_DOWN)
    {
      Dir = -Dir;
    }
    /* 电机停止 */
    if(BRAKE_StateRead() == KEY_DOWN)
    {
      start_flag = 0;
      Disable_BLDC();
      IS_EnableMotor = 0;      
    }
             taskEXIT_CRITICAL();
           
       vTaskDelay(20);
     }
 }



/***********************************************************************************
  *
  * 函数功能: 创建任务应用
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  *
***********************************************************************************/
static void AppTaskCreate (void)
{

    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 1024,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
    xTaskCreate( vTaskLED1,   	      /* 任务函数  */
                 "vTaskLED1",     	  /* 任务名    */
                 512,                /* 任务栈大小，单位word，也就是4字节 ,512*/
                 NULL,              	/* 任务参数  */
                 2,                 	/* 任务优先级,小-低*/
                 &xHandleTaskLED1 );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED2,    		      /* 任务函数  */
                 "vTaskLED2",  		    /* 任务名    */
                 1024,         		    /* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		    /* 任务参数  */
                 3,           		    /* 任务优先级*/
                 &xHandleTaskLED2 );  /* 任务句柄  */
	
	xTaskCreate( vTaskLED3,     		    /* 任务函数  */
                 "vTaskLED3",   		  /* 任务名    */
                 2048,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		  /* 任务参数  */
                 4,               		/* 任务优先级*/
                 &xHandleTaskLED3 );  /* 任务句柄  */
	
}

/***************************************************
 *
 *函数名称：AppObjCreate(void)
 *函数功能：创建消息队列。
 *
 *
****************************************************/
static void AppObjCreate (void)
{
	/* 创建10个uint8_t型消息队列 */
	xQueue1 = xQueueCreate(10, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue2 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
       printf("Thisi create is fail \n") ;/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
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
