#include "OS_Support.h"

osEventFlagsId_t EXTI_event_Group = NULL;

osEventFlagsId_t ADC_event_Group = NULL;

const osEventFlagsAttr_t EXTI_event_Group_attributes = {
  .name = "EXTI_event_Group",
  .attr_bits = osFlagsError | osFlagsWaitAll,
  .cb_mem = NULL,
  .cb_size = 0
};

const osEventFlagsAttr_t ADC_event_Group_attributes = {
  .name = "ADC_event_Group",
  .attr_bits = osFlagsError | osFlagsWaitAll,
  .cb_mem = NULL,
  .cb_size = 0
};
void OS_Init(void)
{
    taskENTER_CRITICAL();
    
    EXTI_event_Group = osEventFlagsNew(&EXTI_event_Group_attributes);

    ADC_event_Group = osEventFlagsNew(&ADC_event_Group_attributes);
    //EXTI_event_Group = xEventGroupCreate();

    taskEXIT_CRITICAL();
}