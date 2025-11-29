#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"



uint32_t LED_Tick = 0;
void Live_Task(void *argument)
{
  for(;;)
  {
    if(LED_Tick%2==0)
        LIVE_Light_On();
    else
        LIVE_Light_Off();
    LED_Tick++;

    if(FOC_State == FOC_Stop)
        vTaskDelay(500);
    else
        vTaskDelay(100);
  }
}