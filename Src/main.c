
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
//#include "math.h"
#include "stm32f4xx_hal.h"
#include "bldc/bsp_bldc.h" 
#include "key/bsp_key.h"
#include "led/bsp_led.h"
#include "usart/bsp_usartx.h"
#include "adc/bsp_adc.h"
#include "CAN/bsp_CAN.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
#define SENDBUFF_SIZE              100  // ����DMA���ͻ�������С
/* ˽�б��� ------------------------------------------------------------------*/
uint32_t IS_EnableMotor = 0;  // ʹ�ܵ����־
int32_t  flag = 0;
int32_t  start_flag = 0;

uint8_t DMAaRxBuffer[8];                      // �������� 
uint8_t DMAaTxBuffer[SENDBUFF_SIZE];       // ����DMA���ͻ�����
// ���ڱ���ת�������ĵ�ѹֵ	 
float ADC_ConvertedValueLocal;
uint32_t ADC_ConvertedValue;
void CAN_SetTxMsg(void)
{						 
  hCAN.pTxMsg->ExtId=0x1314;					   /* ʹ�õ���չID */
  hCAN.pTxMsg->IDE=CAN_ID_EXT;					 /* ��չģʽ */
  hCAN.pTxMsg->RTR=CAN_RTR_DATA;				 /* ���͵������� */
  hCAN.pTxMsg->DLC=2;							       /* ���ݳ���Ϊ2�ֽ� */
  hCAN.pTxMsg->Data[0]=0xAB;
  hCAN.pTxMsg->Data[1]=0xCD;
}

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
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     // ʹ��PWRʱ��

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // ���õ�ѹ�������ѹ����1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        // ��HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    // ��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            // PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 // 8��ƵMHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               // 336��Ƶ
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     // 2��Ƶ���õ�168MHz��ʱ��
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 // USB/SDIO/������������ȵ���PLL��Ƶϵ��
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
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                 // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
  uint8_t key;
    /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  /* ��ʼ���������� */
  KEY_Init();
  /* ��ʼ��ָʾ�� */
  LED_GPIO_Init();
  /* ��ʼ�������������ӿ� */
    
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_USARTx_Init();
  
  HALL_TIMx_Init();
  /* ��ʼ����ʱ����ͨ����� */
  BLDCMOTOR_TIMx_Init();
  /* ������ʱ�� */
  HAL_TIM_Base_Start(&htimx_BLDC);  
  /* ��ʼ�����ڹ��� */

  MX_DMA_Init();
  /* ADC ��ʼ�� */
  MX_ADCx_Init();

  /* ����ADת����ʹ��DMA������ж� */
  HAL_ADC_Start_DMA(&hadcx,&ADC_ConvertedValue,1);  
  
   MX_CAN_Init();
  CAN_SetTxMsg();
  
  //HAL_CAN_Receive_IT(&hCAN, CAN_FIFO0); 
  
 
  while (1)
  {
      
      
       
     key=KEY_Scan(0); 
     //HAL_CAN_Receive_IT(&hCAN, CAN_FIFO0); 
     HAL_CAN_Receive(&hCAN,CAN_FIFO0,0xff);
    
           
               //����ɨ��
     switch(key)
     {
         case KEY0_PRES :
             Enable_BLDC();
          /* ���Ժ㶨��ת�ձ�����,Ȼ���������� */
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
     
     }
     
 
  }
}

/**
  * ��������: ϵͳ�δ�ʱ���жϻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ÿ����һ�εδ�ʱ���жϽ���ûص�����һ��
  */

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
/**
  * ��������: CAN��������жϻص��������
  * �������: hcan��CAN������ָ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
  printf( "\n�ɹ����յ����ӻ������ص�����\n ");	
	printf("���յ��ı���Ϊ��\n"); 
	printf("����չID��ExtId��0x%x\n",hCAN.pRxMsg->ExtId);
  printf("�����յ����ݶγ��ȣ�%d\n",hCAN.pRxMsg->DLC);
	printf("�����ݶε�����:Data[0]= 0x%X ��Data[1]=0x%X \n",hCAN.pRxMsg->Data[0],hCAN.pRxMsg->Data[1]);
	
  HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}
