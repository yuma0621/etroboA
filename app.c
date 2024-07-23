#include "app.h"
#include <stdio.h>
#include "Color.h"
#include "Odometry.h"
#include "Tracer.h"

//using namespace ev3api;
/*
Tracer tracer;
Color color;
Motor motor;
*/
void main_task(intptr_t unused) { // <1>
 //sta_cyc(LOG_CYC);
 main_init();     /*初期設定*/
 wait_start();    /* 動作開始待ち(シミュレータ:自動で開始, 実機:BlueToothボタンで開始) */
 calibration();    /* キャリブレーション */
 p_tracer();     /*P制御ライントレース*/

                                     /* デブリリムーバル */
                                     /* スマートキャリー */
                                     /* 停車処理 */
 //stp_cyc(LOG_CYC);
 ext_tsk();       /* メインタスク終了 */
}

/* キャリブレーションタスク(100msec周期) */
void calibration_task(intptr_t unused){
    rgb_raw_t rgb_val;
    /* 光センサからRGB値取得 */
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_val);
    /* 最小値の更新 */
    if(rgb_val.r < rgb_min.r){      /* red */
        rgb_min.r = rgb_val.r;
    }
    if(rgb_val.g < rgb_min.g){      /* green */
        rgb_min.g = rgb_val.g;
    }
    if(rgb_val.b < rgb_min.b){      /* blue */
        rgb_min.b = rgb_val.b;
    }
    /* 最大値の更新 */
    if(rgb_val.r > rgb_max.r){      /* red */
        rgb_max.r = rgb_val.r;
    }
    if(rgb_val.g > rgb_max.g){      /* green */
        rgb_max.g = rgb_val.g;
    }
    if(rgb_val.b > rgb_max.b){      /* blue */
        rgb_max.b = rgb_val.b;
    }
    /* 平均値(目標値)の計算 */
    rgb_ave.g = (rgb_min.g + rgb_max.g) / 2;
    printf("[Calibration Task] max:%5d, min:%5d, ave:%5d\n", rgb_max.g, rgb_min.g, rgb_ave.g);
    ext_tsk();/* タスク終了 */
}
enum Mode {
  Straight,
  Curve,
  End
};
 enum Mode trace_mode=Straight;

void tracer_task(intptr_t exinf){
 float rotation;
   switch(trace_mode){
          case Straight://0
              rotation = trace_Psteering_calc(); //P制御ステアリング操舵量補正値の計算 
              if(odom_Distance_getDistance()<target_distance){
                  trace_mode = Curve;
              }
              break;

          case Curve://1
              rotation = trace_PIDsteering_calc(); //PID制御ステアリング操舵量補正値の計算
              if(odom_Direction_getDirection()<target_direction){
                 trace_mode = Straight;
              }
              break;

          default:
            rotation = 0;
  }
  trace_motor_control(rotation);
ext_tsk();
}

void odometry_task(intptr_t exinf){
    odom_Distance_update();
    odom_Direction_update();
}

void main_init(){
 color_init(); 
 odom_init();
 trace_init();
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
    sta_cyc(CALIBRATION_CYC);
    /* 右旋回 */
    motor_control(10, -10);
    wait_msec(1500);
    /* 一時停止 */
    motor_control(0, 0);
    wait_msec(1000);
    /* 前進 */
    motor_control(5, 5);
    wait_msec(1000);
    /* 後退 */
    motor_control(-5, -5);
    wait_msec(4000);
    /* 前進 */
    motor_control(5, 5);
    wait_msec(2500);
    /* 左旋回 */
    motor_control(-10, 10);
    wait_msec(1500);
    /* 一時停止 */
    motor_control(0, 0);
    /* キャリブレーションタスク停止 */
    stp_cyc(CALIBRATION_CYC);
    /* キャリブレーション終了 */
    printf("[Main Task] Calibration Finished\n");
    printf("[Main Task] min:%d, max:%d, ave:%d\n", rgb_min.g, rgb_max.g, rgb_ave.g);
    wait_msec(1000);

    /* (デモ用)逆走にする */
    motor_control(-10, 10);
    wait_msec(3200);
    motor_control(0, 0);
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
   trace_terminate();
   ext_tsk();
}

/* タスク停止関数(ミリ秒) */
void wait_msec(int32_t msec){
    dly_tsk(msec * 1000 / 0.6);
}

/* タスク停止関数(ミリ秒) */
void wait_msec(int32_t msec){
    dly_tsk(msec * 1000 / 0.6);
}
