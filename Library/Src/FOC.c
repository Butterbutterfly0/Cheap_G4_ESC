#include "FOC.h"

void Clark_Transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta)// equal A
{
  *Ialpha = 0.6667f*Ia - 0.3333f*Ib - 0.3333f*Ic;
  *Ibeta = 0.5773*Ib - 0.5773f*Ic;
}

void Park_Transform(float Ualpha, float Ubeta, float theta, float *Ud, float *Uq)
{
	float sin_val = sinf(theta);
  float cos_val = cosf(theta);

  *Ud = Ualpha*cos_val + Ubeta*sin_val;
  *Uq = -Ualpha*sin_val + Ubeta*cos_val;
}
void Anti_Park_Transform(float Ud,float Uq, float theta, float *Ualpha,float *Ubeta)
{
	float sin_val = sinf(theta);
  float cos_val = cosf(theta);

  *Ualpha = Ud*cos_val - Uq*sin_val;
  *Ubeta = Ud*sin_val + Uq*cos_val;
}

void Anti_Clark_Transform(float Ualpha, float Ubeta, float *Ua, float *Ub, float *Uc)
{
  static const float cos_30 = 0.866;
  static const float sin_30 = 0.5;
  *Ua = Ualpha;
  *Ub = Ualpha*sin_30 + Ubeta*cos_30;
  *Uc = Ualpha*cos_30 - Ubeta*sin_30;
}

void SVPWM( float Ualpha, float Ubeta, float VBarbus, 
             
            float* PWMa_duty, float* PWMb_duty, float* PWMc_duty)
{
  const static float sqrt3 = 1.7320508075688772;

  float Ua, Ub, Uc;

  uint8_t which_section = 0;
  Ua=Ubeta;
  Ub=(sqrt3*Ualpha-Ubeta)/2;
  Uc=(-sqrt3*Ualpha-Ubeta)/2;

  uint8_t A=0,B=0,C=0;
  if(Ua>0.0f)
  A=1;
  if(Ub>0.0f)
  B=1;
  if(Uc>0.0f)
  C=1;
  which_section = A+2*B+4*C;

  switch(which_section)
  {
    case SECTOR_ONE:
    which_section = 1;break;
    case SECTOR_TWO:
    which_section = 2;break;
    case SECTOR_THREE:
    which_section = 3;break;
    case SECTOR_FOUR:
    which_section = 4;break;
    case SECTOR_FIVE:
    which_section = 5;break;
    case SECTOR_SIX:  
    which_section = 6;break;
  }

  float Vx=0.0f,Vy=0.0f,Vz=0.0f;
  
  Vx = sqrt3*Ubeta/VBarbus;

  Vy = 3*Ualpha+sqrt3*Ubeta;
  Vy = Vy/VBarbus/2;

  Vz = -3*Ualpha+sqrt3*Ubeta;
  Vz = Vz/VBarbus/2;

  float T0=0.0f,T1= 0.0f,T2 = 0.0f;
  switch(which_section)
  {
      case 1:
      T1 = -Vz;T2 = Vx;break;
      case 2:
      T1 = Vz;T2 = Vy;break;
      case 3:
      T1 = Vx;T2 = -Vy;break;
      case 4:
      T1 = -Vx;T2 = Vz;break;
      case 5:
      T1 = -Vy;T2 = -Vz;break;
      case 6:
      T1 = Vy;T2 = -Vx;break;
  }

  

  if(T1+T2>1.0f)
  {
    T0=0.0f;
    T1=T1/(T1+T2);
    T2=T2/(T1+T2);
  }
  else
  {
    T0=1.0f-T1-T2;
  }

  float Ta=0.0f,Tb=0.0f,Tc=0.0f;
  Ta = (1-T1-T2)/4;
  Tb = Ta + T1/2;
  Tc = Tb + T2/2;

  switch(which_section)
  {
    case 1:
    *PWMa_duty = Ta;
    *PWMb_duty = Tb;
    *PWMc_duty = Tc;
    break;
    case 2:
    *PWMa_duty = Tb;
    *PWMb_duty = Ta;
    *PWMc_duty = Tc;
    break;
    case 3:
    *PWMa_duty = Tc;
    *PWMb_duty = Ta;
    *PWMc_duty = Tb;
    break;
    case 4:
    *PWMa_duty = Tc;
    *PWMb_duty = Tb;
    *PWMc_duty = Ta;
    break;
    case 5:
    *PWMa_duty = Tb;
    *PWMb_duty = Tc;
    *PWMc_duty = Ta;
    break;
    case 6:
    *PWMa_duty = Ta;
    *PWMb_duty = Tc;
    *PWMc_duty = Tb;
    break;
  }


}