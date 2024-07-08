#include "Motor.hpp"
#include "Color.hpp"
#include "util.h"

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1)
#define RIGHT_EDGE        (1)

/* 走行基準スピード */

/* ステアリング操舵量の係数 */
#define KP         (0.83)//←これ変数にした方がいい？

using namespace ev3api;

enum Mode {
  Straight,
  Curve
};

class Tracer {
public:
  Tracer(); //コンストラクタ

  extern void tracer_task(intptr_t exinf);
  void init();
  void terminate();
  set_parameters(int base_speed, double Kp, int bias);, base_speed, Kp, biasを変更する関数(未実装)
  motor_control();

private:
  int mode;
  int base_speed = 40;/* 走行基準スピード */
  double Kp = 0.83;
  int bias = 0;
  Motor motor;
  Color color;
  const int8_t mThreshold = 20;
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
 calc();
 int16_t P_steering_amount_calc();

};
