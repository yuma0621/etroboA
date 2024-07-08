#include "app.h"
#include "Color.h"
#include <stdio.h>

Color::Color(){
 rgb_max.r = 0;
    rgb_min.r = 200;
    rgb_max.g = 0;
    rgb_min.g = 200;
    rgb_max.b = 0;
    rgb_min.b = 200;
}

/* キャリブレーションタスク(100msec周期) */
void Color::calibration_task(intptr_t unused) {
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
    /* 平均値（目標値）の計算 */
    rgb_ave.g = (rgb_min.g + rgb_max.g) / 2;
    printf("[Calibration Task] max:%5d, min:%5d, ave:%5d\n", rgb_max.g, rgb_min.g, rgb_ave.g);
    ext_tsk();/* タスク終了 */
}

/* 反射光とキャリブレーション値との差を取得*/
int16_t Color::get_rgb_diff(){
    rgb_raw_t rgb_raw;
    int16_t green_diff;
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);       /* カラーセンサ値の取得 */
    green_diff = rgb_ave.g - rgb_raw.g;                         /* キャリブレーション値との差分を計算(green) */
    return green_diff;
}
