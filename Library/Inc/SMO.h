#ifndef SMO_H
#define SMO_H

typedef struct
{
    float Rs;
    float Ls;
    float psi_f;
    float Ts;
}
Motor_Params_t;
// 2804云台电机
//Rs = 2.55 ohm
//Ls = 0.86 mH
//Flux 0.0035Wb

#define SMO_K_SLIDE 7900
#define SMO_K_LPF 200

#define SMO_SIGN_DEADZONE 0.1f

#define MOTOR_2804_RS 2.55f
#define MOTOR_2804_LS 0.86f
#define MOTOR_2804_PSI_F 0.0035f

typedef struct
{
    float k_slide;
    float k_lpf;

    float i_alpha_hat;
    float i_beta_hat;

    float s_alpha;
    float s_beta;

    float z_alpha;
    float z_beta;

    float e_alpha;
    float e_beta;

    float e_alpha_lpf;
    float e_beta_lpf;

    float theta_est;
    float omega_est;
}
SMO_observer_t;

void smo_init(SMO_observer_t *observer,const Motor_Params_t* params);

void smo_update(SMO_observer_t *observer,
    Motor_Params_t *params,
    float u_alpha,float u_beta,
    float i_alpha,float i_beta);

#endif