#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "OS_Support.h"

#include "tim.h"
uint32_t Buzzer_task_start_tick = 0;
uint32_t Buzzer_tick = 0;

uint32_t Buzzer_CCR = 0;

#define AMP 0.1

void Buzzer_Task(void *argument)
{
    Buzzer_task_start_tick = xTaskGetTickCount();
    
    for(;;)
    {
        Buzzer_tick = xTaskGetTickCount() - Buzzer_task_start_tick;
        if(Buzzer_tick == 1200)
        {
            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        }
        else if(Buzzer_tick == 900)
        {
            Buzzer_CCR = 1000000 / 440 ;
            __HAL_TIM_SetAutoreload(&htim3, Buzzer_CCR-1);
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Buzzer_CCR*0.1*AMP);
        }        
        else if(Buzzer_tick == 600)
        {
            Buzzer_CCR = 1000000 / 392 ;
            __HAL_TIM_SetAutoreload(&htim3, Buzzer_CCR-1);
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Buzzer_CCR*0.1*AMP);
        }
        else if(Buzzer_tick==300)
        {
            Buzzer_CCR = 1000000 / 349 ;
             __HAL_TIM_SetAutoreload(&htim3, Buzzer_CCR-1);
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Buzzer_CCR*0.1*AMP);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
        }
        else
        {
            
        }

        vTaskDelay(1);
    }
}