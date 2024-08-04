#include "app.h"
#include "Calibration.h"
#include "Color.h"
#include <stdio.h>

/* S値キャリブレーション関数 */
void s_calibration(){
    printf("S Calibration Start\n");
    #ifdef ETROBOC_SIM                      /* シミュレータのみ */
        /* 後退 */
        ev3_motor_set_power(left_motor,  -10);
        ev3_motor_set_power(right_motor, -10);
        wait_msec(1000);
    #else
    #endif
    sta_cyc(S_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク起動 */
    calib_movement();                       /* キャリブレーション動作 */
    stp_cyc(S_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク停止 */
    printf("S Calibration Finished\n");
    #ifdef ETROBOC_SIM
        wait_msec(500);
    #else   /* RasPike実機 */
        wait_start();                           /* 動作開始待ち（実機：BlueToothボタンで開始） */
    #endif
}

/* V値キャリブレーション関数 */
void v_calibration(){
    printf("V Calibration Start\n");
    #ifdef ETROBOC_SIM                      /* シミュレータのみ */
        /* 前進 */
        ev3_motor_set_power(left_motor,  10);
        ev3_motor_set_power(right_motor, 10);
        wait_msec(1000);
    #else
    #endif
    sta_cyc(V_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク起動 */
    calib_movement();                       /* キャリブレーション動作 */
    stp_cyc(V_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク停止 */
    printf("V Calibration Finished\n");
    #ifdef ETROBOC_SIM
        wait_msec(500);
    #else   /* RasPike実機 */
        wait_start();                       /* 動作開始待ち（実機：BlueToothボタンで開始） */
    #endif
}

/* 光センサV値キャリブレーションタスク）(10msec周期) */
void v_sensor_calib_task(intptr_t unused) {
    rgb_raw_t rgb_raw;
    int16_t h,s,v;
    /* 光センサ値取得 */
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);   /* RGB値を取得 */
    adjust_rgb(&rgb_raw);                                   /* RGB値を補正 */
    rgb2hsv(rgb_raw, &h, &s, &v);                           /* RGB値をHSV値に変換 */
    /* 最小値と最大値と平均値の更新 */
    if(v < v_min)   v_min = v;
    if(v > v_max)   v_max = v;
    v_ave = (v_min + v_max) / 2;
    /* 表示 */
    printf("v_calib=, %3d, %3d, %3d, \n", v_min, v_max, v_ave);
    ext_tsk();/* タスク終了 */
}

/* 光センサS値キャリブレーションタスク）(10msec周期) */
void s_sensor_calib_task(intptr_t unused) {
    rgb_raw_t rgb_raw;
    int16_t h,s,v;
    /* 光センサ値取得 */
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);   /* RGB値を取得 */
    adjust_rgb(&rgb_raw);                                   /* RGB値を補正 */
    rgb2hsv(rgb_raw, &h, &s, &v);                           /* RGB値をHSV値に変換 */
    /* 最小値と最大値と平均値の更新 */
    if(s < s_min)   s_min = s;
    if(s > s_max)   s_max = s;
    s_ave = (s_min + s_max) / 2;
    printf("s_calib=, %3d, %3d, %3d, \n", s_min, s_max, s_ave);
    ext_tsk();/* タスク終了 */
}

/* キャリブレーション時のモータ制御関数 */
void calib_movement(){

    /* シミュレータ */
    #ifdef ETROBOC_SIM
        /* 右旋回 */
        ev3_motor_set_power(left_motor,  10);
        ev3_motor_set_power(right_motor, -10);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 後退 */
        ev3_motor_set_power(left_motor,  -5);
        ev3_motor_set_power(right_motor, -5);
        wait_msec(2000);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 前進 */
        ev3_motor_set_power(left_motor,  5);
        ev3_motor_set_power(right_motor, 5);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 左旋回 */
        ev3_motor_set_power(left_motor, -10);
        ev3_motor_set_power(right_motor, 10);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        
    /* RasPike実機 */
    #else
        /* 前進 */
        ev3_motor_set_power(left_motor,  40);
        ev3_motor_set_power(right_motor, 40);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 後退 */
        ev3_motor_set_power(left_motor,  -40);
        ev3_motor_set_power(right_motor, -40);
        wait_msec(1500);
        
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);          
    #endif
}
