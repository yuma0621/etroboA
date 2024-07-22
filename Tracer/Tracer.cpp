#include "Tracer.hpp"

//using namespace ev3api;

Tracer::Tracer()
{
  mode = Straight;
  target_distance = 4000;//mm
  target_direction =90;//deg

}

void Tracer::terminate(){
    motor.motor_control(0, 0);
}

float Tracer::P_steering_amount_calc() {
  float p;

   cur_diff =color.get_rgb_diff();//目標輝度値とカラーセンサ値の差分を計算
   p = KP * cur_diff;

   return (p + bias);
}

float Tracer::PID_steering_amount_calc(){
 float p, i, d;

 pre_diff = cur_diff;
 cur_diff = color.get_rgb_diff();
 integral += (cur_diff + pre_diff) / 2.0 * TRACER_PERIOD;

 p = KP * cur_diff;
 i = KI * integral;
 d = KD * (cur_diff - pre_diff)/TRACER_PERIOD;

 return (p + i + d);
}

void Tracer::trace_motor_control(float rotation) {

  int left_motor_power, right_motor_power; /*左右モータ設定パワー*/

    left_motor_power  = (int)(base_speed + (rotation * LEFT_EDGE));
    right_motor_power = (int)(base_speed - (rotation * LEFT_EDGE));

  motor.motor_control(left_motor_power, right_motor_power);

  return;
}
