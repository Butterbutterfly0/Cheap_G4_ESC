#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "OS_Support.h"

#include "fdcan.h"

uint8_t data[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

void CAN_Init(void)
{
    FDCAN_FilterTypeDef fdcan_filter;
    fdcan_filter.IdType       = FDCAN_STANDARD_ID;
    fdcan_filter.FilterIndex  = 0;
    fdcan_filter.FilterType   = FDCAN_FILTER_MASK;
    fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    fdcan_filter.FilterID1    = 0x00;
    fdcan_filter.FilterID2    = 0x00;

    HAL_FDCAN_ConfigFilter(&hfdcan1, &fdcan_filter);
        
    
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT,
                                     FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
    
    HAL_FDCAN_Start(&hfdcan1);
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    // HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_CFG_RX_FIFO0, 1);
}


void Send_Feedback(void)
{
    uint32_t ret = 0;
    FDCAN_TxHeaderTypeDef tx_header;
    // if(xSemaphoreTake(_registermtx,portMAX_DELAY)==pdTRUE){
    tx_header.IdType              = FDCAN_STANDARD_ID;
    tx_header.Identifier          = 0x100;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;

    ret = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, data);
    
}
void CAN_Communication_task(void *argument)
{
    osDelay(1000);
    CAN_Init();
    
  for(;;)
  {
    vTaskDelay(1);
Send_Feedback();

  }
}
uint32_t cnt = 0;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  /* Prevent unused argument(s) compilation warning */
    cnt++;
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_FDCAN_RxFifo0Callback could be implemented in the user file
   */
}