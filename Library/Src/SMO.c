#include "SMO.h"

#include <math.h>

#define M_PI 3.14159265358979323846
inline static float sign_smo(float x,float deadzone)
{
    if(x>deadzone)
    {
        return 1;
    }
    else if(x<-deadzone)
    {
        return -1;
    }
    else
    {
        return x;
    }
}

static inline float normalize_angle(float angle) 
{
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

void smo_init(SMO_observer_t *observer,const Motor_Params_t* params)
{
    observer->k_slide = SMO_K_SLIDE;
    observer->k_lpf = SMO_K_LPF;

    observer->i_alpha_hat = 0.0f;
    observer->i_beta_hat = 0.0f;

    observer->s_alpha = 0.0f;
    observer->s_beta = 0.0f;

    observer->z_alpha = 0.0f;
    observer->z_beta = 0.0f;

    observer->e_alpha = 0.0f;
    observer->e_beta = 0.0f;

    observer->e_alpha_lpf = 0.0f;
    observer->e_beta_lpf = 0.0f;

    observer->theta_est = 0.0f;
    observer->omega_est = 0.0f;
}
void smo_update(SMO_observer_t *observer,
    Motor_Params_t *params,
    float u_alpha,float u_beta,
    float i_alpha,float i_beta)
{
    static float last_etheta_est = 0.0f;
    observer->s_alpha = observer->i_alpha_hat - i_alpha;
    observer->s_beta = observer->i_beta_hat - i_beta;

    // observer->z_alpha = observer->k_slide*sign_smo(observer->s_aplpha,SMO_SIGN_DEADZONE);
    // observer->z_beta = observer->k_slide*sign_smo(observer->s_beta,SMO_SIGN_DEADZONE);

    observer->i_alpha_hat +=params->Ts*
    (   u_alpha
        -params->Rs*observer->i_alpha_hat
        -observer->e_alpha_lpf
        -observer->k_slide*observer->s_alpha*params->Ls)
        /params->Ls;

    observer->i_beta_hat +=params->Ts*
    (   u_beta
        -params->Rs*observer->i_beta_hat
        -observer->e_beta_lpf
        -observer->k_slide*observer
        ->s_beta*params->Ls)
        /params->Ls;

    // observer->e_alpha = observer->z_alpha;
    // observer->e_beta = observer->z_beta;
    float e_alpha = observer->e_alpha_lpf;
    float e_beta = observer->e_beta_lpf;

    observer->e_alpha_lpf += 
    (-observer->omega_est*e_beta
    +observer->k_lpf*observer->s_beta)*params->Ts;
    observer->e_beta_lpf +=
    (observer->omega_est*e_alpha 
    +observer->k_lpf*observer->s_alpha)*params->Ts;

    observer->theta_est = atan2f(-observer->e_alpha_lpf,observer->e_beta_lpf);
    
    if((observer->theta_est-last_etheta_est)>3.1415926)
    {
        last_etheta_est += 6.283185307f;
    }
    else if((observer->theta_est-last_etheta_est)<-3.1415926)
    {
        last_etheta_est -= 6.283185307f;
    }

    observer->omega_est = observer->theta_est-last_etheta_est;

    last_etheta_est = observer->theta_est;
}
