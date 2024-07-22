#include "ev3.h"
#include "Color.h"
#include "Odometry.h"

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1)
#define RIGHT_EDGE        (1)

/* 走行基準スピード */

/* ステアリング操舵量の係数 */
#define KP          (0.83)
#define KI         (0.83)
#define KD         (0.83)

enum Mode {
  Straight,
  Curve,
  End
};
enum Mode mode;

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
  float cur_diff;
  float pre_diff;
  float integral;
  int base_speed = 40;/* 走行基準スピード */
  double Kp = 0.83;
  int bias = 0;

  const int8_t mThreshold = 20;
/*
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
*/
};
