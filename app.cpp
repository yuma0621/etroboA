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

 main_init();     /*初期設定*/
 wait_start();    /* 動作開始待ち(シミュレータ:自動で開始, 実機:BlueToothボタンで開始) */
 calibration();    /* キャリブレーション */
 p_tracer();     /*P制御ライントレース*/

                                     /* デブリリムーバル */
                                     /* スマートキャリー */
                                     /* 停車処理 */

 ext_tsk();       /* メインタスク終了 */
}

/* キャリブレーションタスク(100msec周期) */
void calibration_task(intptr_t unused){
    rgb_raw_t rgb_val;
    /* 光センサからRGB値取得 */
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_val);
    /* 最小値の更新 */
    if(rgb_val.r < color.rgb_min.r){      /* red */
        color.rgb_min.r = rgb_val.r;
    }
    if(rgb_val.g < color.rgb_min.g){      /* green */
        color.rgb_min.g = rgb_val.g;
    }
    if(rgb_val.b < color.rgb_min.b){      /* blue */
        color.rgb_min.b = rgb_val.b;
    }
    /* 最大値の更新 */
    if(rgb_val.r > color.rgb_max.r){      /* red */
        color.rgb_max.r = rgb_val.r;
    }
    if(rgb_val.g > color.rgb_max.g){      /* green */
        color.rgb_max.g = rgb_val.g;
    }
    if(rgb_val.b > color.rgb_max.b){      /* blue */
        color.rgb_max.b = rgb_val.b;
    }
    /* 平均値(目標値)の計算 */
    color.rgb_ave.g = (color.rgb_min.g + color.rgb_max.g) / 2;
    printf("[Calibration Task] max:%5d, min:%5d, ave:%5d\n", color.rgb_max.g, color.rgb_min.g, color.rgb_ave.g);
    ext_tsk();/* タスク終了 */
}

void tracer_task(intptr_t exinf){
 float rotation;
   switch(mode){
          case Straight://0
              rotation = tracer.P_steering_amount_calc(); //P制御ステアリング操舵量補正値の計算 
              if(motor.Distance_getDistance()<tracer.target_distance){
                  mode = Curve;
              }
              break;

          case Curve://1
              rotation = tracer.PID_steering_amount_calc(); //PID制御ステアリング操舵量補正値の計算
              if(motor.Direction_getDirection()<tracer.target_direction){
                 mode = Straight;
              }
              break;

          default:
            rotation = 0;
  }
  tracer.trace_motor_control(rotation);
ext_tsk();
}

void odometry_task(intptr_t exinf){
    motor.Distance_update();
    motor.Direction_update();
}

void main_init(){
/* グローバル変数の初期化 */

  /* センサー入力ポートの設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);

    /* モーター出力ポートの設定 */
    ev3_motor_config(motor.arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(motor.left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(motor.right_motor   ,MEDIUM_MOTOR);
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
    sta_cyc(CALIBRATION_CYC);
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
    stp_cyc(CALIBRATION_CYC);
    /* キャリブレーション終了 */
    printf("[Main Task] Calibration Finished\n");
    printf("[Main Task] min:%d, max:%d, ave:%d\n", color.rgb_min.g, color.rgb_max.g, color.rgb_ave.g);
    wait_msec(1000);

    /* (デモ用)逆走にする */
    motor.motor_control(-10, 10);
    wait_msec(3200);
    motor.motor_control(0, 0);
    wait_msec(1000);
}

void p_tracer(){

 /*P制御ライントレースタスク*/
     //tracer.init();
     sta_cyc(TRACER_CYC);
     sta_cyc(ODOMETRY_CYC);

   while (!ev3_button_is_pressed(LEFT_BUTTON)) { // <1>
      wait_msec(100);
   }

   stp_cyc(TRACER_CYC);
   stp_cyc(ODOMETRY_CYC);
   tracer.terminate();
   ext_tsk();
}

/* タスク停止関数(ミリ秒) */
void wait_msec(int32_t msec){
    dly_tsk(msec * 1000 / 0.6);
}
