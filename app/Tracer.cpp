#include "app.h"
#include "Tracer.hpp"
#include "ev3.h"

using namespace ev3api;

Tracer::Tracer()
{
 mode = Straight;
}

void Tracer::init() {
  init_f("Tracer");
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
 ev3_motor_set_power(left_motor, 0);
    ev3_motor_set_power(right_motor, 0);
}

void Tracer::motor_control(int16_t rotation) {

 int left_motor_power, right_motor_power; /*左右モータ設定パワー*/    

    left_motor_power  = (int)(base_speed + (rotaion * LEFT_EDGE));
    right_motor_power = (int)(base_speed - (rotation * LEFT_EDGE));

 motor.motor_control(left_motor_power, right_motor_power);

 return;
}

int16_t Tracer::P_steering_amount_calc() {

    /* 目標輝度値とカラーセンサ値の差分を計算 */
    int16_t diff =color.get_rgb_diff();

  return (Kp * diff + bias);
}

void tracer_task(intptr_t exinf) { 

 switch(mode){
        case Straight:
            int16_t rotation = P_steering_amount_calc();/* ステアリング操舵量補正値の計算 *
   motor_control(rotation);
            break;
        case Curve:

            break;
        default:
            break;
    }
 //set_parameters(40, 0.83, 0);//mode, base_speed, Kp, bias
 ext_tsk();
}
