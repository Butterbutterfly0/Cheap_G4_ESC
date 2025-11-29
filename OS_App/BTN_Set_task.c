#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "OS_Support.h"

void BTN_Set_Task(void *argument)
{
  for(;;)
  {
    BaseType_t uxBits;
    uxBits  = osEventFlagsWait( EXTI_event_Group,
                                0x01<<4,
                                osFlagsWaitAll|osFlagsNoClear,
                                portMAX_DELAY);
    osEventFlagsClear( EXTI_event_Group,
                      0x01<<4);
    // uxBits  = xEventGroupWaitBits(
    //     EXTI_event_Group,    /* The event group that the bits are being waited for. */
    //     0x01<<4,             /* Bits that should be set in the event group. */
    //     pdTRUE,              /* Should the bits be cleared after being set/pending ints. */
    //     pdFALSE,             /* Should the bits be cleared after being set/pending ints. */
    //     portMAX_DELAY        /* The maximum number of ticks to wait for the bits to be set/pending. */
    // );
    if(FOC_State == FOC_Stop)
    {
        FOC_State = FOC_Start;
        Three_Phase_Inverter_Start();
    }
    else
    {
        FOC_State = FOC_Stop;
        Three_Phase_Inverter_Stop();
    }
  }
}