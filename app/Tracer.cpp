#include "app.h"
#include "Tracer.hpp"

//using namespace ev3api;

Tracer::Tracer()
{
  mode = Straight;
}

void Tracer::terminate() {
    motor.motor_control(0, 0);
}

int16_t Tracer::P_steering_amount_calc() {
  float p;

   cur_diff =color.get_rgb_diff();//目標輝度値とカラーセンサ値の差分を計算
   p = KP * cur_diff;

   return (p + bias);
}

int16_t PID_steering_amount_calc(){
 float p, i, d;

 pre_diff = cur_diff;
 cur_diff = color.get_rgb_diff();
 integral += (cur_diff + pre_diff) / 2.0 * TRACER_PERIOD;

 p = KP * cur_diff;
 i = KI * integral;
 d = KD * (cur_diff - pre_diff)/TRACER_PERIOD;

 return (p + i + d);
}

void Tracer::trace_motor_control(int16_t rotation) {

  int left_motor_power, right_motor_power; /*左右モータ設定パワー*/

    left_motor_power  = (int)(base_speed + (rotation * LEFT_EDGE));
    right_motor_power = (int)(base_speed - (rotation * LEFT_EDGE));

  motor.motor_control(left_motor_power, right_motor_power);

  return;
}

Tracer tracer;

void tracer_task(intptr_t exinf){
//if(青を検知する関数=true){
 //mode = End;
   //}
 //else{
   switch(mode){
          case Straight://0
              float rotation = tracer.P_steering_amount_calc(); //P制御ステアリング操舵量補正値の計算 
     if(motor.Distance_getDistance()<target_distance){
                  mode = Curve;
                 }
              break;
          case Curve://1
     float rotation = tracer.PID_steering_amount_calc(); //PID制御ステアリング操舵量補正値の計算
                    if(motor.Distance_getDiraction()<target_diraction){
                 mode = Straight;
              }
              break;
    case End://2
     break;
          default:
              break;
    }
   motor_control(rotation);
 //}

 ext_tsk();
}
