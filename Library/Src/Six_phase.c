#include "Six_phase.h"

#include "main.h"
#include "tim.h"
#include "stm32g4xx_hal_gpio_ex.h"

typedef struct {
    uint32_t channel;
    uint32_t pin;
    uint32_t alternate;
} PWM_Channel;

static PWM_Channel channels[] = {
    {TIM_CHANNEL_1, GPIO_PIN_8, GPIO_AF6_TIM1},
    {TIM_CHANNEL_2, GPIO_PIN_9, GPIO_AF6_TIM1},
    {TIM_CHANNEL_3, GPIO_PIN_10, GPIO_AF6_TIM1}
};

// 配置指定通道
void enable_PWM_channel(uint32_t ch_index) {
    // 配置GPIO为复用功能
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = channels[ch_index].pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = channels[ch_index].alternate;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 使能定时器通道
    switch(channels[ch_index].channel) {
        case TIM_CHANNEL_1:
            TIM1->CCER |= TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIM1->CCER |= TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIM1->CCER |= TIM_CCER_CC3E;
            break;
    }
}

// 禁用指定通道
void disable_PWM_channel(uint32_t ch_index) {
    // 禁用定时器通道
    switch(channels[ch_index].channel) {
        case TIM_CHANNEL_1:
            TIM1->CCER &= ~TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIM1->CCER &= ~TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIM1->CCER &= ~TIM_CCER_CC3E;
            break;
    }
    
    // 等待寄存器写入完成
    __DSB();
    
    // 重配置GPIO为输出低电平
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = channels[ch_index].pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, channels[ch_index].pin, GPIO_PIN_RESET);
}

#define CHANNEL_1_Start {enable_PWM_channel(0);}
#define CHANNEL_2_Start {enable_PWM_channel(1);}
#define CHANNEL_3_Start {enable_PWM_channel(2);}
#define CHANNEL_1_Stop {disable_PWM_channel(0);}
#define CHANNEL_2_Stop {disable_PWM_channel(1);}
#define CHANNEL_3_Stop {disable_PWM_channel(2);}
static void Step_to_1(void)
{
    //->1
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_RESET);
   CHANNEL_1_Stop;

    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP);

    CHANNEL_2_Stop;

    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_RESET);

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_SET);
    CHANNEL_3_Start;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP);
}

static void Step_to_2(void)
{
    //->2
    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_RESET);
    
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP_CMPLE);
    
    CHANNEL_2_Stop;

    
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_RESET);
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    CHANNEL_3_Stop;

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_SET);
    CHANNEL_1_Start;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);

}

static void Step_to_3(void)
{
    //->3
   
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_RESET);
    
    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_RESET);
    CHANNEL_2_Stop;

    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);
    
    CHANNEL_3_Stop;

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_SET);
    CHANNEL_1_Start;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);
}

static void Step_to_4(void)
{
    //->4
                
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_RESET);
    CHANNEL_1_Stop;

    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_RESET);
   

    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);
    
    CHANNEL_3_Stop;

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_SET);
    CHANNEL_2_Start;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);
}

static void Step_to_5(void)
{
    //->5
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
    CHANNEL_1_Stop;
    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_RESET);
    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_RESET);
    CHANNEL_3_Stop;

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_SET);
    CHANNEL_2_Start;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP_CMPLE);
}
static void Step_to_6(void)
{
    //->6
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
   
    CHANNEL_1_Stop;


    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);

    HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_RESET);
    CHANNEL_2_Stop;

    
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_RESET);

    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_SET);
   CHANNEL_3_Start;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, SAFE_CMP_CMPLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, SAFE_CMP);
    }

void six_step_set_pulse(six_step_change_phase_t phase, six_step_direction_t direction)
{
    // HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    // HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    // HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

    // HAL_GPIO_WritePin(LIN1_GPIO_Port, LIN1_Pin, GPIO_PIN_RESET);
    // HAL_GPIO_WritePin(LIN2_GPIO_Port, LIN2_Pin, GPIO_PIN_RESET);    
    // HAL_GPIO_WritePin(LIN3_GPIO_Port, LIN3_Pin, GPIO_PIN_RESET);
    TIM1->CR1 &= ~TIM_CR1_CEN;
    switch(phase)
    {
        case PHASE_1:
            if(direction == FORWARD)
            {
                Step_to_2();
            }
            else
            {
                Step_to_6();   
            }
            break;
        case PHASE_2:
            if(direction == FORWARD)
            {
                Step_to_3();
            }
            else
            {
                Step_to_1();
            }
            break;
        case PHASE_3:
            if(direction == FORWARD)
            {
                Step_to_4();
            }
            else
            {
                Step_to_2();
            }
            break;
        case PHASE_4:
            if(direction == FORWARD)
            {
                Step_to_5();
            }
            else
            {
                Step_to_3();
            }
            break;
        case PHASE_5:
            if(direction == FORWARD)
            {
                Step_to_6();
            }
            else
            {
                Step_to_4();
            }
            break;
        case PHASE_6:
            if(direction == FORWARD)
            {
                Step_to_1();
            }
            else
            {
                Step_to_5();
            }
            break;
    }
    TIM1->EGR |= TIM_EGR_UG;

    TIM1->CNT = 0;
    TIM1->CR1 |= TIM_CR1_CEN;
}


void six_step_change_phase(six_step_change_phase_t* phase, six_step_direction_t direction)
{
    switch(*phase)
    {
        case PHASE_1:
            if(direction == FORWARD)
            {
                *phase = PHASE_2;
            }
            else
            {
                *phase = PHASE_6;
            }
            break;
        case PHASE_2:
            if(direction == FORWARD)
            {
                *phase = PHASE_3;
            }
            else
            {
                *phase = PHASE_1;
            }
            break;
        case PHASE_3:
            if(direction == FORWARD)
            {
                *phase = PHASE_4;
            }
            else
            {
                *phase = PHASE_2;
            }
            break;
        case PHASE_4:
            if(direction == FORWARD)
            {
                *phase = PHASE_5;
            }
            else
            {
                *phase = PHASE_3;
            }
            break;
        case PHASE_5:
            if(direction == FORWARD)
            {
                *phase = PHASE_6;
            }
            else
            {
                *phase = PHASE_4;
            }
            break;
        case PHASE_6:
            if(direction == FORWARD)
            {
                *phase = PHASE_1;
            }
            else
            {
                *phase = PHASE_5;
            }
            break;
    }
}