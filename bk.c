#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FOC_Portable.h"
#include "FOC.h"
#include "OS_Support.h"

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

float Id_ref = 0.0f,Iq_ref = 0.0f;
/**************/
#define Kp 0.1
#define Ki 0.1
#define max_u 2.0f
float ud_out = 0.0f,uq_out = 0.0f;
/**************/
const float ADC_ecd2V = 3.3f/4096.0f;
const float Current_gain = 2.0f;

sample_struct_t sample;

VOFA_tx_frame_t FOC_frame;
uint32_t count =0;

uint32_t adc_offset[3] = {0.0f,0.0f,0.0f};
uint32_t calibration_counter = 0;

typedef struct
{
    float kp;
    float ki;

    float error;
    float integral;

    float max;
    float output;
}
pid_t;

pid_t Id_pid=
{.error=0.0f,
.integral=0.0f,
.kp=Kp,
.ki=Ki,
.max=max_u,
.output=0.0f
},
Iq_pid=
{.error=0.0f,
.integral=0.0f,
.kp=Kp,
.ki=Ki,
.max=max_u,
.output=0.0f
};
float pid_calc(pid_t *pid,float feedback,float target)
{
    pid->error = target-feedback;
    pid->integral = pid->integral+pid->error;
    pid->output = pid->kp*pid->error + pid->ki*pid->integral;
    if(pid->output>pid->max)
        pid->output = pid->max;
    else if(pid->output<-pid->max)
        pid->output = -pid->max;
    return pid->output;
}

void FOC_Task(void *argument)
{
    Uq=1.0f;
    // Iq_ref = 6.0f;
    sample_struct_init(&sample);
    Phase_Current_Sample_Start();
    VOFA_tx_frame_init(&FOC_frame,4);
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
        Id_pid.integral = 0.0f;
        Iq_pid.integral = 0.0f;
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
        // sample_struct_update(&sample);
        // Clark_Transform(Ia_meas,Ib_meas,Ic_meas,&Ialpha_meas,&Ibeta_meas);
        // Park_Transform(Ialpha_meas,Ibeta_meas,etheta,&Id_meas,&Iq_meas);
        // Ud= pid_calc(&Id_pid,Id_meas,Id_ref);
        // Uq= pid_calc(&Iq_pid,Iq_meas,Iq_ref);
        // float temp= Ud*Ud+Uq*Uq;
        // if(temp>2.0f)
        // {
        //     Ud = Ud/temp;
        //     Uq = Uq/temp;
        // }
        Anti_Park_Transform(Ud,Uq,etheta,&Ualpha,&Ubeta);
        SVPWM(Ualpha,Ubeta,VBarbus,PWM_cmp,PWM_cmp+1,PWM_cmp+2);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM_cmp[0]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, PWM_cmp[1]*8000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, PWM_cmp[2]*8000);
        // Ia_meas =((float)sample.sample_current[0]-adc_offset[0])*ADC_ecd2V*Current_gain;
        // Ib_meas =((float)sample.sample_current[1]-adc_offset[1])*ADC_ecd2V*Current_gain;
        // Ic_meas =((float)sample.sample_current[2]-adc_offset[2])*ADC_ecd2V*Current_gain;
        // FOC_frame.float_buf[0] = PWM_cmp[0];
        // FOC_frame.float_buf[1] = PWM_cmp[1];
        // FOC_frame.float_buf[2] = PWM_cmp[2];
        // // FOC_frame.float_buf[3] = Uq;
        // VOFA_tx_frame_update(&FOC_frame);
        // uint8_t *u8ptr = VOFA_tx_frame_get_u8ptr(&FOC_frame);
        // uint32_t u8num = VOFA_tx_frame_get_u8_num(&FOC_frame);
        // HAL_UART_Transmit_DMA(&huart1,u8ptr,u8num);
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
            etheta += 0.1000f;
            if(etheta>6.28f)
                etheta =0.0f;
            else if(etheta<-0.0f)
                etheta = 6.28f;
        }
    }
}