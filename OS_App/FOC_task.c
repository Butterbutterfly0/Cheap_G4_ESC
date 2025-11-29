#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "FOC.h"
#include "OS_Support.h"

#include "tim.h"

float etheta = 0.0f;

float VBarbus = 12.0f;

float Ud = 0.0f, Uq = 0.0f;

float Ualpha = 0.0f, Ubeta = 0.0f;

float PWM_cmp[3] = {0.0f, 0.0f, 0.0f};
void FOC_Task(void *argument)
{
    Uq=1.0f;
  for(;;)
  {
    if(FOC_State == FOC_Stop)
    {
        
    }
    else if(FOC_State == FOC_Start)
    {
        Anti_Park_Transform(Ud,Uq,etheta,&Ualpha,&Ubeta);
        SVPWM(Ualpha,Ubeta,VBarbus,PWM_cmp,PWM_cmp+1,PWM_cmp+2);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM_cmp[0]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, PWM_cmp[1]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, PWM_cmp[2]*8000);
    }

    {
        etheta += 0.1000f;
        if(etheta>6.28f)
            etheta =0.0f;
        else if(etheta<-0.0f)
            etheta = 6.28f;
    }
    vTaskDelay(1);
  }
}