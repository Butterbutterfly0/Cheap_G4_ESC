#ifndef FOC_PORTABLE_H
#define FOC_PORTABLE_H

#include <stdint.h>

typedef enum{
    FOC_Stop = 0,
    FOC_Start = 1,
    FOC_Ready = 2
}
FOC_State_t;

extern FOC_State_t FOC_State;

extern void Three_Phase_Inverter_Start(void);
extern void Three_Phase_Inverter_Stop(void);

extern void Phase_Current_Sample_Start(void);
extern void Phase_Current_Sample_Stop(void);

extern void LIVE_Light_On(void);
extern void LIVE_Light_Off(void);

extern void Get_Current_Sample(uint16_t *current_sample);


#endif