#pragma once

#include "ev3.h"
#include "Color.h"
#include "Odometry.h"

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1)
#define RIGHT_EDGE        (1)

/* 走行基準スピード */

/* ステアリング操舵量の係数 */
#define KP          (0.5)
#define KI         (0)
#define KD         (0.01)


extern void motor_control(int left_motor_power, int right_motor_power);
//public:
  void trace_init(); //コンストラクタ

  void trace_terminate();
  //void set_parameters(int base_speed, double Kp, int bias);base_speed, Kp, biasを変更する関数(未実装)
  float trace_Psteering_calc();
  float trace_PIDsteering_calc();
  void trace_motor_control(float rotation);

  float target_distance;
  float target_direction;

//private:
  int16_t cur_diff;
  int16_t pre_diff;
  float integral;
  int base_speed;/* 走行基準スピード */
  int bias;

  //const int8_t mThreshold = 20;
/*
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
*/
