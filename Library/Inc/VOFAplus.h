#ifndef VOFAPLUS_H
#define VOFAPLUS_H

#include <stdint.h>

typedef struct
{
    uint32_t JustFloat_Num;
    uint32_t byte_num;
    float *float_buf;
    uint8_t *uint8_buf;
}
VOFA_tx_frame_t;

void VOFA_tx_frame_init(VOFA_tx_frame_t *tx_frame,uint32_t num);
void VOFA_tx_frame_update(VOFA_tx_frame_t *tx_frame);
uint8_t* VOFA_tx_frame_get_u8ptr(VOFA_tx_frame_t *tx_frame);
uint32_t VOFA_tx_frame_get_u8_num(VOFA_tx_frame_t *tx_frame);

#endif