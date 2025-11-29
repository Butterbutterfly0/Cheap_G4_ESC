#include "FOC_Portable.h"

#include"main.h"  

FOC_State_t FOC_State = FOC_Stop;

#include "tim.h"

void Three_Phase_Inverter_Start(void)
{
    HAL_TIM_Base_Start(&htim1);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void Three_Phase_Inverter_Stop(void)
{
    HAL_TIM_Base_Stop(&htim1);

    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

#include "adc.h"

void Phase_Current_Sample_Start(void)
{
    HAL_ADCEx_InjectedStart_IT(&hadc1);
}

void Phase_Current_Sample_Stop(void)
{
    HAL_ADCEx_InjectedStop_IT(&hadc1);
}

#include "gpio.h"

void LIVE_Light_On(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET);
}

void LIVE_Light_Off(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);
}