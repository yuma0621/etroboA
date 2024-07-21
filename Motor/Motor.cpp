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

/* 初期化関数 */
void Motor::Distance_reset() {
    //各変数の値の初期化
    distance = 0.0;
    distance4msR = 0.0;
    distance4msL = 0.0;
    //モータ角度の過去値に現在値を代入
    pre_angleL = ev3_motor_get_counts(left_motor);
    pre_angleR = ev3_motor_get_counts(right_motor);
}

/* 距離更新(4ms間の移動距離を毎回加算している) */
void Motor::Distance_update(){
    float cur_angleL = ev3_motor_get_counts(left_motor); //左モータ回転角度の現在値
    float cur_angleR = ev3_motor_get_counts(right_motor);//右モータ回転角度の現在値
    float distance4ms = 0.0;        //4msの距離

    // 4ms間の走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値 - モータ角度現在値)
    distance4msL = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL);  // 4ms間の左モータ距離
    distance4msR = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR);  // 4ms間の右モータ距離
    distance4ms = (distance4msL + distance4msR) / 2.0; //左右タイヤの走行距離を足して割る
    distance += distance4ms;

    //モータの回転角度の過去値を更新
    pre_angleL = cur_angleL;
    pre_angleR = cur_angleR;
}

/* 走行距離を取得 */
float Motor::Distance_getDistance(){
    return distance;
}

/* 右タイヤの4ms間の距離を取得 */
float Motor::Distance_getDistance4msRight(){
    return distance4msR;
}

/* 左タイヤの4ms間の距離を取得 */
float Motor::Distance_getDistance4msLeft(){
    return distance4msL;
}

/* 方位リセット */
void Motor::Direction_reset(){
    direction = 0.0;
}

 /* 方位を取得(右旋回が正転) */
float Motor::Direction_getDirection(){
    return direction;
}

/* 方位を更新 */
void Motor::Direction_update(){
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
    direction += (360.0 / (2.0 * PI * TREAD)) * (Distance_getDistance4msLeft() - Distance_getDistance4msRight());
}

Motor motor;
void odmetory_task(intptr_t exinf){
 motor.Distance_update();
 motor.Direction_update();
}
