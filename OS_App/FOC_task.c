#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "FOC.h"
#include "OS_Support.h"
#include "SMO.h"

#include "Six_phase.h"

#include "tim.h"
#include "usart.h"

#include "VOFAplus.h"

float etheta = 0.0f;

float VBarbus = 12.0f;

float Ud = 0.0f, Uq = 0.0f;

float Ualpha = 0.0f, Ubeta = 0.0f;

float PWM_cmp[3] = {0.0f, 0.0f, 0.0f};

float Ia_meas=0.0f,Ib_meas=0.0f,Ic_meas=0.0f;
float Ialpha_meas =0.0f, Ibeta_meas =0.0f;
float Id_meas =0.0f,Iq_meas =0.0f;

const float ADC_ecd2V = 3.3f/4096.0f;
const float Current_gain = 2.0f;

sample_struct_t sample;

VOFA_tx_frame_t FOC_frame;
uint32_t count =0;

uint32_t adc_offset[3] = {0.0f,0.0f,0.0f};
uint32_t calibration_counter = 0;

Motor_Params_t motor_params =
{
    .Rs = MOTOR_2804_RS,
    .Ls = MOTOR_2804_LS,
    .psi_f = MOTOR_2804_PSI_F,
    .Ts = 0.00005f
};

SMO_observer_t observer;

six_step_change_phase_t phase = PHASE_1;
six_step_direction_t direction = FORWARD;
uint32_t change_counter = 0;

void FOC_Task(void *argument)
{
    //FOC_State = FOC_Start;

    // Uq=4.50f;
    // sample_struct_init(&sample);
    Phase_Current_Sample_Start();
    VOFA_tx_frame_init(&FOC_frame,1);
    // smo_init(&observer,&motor_params);
    Three_Phase_Inverter_Start();
    disable_PWM_channel(0);
    disable_PWM_channel(1);
    disable_PWM_channel(2);
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_SET);
    
  for(;;)
  {
    vTaskDelay(1);
    // FOC_State = FOC_Start;
            //     HAL_GPIO_TogglePin(LIN1_GPIO_Port, LIN1_Pin);
            // HAL_GPIO_TogglePin(LIN2_GPIO_Port, LIN2_Pin);
            // HAL_GPIO_TogglePin(LIN3_GPIO_Port, LIN3_Pin);
    // BaseType_t uxBits;
    // uxBits  = osEventFlagsWait( ADC_event_Group,
    //                             0x01<<1,
    //                             osFlagsWaitAll|osFlagsNoClear,
    //                             portMAX_DELAY);
    // osEventFlagsClear( ADC_event_Group,
    //                   0x01<<1);
    
    
  }
}

void Timer_emergency(void)
{
    if(FOC_State == FOC_Stop)
    {
        disable_PWM_channel(0);
        disable_PWM_channel(1);
        disable_PWM_channel(2);
        HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_SET);
        
        // Three_Phase_Inverter_Stop();
    }
    // else if(FOC_State == FOC_Ready)
    // {
    //     sample_struct_update(&sample);
    //     float temp ;
    //     adc_offset[0] = adc_offset[0]*calibration_counter+sample.filtered_current[0];
    //     adc_offset[1] = adc_offset[1]*calibration_counter+sample.filtered_current[1];
    //     adc_offset[2] = adc_offset[2]*calibration_counter+sample.filtered_current[2];
    //     calibration_counter++;
    //     temp = adc_offset[0]/calibration_counter;
    //     adc_offset[0] = temp;
    //     temp = adc_offset[1]/calibration_counter;
    //     adc_offset[1] = temp;
    //     temp = adc_offset[2]/calibration_counter;
    //     adc_offset[2] = temp;
    // }
    else if(FOC_State == FOC_Start)
    {
        
        change_counter++;
        if(change_counter==25)
        {
            six_step_change_phase(&phase, direction);
            six_step_set_pulse(phase, direction);
            change_counter = 0;
            FOC_frame.float_buf[0] = phase;
            VOFA_tx_frame_update(&FOC_frame);
            uint8_t *u8ptr = VOFA_tx_frame_get_u8ptr(&FOC_frame);
            uint32_t u8num = VOFA_tx_frame_get_u8_num(&FOC_frame);
            HAL_UART_Transmit_DMA(&huart1,u8ptr,u8num);
        }
#if 0
        {
            count++;
            if(count==3000)
            {
                count =0;
                for(int i=0;i<1000;i++)
                {
                    FOC_frame.float_buf[0] = sample.sample_current_slide_window[0][i];
                    FOC_frame.float_buf[1] = sample.sample_current_slide_window[1][i];
                    FOC_frame.float_buf[2] = sample.sample_current_slide_window[2][i];
                    VOFA_tx_frame_update(&FOC_frame);
                    uint8_t *u8ptr = VOFA_tx_frame_get_u8ptr(&FOC_frame);
                    uint32_t u8num = VOFA_tx_frame_get_u8_num(&FOC_frame);
                    HAL_UART_Transmit(&huart1,u8ptr,u8num,100);
                    vTaskDelay(10);
                }
            }
        }
    #endif
    }
}