
/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
//#include "math.h"
#include "stm32f4xx_hal.h"
#include "bldc/bsp_bldc.h" 
#include "key/bsp_key.h"
#include "led/bsp_led.h"
#include "usart/bsp_usartx.h"
#include "adc/bsp_adc.h"
#include "CAN/bsp_CAN.h"
/* 私有类型定义 --------------------------------------------------------------*/
#define SENDBUFF_SIZE              100  // 串口DMA发送缓冲区大小
/* 私有变量 ------------------------------------------------------------------*/
uint32_t IS_EnableMotor = 0;  // 使能电机标志
int32_t  flag = 0;
int32_t  start_flag = 0;

uint8_t DMAaRxBuffer[8];                      // 接收数据 
uint8_t DMAaTxBuffer[SENDBUFF_SIZE];       // 串口DMA发送缓冲区
// 用于保存转换计算后的电压值	 
float ADC_ConvertedValueLocal;
uint32_t ADC_ConvertedValue;
void CAN_SetTxMsg(void)
{						 
  hCAN.pTxMsg->ExtId=0x1314;					   /* 使用的扩展ID */
  hCAN.pTxMsg->IDE=CAN_ID_EXT;					 /* 扩展模式 */
  hCAN.pTxMsg->RTR=CAN_RTR_DATA;				 /* 发送的是数据 */
  hCAN.pTxMsg->DLC=2;							       /* 数据长度为2字节 */
  hCAN.pTxMsg->Data[0]=0xAB;
  hCAN.pTxMsg->Data[1]=0xCD;
}

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
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     // 使能PWR时钟

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // 设置调压器输出电压级别1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        // 打开HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    // 打开PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            // PLL时钟源选择HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 // 8分频MHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               // 336倍频
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     // 2分频，得到168MHz主时钟
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 // USB/SDIO/随机数产生器等的主PLL分频系数
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
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                 // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{
  uint8_t key;
    /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
  /* 初始化按键配置 */
  KEY_Init();
  /* 初始化指示灯 */
  LED_GPIO_Init();
  /* 初始化霍尔传感器接口 */
    
  /* 初始化串口并配置串口中断优先级 */
  MX_USARTx_Init();
  
  HALL_TIMx_Init();
  /* 初始化定时器各通道输出 */
  BLDCMOTOR_TIMx_Init();
  /* 启动定时器 */
  HAL_TIM_Base_Start(&htimx_BLDC);  
  /* 初始化串口功能 */

  MX_DMA_Init();
  /* ADC 初始化 */
  MX_ADCx_Init();

  /* 启动AD转换并使能DMA传输和中断 */
  HAL_ADC_Start_DMA(&hadcx,&ADC_ConvertedValue,1);  
  
   MX_CAN_Init();
  CAN_SetTxMsg();
  
  //HAL_CAN_Receive_IT(&hCAN, CAN_FIFO0); 
  
 
  while (1)
  {
      
      
       
     key=KEY_Scan(0); 
     //HAL_CAN_Receive_IT(&hCAN, CAN_FIFO0); 
     HAL_CAN_Receive(&hCAN,CAN_FIFO0,0xff);
    
           
               //按键扫描
     switch(key)
     {
         case KEY0_PRES :
             Enable_BLDC();
          /* 先以恒定的转空比启动,然后再来调速 */
    //      BLDCMotor.PWM_Duty = (int32_t)(BLDCMOTOR_TIM_PERIOD*5/100);
    //      HAL_Delay(100);
          start_flag  = 1;
          IS_EnableMotor = 1;
          LED1_OFF;
             
             break;
         case KEY1_PRES :
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
     
     }
     
 
  }
}

/**
  * 函数功能: 系统滴答定时器中断回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 每发生一次滴答定时器中断进入该回调函数一次
  */

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
/**
  * 函数功能: CAN接收完成中断回调服务程序
  * 输入参数: hcan：CAN外设句柄指针
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
  printf( "\n成功接收到“从机”返回的数据\n ");	
	printf("接收到的报文为：\n"); 
	printf("》扩展ID号ExtId：0x%x\n",hCAN.pRxMsg->ExtId);
  printf("》接收到数据段长度：%d\n",hCAN.pRxMsg->DLC);
	printf("》数据段的内容:Data[0]= 0x%X ，Data[1]=0x%X \n",hCAN.pRxMsg->Data[0],hCAN.pRxMsg->Data[1]);
	
  HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}
