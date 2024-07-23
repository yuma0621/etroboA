#include "Color.h"
//#include <stdio.h>

void color_init(){
    color_sensor = EV3_PORT_2;
    rgb_max.r = 0;
    rgb_min.r = 200;
    rgb_max.g = 0;
    rgb_min.g = 200;
    rgb_max.b = 0;
    rgb_min.b = 200;
}

/* 反射光とキャリブレーション値との差を取得*/
int16_t color_get_rgb_diff(){
    rgb_raw_t rgb_raw;
    int16_t green_diff;
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);       /* カラーセンサ値の取得 */
    green_diff = rgb_ave.g - rgb_raw.g;                     /* キャリブレーション値との差分を計算(green) */
    return green_diff;
}

