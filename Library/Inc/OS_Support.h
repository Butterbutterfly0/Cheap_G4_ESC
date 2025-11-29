#ifndef OS_SUPPORT_H
#define OS_SUPPORT_H

#include "cmsis_os.h"
#include "cmsis_os2.h"

extern osEventFlagsId_t EXTI_event_Group;
extern osEventFlagsId_t ADC_event_Group;

extern void OS_Init(void);

#endif