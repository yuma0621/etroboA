#include "app.h"
#include <stdio.h>
#include "Tracer.hpp"
#include "Color.hpp"
#include "Motor.hpp"

using namespace ev3api;

Tracer tracer;
Color color;
Motor motor;

void main_task(intptr_t unused) { // <1>

 main_init();  　　　/*初期設定*/
 wait_start();                   /* 動作開始待ち(シミュレータ:自動で開始, 実機:BlueToothボタンで開始) */
 calibration();    /* キャリブレーション */
    p_tracer();     /*P制御ライントレース*/

                                   　　/* デブリリムーバル */
                                   　　/* スマートキャリー */
                                   　　/* 停車処理 */

 ext_tsk();     　 /* メインタスク終了 */
}

void main_init(){
/* グローバル変数の初期化 */

  /* センサー入力ポートの設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);

    /* モーター出力ポートの設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
}

void wait_start(){
    while(true){
        bool_t pressed_flg;
        pressed_flg = ev3_touch_sensor_is_pressed(touch_sensor);
        if(pressed_flg){
            printf("[Main Task] Simulator is ready\n");
            break;
        }
        wait_msec(50);
    }
}

void calibration(){
 printf("[Main Task] Calibration Start\n");
    /* キャリブレーションタスク起動 */
    sta_cyc(CALIBRATION_TASK_CYC);
    /* 右旋回 */
    motor.motor_control(10, -10);
    wait_msec(1500);
    /* 一時停止 */
    motor.motor_control(0, 0);
    wait_msec(1000);
    /* 前進 */
    motor.motor_control(5, 5);
    wait_msec(1000);
    /* 後退 */
    motor.motor_control(-5, -5);
    wait_msec(4000);
    /* 前進 */
    motor.motor_control(5, 5);
    wait_msec(2500);
    /* 左旋回 */
    motor.motor_control(-10, 10);
    wait_msec(1500);
    /* 一時停止 */
    motor.motor_control(0, 0);
    /* キャリブレーションタスク停止 */
    stp_cyc(CALIBRATION_TASK_CYC);
    /* キャリブレーション終了 */
    printf("[Main Task] Calibration Finished\n");
    printf("[Main Task] min:%d, max:%d, ave:%d\n",rgb_min.g, rgb_max.g, rgb_ave.g);
    wait_msec(1000);

    /* (デモ用)逆走にする */
    motor.motor_control(-10, 10);
    wait_msec(3200);
    motor.motor_control(0, 0);
    wait_msec(1000);
}

void p_tracer(){

 /*P制御ライントレースタスク*/
  const uint32_t duration = 100*1000; // <2>
     //tracer.init();
     sta_cyc(TRACER_CYC); // <4>

   while (!ev3_button_is_pressed(LEFT_BUTTON)) { // <1>
      clock.sleep(duration);
   }

   stp_cyc(TRACER_CYC);
   tracer.terminate();
   ext_tsk();
}

/* タスク停止関数(ミリ秒) */
void wait_msec(int32_t msec){
    dly_tsk(msec * 1000 / 0.6);
}
