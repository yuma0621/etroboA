#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"
//using namespace ev3api;
/* 円周率 */
#define PI 3.14159265358
#define TREAD 127.0
#define TIRE_DIAMETER 100.0

//public:
  void odom_init(); //コンストラクタ

  void odom_terminate();
  void motor_control(int left_motor_power, int right_motor_power);
  void odom_Distance_reset();
  static void odom_Distance_update();
  float odom_Distance_getDistance();
  static float odom_Distance_getDistanceRight();
  static float odom_Distance_getDistanceLeft();
  void odom_Direction_reset();
  float odom_Direction_getDirection();
  static void odom_Direction_update();


/* モーターポートの定義 */
/*motor_port_t
    arm_motor,
    left_motor,
    right_motor;
*/

   static float distance;
   static float distanceR;
   static float distanceL;
   static float pre_angleL;
   static float pre_angleR;
   static float distance;
   static float direction; //現在の方位
   //extern float angle_diff;

//private:

/*
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
*/

#ifdef __cplusplus
}
#endif
