
/* ����ͷ�ļ� ----------------------------------------------------------------*/
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

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		2
//�����ջ��С	
#define TASK1_STK_SIZE 		256  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);

//�������ȼ�
#define KEYPROCESS_TASK_PRIO 3
//�����ջ��С	
#define KEYPROCESS_STK_SIZE  256 
//������
TaskHandle_t Keyprocess_Handler;
//������
void Keyprocess_task(void *pvParameters);


//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define MESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��
QueueHandle_t Message_Queue;	//��Ϣ���о��




/* ˽�к궨�� ----------------------------------------------------------------*/
#define SENDBUFF_SIZE              100  // ����DMA���ͻ�������С



uint32_t IS_EnableMotor = 0;  // ʹ�ܵ����־
uint32_t Time_CNT = 0;
int32_t  flag = 0;
int32_t  start_flag = 0;

/* ˽�б��� ------------------------------------------------------------------*/
uint8_t DMAaRxBuffer[8];                      // �������� 
uint8_t DMAaTxBuffer[SENDBUFF_SIZE];       // ����DMA���ͻ�����


// ���ڱ���ת�������ĵ�ѹֵ	 
float ADC_ConvertedValueLocal;
uint32_t ADC_ConvertedValue;




typedef struct Msg
{
	uint8_t  ucMessageID;
	uint16_t usData[8];
	uint32_t ulData[8];
}MSG_T;

MSG_T   g_tMsg; /* ����һ���ṹ��������Ϣ���� */

/* ��չ���� ------------------------------------------------------------------*/
extern __IO uint16_t CCR1_Val;







/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     //ʹ��PWRʱ��

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  //���õ�ѹ�������ѹ����1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        //��HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    //��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            //PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 //8��ƵMHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               //336��Ƶ
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     //2��Ƶ���õ�168MHz��ʱ��
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 //USB/SDIO/������������ȵ���PLL��Ƶϵ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�168MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ� 168MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;               // APB1ʱ�ӣ�42MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;               // APB2ʱ�ӣ�84MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_RCC_EnableCSS();                                            // ʹ��CSS���ܣ�����ʹ���ⲿ�����ڲ�ʱ��ԴΪ����
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
  
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  
   MX_USARTx_Init();
	
   /* ��ʼ���������� */
  KEY_Init();
	
  /* ��ʼ��LED */
  LED_GPIO_Init();
  /* ���Ӱ�����ʼ�� */
 // KEY_GPIO_Init();
  /* ������ʱ����ʼ����100us�ж�һ�� */
 BLDCMOTOR_TIMx_Init();
  /* ������ʱ�� */
  HAL_TIM_Base_Start(&htimx_BLDC);
  /* ADC ��ʼ�� */
   MX_DMA_Init();
  /* ADC ��ʼ�� */
  MX_ADCx_Init();

/* ����ADת����ʹ��DMA������ж� */
  HAL_ADC_Start_DMA(&hadcx,&ADC_ConvertedValue,1); 

	 //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������                
    vTaskStartScheduler();          //�����������
	
 
  
 
}

/********************************************************************
  *
  * ��������: �ӿ���Ϣ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *
 ********************************************************************/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	//������Ϣ����
    Key_Queue=xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));        //������ϢKey_Queue
    Message_Queue=xQueueCreate(MESSAGE_Q_NUM,USART_REC_LEN); //������ϢMessage_Queue,��������Ǵ��ڽ��ջ���������
	
    //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
    //����TASK2����
    xTaskCreate((TaskFunction_t )Keyprocess_task,     
                (const char*    )"keyprocess_task",   
                (uint16_t       )KEYPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEYPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Keyprocess_Handler);
    
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

/********************************************************
  *
  * �������ƣ�
  * ��������: ������е���Ϣ,xQueue2 ,�ṹ���ַ&ptMsg.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *
*********************************************************/
void task1_task(void *pvParameters)
{
	uint8_t key,i=0;
    BaseType_t err;
	while(1)
	{
		key=KEY_Scan(0);            //ɨ�谴��
        if((Key_Queue!=NULL)&&(key))   //��Ϣ����Key_Queue�����ɹ�,���Ұ���������
        {
            err=xQueueSend(Key_Queue,&key,10);
            if(err==errQUEUE_FULL)   //���Ͱ���ֵ
            {
                printf("����Key_Queue���������ݷ���ʧ��!\r\n");
            }
        }
        i++;
        if(i==50)
        {
            i=0;
            LED1_TOGGLE ;
        }
        vTaskDelay(10);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}
}

/***************************************************************
  *
  * �������ƣ�
  * ��������: �������1�����ݣ�xQueue1,���ն���1�����ݡ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *
***************************************************************/
void Keyprocess_task(void *pvParameters)
{
	uint8_t key;
	while(1)
	{
        if(Key_Queue!=NULL)
        {
            if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))//������ϢKey_Queue
            {
                switch(key)
                {
                    case START_PRES:		//KEY_UP����LED1
                          Enable_BLDC();
                          /* ���Ժ㶨��ת�ձ�����,Ȼ���������� */
                         //BLDCMotor.PWM_Duty = (int32_t)(BLDCMOTOR_TIM_PERIOD*5/100);
                         //HAL_Delay(100);
                         start_flag  = 1;
                         IS_EnableMotor = 1;
                        LED1_TOGGLE;
                        break;
                    case BRAKE_PRES:		//KEY2���Ʒ�����
                          start_flag = 0;
                         Disable_BLDC();
                         IS_EnableMotor = 0;  
                         LED2_ON;        
                         break;
                  
                   case KEY2_PRES : //�����
                          Dir = -Dir;
                          LED1_ON;
             
                     break;
                   case KEY3_PRES :
                     ADC_ConvertedValueLocal =(double)ADC_ConvertedValue*3.3/4096; 	
                    HAL_Delay(10); 
                    printf("ADת��ԭʼֵ = 0x%04X \r\n", ADC_ConvertedValue); 
                    printf("����ó���ѹֵ = %f V \r\n",ADC_ConvertedValueLocal); 
                    
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
		vTaskDelay(10);      //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}
}

 


void HAL_SYSTICK_Callback()
{
  if(IS_EnableMotor)
  {
    BLDCMotor.Lock_Time++;//Lock_Time++;
    /* ����100ms ����Ϊ�Ƕ�ת,ֹͣPWM��� */
    if(Lock_Time >= 100)  //100ms
    {
      Disable_BLDC();
      LED2_TOGGLE;
      BLDCMotor.Lock_Time = 0;//Lock_Time = 0;
    }
  }
}
