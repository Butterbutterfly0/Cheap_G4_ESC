#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "FOC.h"
#include "OS_Support.h"
#include "SMO.h"

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

void FOC_Task(void *argument)
{
    Uq=4.50f;
    sample_struct_init(&sample);
    Phase_Current_Sample_Start();
    VOFA_tx_frame_init(&FOC_frame,8);
    smo_init(&observer,&motor_params);
  for(;;)
  {
    vTaskDelay(1);
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
        calibration_counter = 0;
    }
    else if(FOC_State == FOC_Ready)
    {
        sample_struct_update(&sample);
        float temp ;
        adc_offset[0] = adc_offset[0]*calibration_counter+sample.filtered_current[0];
        adc_offset[1] = adc_offset[1]*calibration_counter+sample.filtered_current[1];
        adc_offset[2] = adc_offset[2]*calibration_counter+sample.filtered_current[2];
        calibration_counter++;
        temp = adc_offset[0]/calibration_counter;
        adc_offset[0] = temp;
        temp = adc_offset[1]/calibration_counter;
        adc_offset[1] = temp;
        temp = adc_offset[2]/calibration_counter;
        adc_offset[2] = temp;
    }
    else if(FOC_State == FOC_Start)
    {
        sample_struct_update(&sample);
        Ia_meas =((float)sample.sample_current[0]-adc_offset[0])*ADC_ecd2V*Current_gain;
        Ib_meas =((float)sample.sample_current[1]-adc_offset[1])*ADC_ecd2V*Current_gain;
        Ic_meas =((float)sample.sample_current[2]-adc_offset[2])*ADC_ecd2V*Current_gain;
        Clark_Transform(Ia_meas,Ib_meas,Ic_meas,&Ialpha_meas,&Ibeta_meas);
        Park_Transform(Ialpha_meas,Ibeta_meas,etheta,&Id_meas,&Iq_meas);
        smo_update(&observer,&motor_params,Ualpha,Ubeta,Ialpha_meas,Ibeta_meas);
        Anti_Park_Transform(Ud,Uq,etheta,&Ualpha,&Ubeta);
        SVPWM(Ualpha,Ubeta,VBarbus,PWM_cmp,PWM_cmp+1,PWM_cmp+2);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM_cmp[0]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, PWM_cmp[1]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, PWM_cmp[2]*8000);
        
        // FOC_frame.float_buf[0] = Ia_meas;
        // FOC_frame.float_buf[1] = Ib_meas;
        // FOC_frame.float_buf[2] = Ic_meas;
        // FOC_frame.float_buf[3] = observer.theta_est;
        FOC_frame.float_buf[0] = Ialpha_meas;
        FOC_frame.float_buf[1] = Ibeta_meas;
        FOC_frame.float_buf[2] = observer.i_alpha_hat;
        FOC_frame.float_buf[3] = observer.i_beta_hat;
        FOC_frame.float_buf[4] = observer.e_alpha_lpf;
        FOC_frame.float_buf[5] = observer.e_beta_lpf;
        FOC_frame.float_buf[6] = observer.theta_est;
        FOC_frame.float_buf[7] = observer.omega_est;
        VOFA_tx_frame_update(&FOC_frame);
        uint8_t *u8ptr = VOFA_tx_frame_get_u8ptr(&FOC_frame);
        uint32_t u8num = VOFA_tx_frame_get_u8_num(&FOC_frame);
        HAL_UART_Transmit_DMA(&huart1,u8ptr,u8num);
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

        {
            etheta -= 0.3000f;
            if(etheta>6.28f)
                etheta =0.0f;
            else if(etheta<-0.0f)
                etheta = 6.28f;
        }
    }
}