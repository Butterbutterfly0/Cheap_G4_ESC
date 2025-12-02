#ifndef _SIX_PHASE_H_
#define _SIX_PHASE_H_

#include <stdint.h>

typedef enum{
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_4,
    PHASE_5,
    PHASE_6
} 
six_step_change_phase_t;
typedef enum{
    FORWARD = 0,
    BACKWARD = 1,
}
six_step_direction_t;

extern void six_step_set_pulse(six_step_change_phase_t phase, six_step_direction_t direction);

extern void six_step_change_phase(six_step_change_phase_t* phase, six_step_direction_t direction);
#define SAFE_CMP_CMPLE 4000U
#define SAFE_CMP (8000U-6500U)

void enable_PWM_channel(uint32_t ch_index);
void disable_PWM_channel(uint32_t ch_index);

#endif