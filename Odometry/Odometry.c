#include "app.h"
#include "Odometry.h"
#include "ev3api.h"
//using namespace ev3api;

void odometry_task(intptr_t exinf){
    odom_Distance_update();
    odom_Direction_update();
    printf(", distance = %lf, direction= %lf\n", distance, direction);
}
/*
void odom_init(){
    arm_motor       = EV3_PORT_A;
    left_motor      = EV3_PORT_C;
    right_motor     = EV3_PORT_B;
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
}
*/
void odom_terminate(){
   ev3_motor_set_power(left_motor, 0);
   ev3_motor_set_power(right_motor, 0);
}

void motor_control(int left_motor_power, int right_motor_power) {    
   ev3_motor_set_power(left_motor, left_motor_power);
   ev3_motor_set_power(right_motor, right_motor_power);
   return;
}

/* 初期化関数 */
void odom_Distance_reset(){
    //各変数の値の初期化
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    distance = 0.0;
    distanceR = 0.0;
    distanceL = 0.0;

    /*
    //モータ角度の過去値に現在値を代入
    //pre_angleL = ev3_motor_get_counts(left_motor);
    //pre_angleR = ev3_motor_get_counts(right_motor);
    */
}

/* 距離更新(4ms間の移動距離を毎回加算している) */
void odom_Distance_update(){
    cur_angleL = ev3_motor_get_counts(left_motor); //左モータ回転角度の現在値
    cur_angleR = ev3_motor_get_counts(right_motor);//右モータ回転角度の現在値
    printf("cur_angleL = %lf, cur_angleR = %lf, pre_angleL = %lf, pre_angleR = %lf", cur_angleL, cur_angleR, pre_angleL, pre_angleR);
    float distance_dt = 0.0;        //

    // 4ms間の走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値 - モータ角度現在値)
    distanceL = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL);  // 左モータ距離
    distanceR = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR);  // 右モータ距離
    distance_dt = (distanceL + distanceR) / 2.0; //左右タイヤの走行距離を足して割る
    distance += distance_dt;

    //モータの回転角度の過去値を更新
    pre_angleL = cur_angleL;
    pre_angleR = cur_angleR;
}

/* 走行距離を取得 */
float odom_Distance_getDistance(){
    return distance;
}

/* 右タイヤの4ms間の距離を取得 */
float odom_Distance_getDistanceRight(){
    return distanceR;
}

/* 左タイヤの4ms間の距離を取得 */
float odom_Distance_getDistanceLeft(){
    return distanceL;
}

/* 方位リセット */
void odom_Direction_reset(){
    direction = 0.0;
    cur_angleR = 0.0;
    cur_angleL = 0.0;
    cur_angleR = 0.0;
    cur_angleL = 0.0;
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
}

 /* 方位を取得(右旋回が正転) */
float odom_Direction_getDirection(){
    return direction;
}

/* 方位を更新 */
void odom_Direction_update(){
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
    direction += (360.0 / (2.0 * PI * TREAD)) * (odom_Distance_getDistanceLeft() - odom_Distance_getDistanceRight());
}

/* 方位を設定 */
void odom_Direction_setDirection(float set_dir){
    direction = set_dir;
}
