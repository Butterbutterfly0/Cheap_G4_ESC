#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "OS_Support.h"



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==GPIO_PIN_4)
    {
        if(EXTI_event_Group)
        {
            osEventFlagsSet(EXTI_event_Group,0x01<<4);
        }
        // xEventGroupSetBitsFromISR(EXTI_event_Group,0x01<<4,&xHigherPriorityTaskWoken);
    }
//   if(GPIO_Pin==GPIO_PIN_4)
//   {
//     start_motor_flag = ! start_motor_flag;

//     if(start_motor_flag)
//     {
// 			// LIVE_Light_Off();
//       Three_Phase_Inverter_Start();
//     }
//     else
//     {
// 			// LIVE_Light_On();
//       Three_Phase_Inverter_Stop();
//     }
//   }
}
__weak void Timer_emergency(void)
{
    
}
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC1)
    {
        // if(ADC_event_Group)
        // {
        //     osEventFlagsSet(ADC_event_Group,0x01<<1);
        // }
        Timer_emergency();
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC2)
    {
        if(ADC_event_Group)
        {
            osEventFlagsSet(ADC_event_Group,0x01<<2);
        }
    }
}
