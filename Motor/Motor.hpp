#include "ev3.h"
using namespace ev3api;
/* 円周率 */
#define PI 3.14159265358
#define TREAD 50
#define TIRE_DIAMETER 0.81

class appMotor{
public:
  Motor(); //コンストラクタ

  void terminate();
  void motor_control(int left_motor_power, int right_motor_power);
  void Distance_reset();
  void Distance_update();
  float Distance_getDistance();
  float Distance_getDistance4msRight();
  float Distance_getDistance4msLeft();
  void Direction_reset();
  float Direction_getDirection();
  void Direction_update();


/* モーターポートの定義 */
static const motor_port_t
    arm_motor       = EV3_PORT_A,
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B;

   static float distance;
   static float distance4msR;
   static float distance4msL;
   static float pre_angleL;
   static float pre_angleR;
   static float distance4ms;
   static float direction; //現在の方位

private:
  const int8_t mThreshold = 20;
/*
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
*/
};
