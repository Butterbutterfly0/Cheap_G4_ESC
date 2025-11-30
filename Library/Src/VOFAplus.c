#include "VOFAplus.h"
#include <string.h>
#include "cmsis_os.h"
void VOFA_tx_frame_init(VOFA_tx_frame_t *tx_frame,uint32_t num)
{
    tx_frame->JustFloat_Num = num;
    tx_frame->byte_num = num*4+4;
    tx_frame->float_buf = pvPortMalloc(sizeof(float)*num);
    tx_frame->uint8_buf = pvPortMalloc(tx_frame->byte_num);
    tx_frame->uint8_buf[4*num] = 0x00;
    tx_frame->uint8_buf[4*num+1] = 0x00;
    tx_frame->uint8_buf[4*num+2] = 0x80;
    tx_frame->uint8_buf[4*num+3] = 0x7f;
}

void VOFA_tx_frame_update(VOFA_tx_frame_t *tx_frame)
{
    memcpy(tx_frame->uint8_buf,tx_frame->float_buf,sizeof(float)*tx_frame->JustFloat_Num);
}

uint8_t* VOFA_tx_frame_get_u8ptr(VOFA_tx_frame_t *tx_frame)
{
    return tx_frame->uint8_buf;
}

uint32_t VOFA_tx_frame_get_u8_num(VOFA_tx_frame_t *tx_frame)
{
    return tx_frame->byte_num;
}