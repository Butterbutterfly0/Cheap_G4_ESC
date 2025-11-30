#ifndef FOC_H
#define FOC_H

#include <stdint.h>
#include <math.h>

#define SECTOR_ONE 3
#define SECTOR_TWO 1
#define SECTOR_THREE 5
#define SECTOR_FOUR 4
#define SECTOR_FIVE 6
#define SECTOR_SIX 2

extern void Clark_Transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta);// equal A

extern void Park_Transform(float Ualpha, float Ubeta, float theta, float *Ud, float *Uq);

extern void Anti_Park_Transform(float Ud,float Uq, float theta, float *Ualpha,float *Ubeta);

extern void Anti_Clark_Transform(float Ualpha, float Ubeta, float *Ua, float *Ub, float *Uc);

extern void SVPWM( float Ualpha, float Ubeta, float VBarbus, 
            float* PWMa_duty, float* PWMb_duty, float* PWMc_duty);

#define SLIDING_WINDOW_SIZE 1000
typedef struct{
    uint16_t sample_current[3];
    
    uint16_t sample_current_slide_window[3][SLIDING_WINDOW_SIZE];

    uint16_t filtered_current[3]; 
}
sample_struct_t;

void sample_struct_init(sample_struct_t *sample);

void sample_struct_update(sample_struct_t *sample);
#endif