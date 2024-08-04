#pragma once

#include "ev3api.h"
//using namespace ev3api;
/* 円周率 */
#define PI 3.14159265358
#define TREAD 0.127
#define TIRE_DIAMETER 0.1

//public:
  void odom_init(); //コンストラクタ

  void odom_terminate();
  void motor_control(int left_motor_power, int right_motor_power);
  void odom_Distance_reset();
  void odom_Distance_update();
  float odom_Distance_getDistance();
  float odom_Distance_getDistanceRight();
  float odom_Distance_getDistanceLeft();
  void odom_Direction_reset();
  float odom_Direction_getDirection();
  void odom_Direction_update();


/* モーターポートの定義 */
/*motor_port_t
    arm_motor,
    left_motor,
    right_motor;
*/

   extern float distance;
   extern float distanceR;
   extern float distanceL;
   extern float pre_angleL;
   extern float pre_angleR;
   extern float distance;
   extern float direction; //現在の方位

//private:

/*
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
*/

