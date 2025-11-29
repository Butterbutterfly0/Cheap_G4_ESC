#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "FOC.h"
#include "OS_Support.h"

#include "adc.h"
#include"tim.h"

uint16_t ADC2_buf = 0.0f;
float VB = 0.0f;
const float VB_normal_gain = 1.105845f;

float VB_mean = 0.0f;

uint16_t VB_mean_counter = 0;
#define VB_mean_counter_MAX 100

void BusBar_Task(void *argument)
{
    HAL_ADC_Start_DMA(&hadc2,(uint32_t*)&ADC2_buf,1);
    HAL_TIM_Base_Start(&htim15);
  for(;;)
  {
     osEventFlagsWait(   ADC_event_Group,
                         0x01<<2,
                         osFlagsWaitAny,
                         portMAX_DELAY);
     osEventFlagsClear(  ADC_event_Group,
                         0x01<<2);
     VB = (float)ADC2_buf * 3.3f / 4096.0f*25*VB_normal_gain;
     if(VB_mean_counter<VB_mean_counter_MAX)
     {
         VB_mean += VB;
         VB_mean_counter++;
     }
     else if(VB_mean_counter==VB_mean_counter_MAX)
     {
         VB_mean = VB_mean / VB_mean_counter;
        VB_mean_counter++;
     }
  }
}