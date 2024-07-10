#include "Motor.hpp"
#include "ev3.h"

using namespace ev3api;

Motor::Motor(){
 /* モーター出力ポートの設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
}

void Motor::terminate() {
  msg_f("Stopped.", 1);
 ev3_motor_set_power(left_motor, 0);
    ev3_motor_set_power(right_motor, 0);
}

void Motor::motor_control(int left_motor_power, int right_motor_power) {    

 ev3_motor_set_power(left_motor, left_motor_power);
    ev3_motor_set_power(right_motor, right_motor_power);

 return;
}