#include "pid.h"
class SM
{
  private:
    int dir_link1, pwm_link1, dir_link2, pwm_link2, dir_swivel, pwm_swivel, dir_pump, pwm_pump;
    int pwmswl, dirswl, pwm1, dir1, pwm2, dir2, pwm_pp, dir_pp;
    int freq, L1channel, L2channel, Swchannel, Tchannel, resolution;
    // PID class instances
    PID arm_pid[3]; //swl,L1,L2

  public:
    SM()
    {
      freq = 8000;
      L1channel = 2;
      L2channel = 3;
      Swchannel = 4;
      Tchannel = 5;
      resolution = 8;
    }

    void setPins( int dir_sw, int pwm_sw, int dir_1, int pwm_1, int dir_2, int pwm_2, int dir_pp, int pwm_pp)
    {
      dir_link1 = dir_1;
      pwm_link1 = pwm_1;
      dir_link2 = dir_2;
      pwm_link2 = pwm_2;
      dir_swivel = dir_sw;
      pwm_swivel = pwm_sw;
      dir_pump = dir_pp;
      pwm_pump = pwm_pp;

      ledcSetup(L1channel, freq, resolution);
      ledcSetup(L2channel, freq, resolution);
      ledcSetup(Swchannel, freq, resolution);
      ledcSetup(Tchannel, freq, resolution);
      ledcAttachPin(pwm_link1, L1channel);
      ledcAttachPin(pwm_link2, L2channel);
      ledcAttachPin(pwm_swivel, Swchannel);
      ledcAttachPin(pwm_pump, Tchannel);
      pinMode(dir_link1, OUTPUT);
      pinMode(dir_link2, OUTPUT);
      pinMode(dir_swivel, OUTPUT);
      pinMode(dir_pump, OUTPUT);
    }



    void beginSM(int X, int Y, int Z, int P)
    {
  if (Y < 0)
    {
      digitalWrite(dir_swivel, LOW);
      int s = map(abs(Y), 0, 11, 0, 255);
      ledcWrite(Swchannel, s);
    }
    else if (Y > 0)
    {
      digitalWrite(dir_swivel, HIGH);
      int s = map(abs(Y), 0, 11, 0, 255);
      ledcWrite(Swchannel, s);
    }
    else
    {
      digitalWrite(dir_swivel, LOW);
      ledcWrite(Swchannel, 0);
    }
    if (X < 0)
    {
      digitalWrite(dir_link1, HIGH);
      int l1 = map(abs(X), 0, 11, 0, 255);
      ledcWrite(L1channel, l1);
    }
    else if (X > 0)
    {
      digitalWrite(dir_link1, LOW);
      int l1 = map(abs(X), 0, 11, 0, 255);
      ledcWrite(L1channel, l1);
    }
    else
    {
      digitalWrite(dir_link1, LOW);
      ledcWrite(L1channel, 0);
    }
    if (Z < 0)
    {
      digitalWrite(dir_link2, LOW);
      int l2 = map(abs(Z), 0, 11, 0, 255);
      ledcWrite(L2channel, l2);
    }
    else if (Z > 0)
    { 
      digitalWrite(dir_link2, HIGH);
      int l2 = map(abs(Z), 0, 11, 0, 255);
      ledcWrite(L2channel, l2);
    }
    else
    {
      digitalWrite(dir_link2, LOW);
      ledcWrite(L2channel, 0);
    }
    if (P == 7)
    {
      digitalWrite(dir_pump, HIGH);
      ledcWrite(Tchannel, 120);
    }
    else if(P == 8)
    {
      digitalWrite(dir_pump, LOW);
      ledcWrite(Tchannel, 120);
    }
    else
    {
      digitalWrite(dir_pump, LOW);
      ledcWrite(Tchannel, 0);
    }
  
    }
  void Stop(void)
    {
      ledcWrite(Swchannel, 0);
      ledcWrite(L1channel, 0);
      ledcWrite(L2channel, 0);
      ledcWrite(Tchannel, 0);
    }
};