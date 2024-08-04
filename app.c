#include <stdio.h>
#include "app.h"
#include "Calibration.h"

/* グローバル変数 */
int16_t s_min;          /* 彩度Sの平均値（目標値） */
int16_t s_max;          /* 彩度Sの平均値（目標値） */
int16_t s_ave;          /* 彩度Sの平均値（目標値） */
int16_t v_min;          /* 明度Vの平均値（目標値） */
int16_t v_max;          /* 明度Vの平均値（目標値） */
int16_t v_ave;          /* 明度Vの平均値（目標値） */

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused){
    init_global();                  /* グローバル変数の初期化 */
    port_config();                  /* ポート初期設定 */
    wait_start();                   /* 動作開始待ち（シミュレータ：自動またはスペースキーで開始, 実機：BlueToothボタンで開始） */
    s_calibration();                /* S値キャリブレーション（青線） */
    v_calibration();                /* V値キャリブレーション（黒線） */
    laptime_tracer();               /* ラップタイム走行 */
                                    /* タブルループNEO */
                                    /* スマートデブリ */
                                    /* スマートキャリー */
                                    /* 停車処理 */
    ext_tsk();                      /* メインタスク終了 */
}

/* グローバル変数の初期化 */
static void init_global(){
    s_min = 255;
    s_max = 0;
    s_ave = 90;
    v_min = 255;
    v_max = 0;
    v_ave = 50;
}

/* ポート初期設定 */
static void port_config(){
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

/* 動作開始待ち（シミュレータ：自動またはスペースキーで開始, 実機：BlueToothボタンで開始） */
void wait_start(){
    while(true){
        bool_t pressed_flg;
        pressed_flg = ev3_touch_sensor_is_pressed(touch_sensor);
        if(pressed_flg){
            printf("Start Botton Pressed\n");
            break;
        }
        wait_msec(50);
    }
}

/* ラップタイム走行 */
static void laptime_tracer(){
    sta_cyc(LINE_TRACER_TASK_CYC);  /* ライントレースタスクの起動 */
    sta_cyc(ODOMETRY_TASK_CYC);  /*オドメトリタスクの起動 */
}

/* タスク停止関数（ミリ秒） */
void wait_msec(int32_t msec){
    #ifdef ETROBOC_SIM
        dly_tsk(msec * 1000 / 0.6);
    #else
        dly_tsk(msec * 1000);
    #endif
}