#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"

uint8_t start_counter = 0;

uint32_t LED_Tick = 0;

#include "tim.h"
uint8_t LED_array[25]={	55,55,55,55,55,55,55,133,
						55,55,55,55,55,55,55,133,
						55,55,55,55,55,55,55,133,0};
void Live_Task(void *argument)
{
  for(;;)
  {
    HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t*)LED_array,25);
    if(LED_Tick%2==0)
        LIVE_Light_On();
    else
        LIVE_Light_Off();
    LED_Tick++;

    if(FOC_State == FOC_Stop)
    {
        vTaskDelay(500);
    }
    else if(FOC_State == FOC_Start)
    {
        vTaskDelay(10);
    }
    else if(FOC_State == FOC_Ready)
    {
        start_counter++;
        vTaskDelay(5);
        if(start_counter>10)
        {
            FOC_State = FOC_Start; 
            // Three_Phase_Inverter_Start();
            
        }
    }
  }
}