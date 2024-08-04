#include <stdio.h>
#include "app.h"
#include "Color.h"

/* RGB値補正関数 */
 void adjust_rgb(rgb_raw_t *rgb_raw){
    /* RGB値補正（シミュレーション向けの補正値） */
    (*rgb_raw).r = (uint16_t)((*rgb_raw).r * RGB_GAIN_RED);
    (*rgb_raw).g = (uint16_t)((*rgb_raw).g * RGB_GAIN_GREEN);
    (*rgb_raw).b = (uint16_t)((*rgb_raw).b * RGB_GAIN_BLUE);
}

/* RGB値からHSV値に変換する関数 */
void rgb2hsv(rgb_raw_t rgb_raw, int16_t *h, int16_t *s, int16_t *v){
    int16_t max,min;
    /* 最大値取得 */
    max = rgb_raw.r;                
    if (rgb_raw.g > max) max = rgb_raw.g;
    if (rgb_raw.b > max) max = rgb_raw.b;
    /* 最小値取得 */
    min = rgb_raw.r;
    if (rgb_raw.g < min) min = rgb_raw.g;
    if (rgb_raw.b < min) min = rgb_raw.b;
    /* 色相H計算 */
    if(rgb_raw.r == max) *h = 60 * (rgb_raw.g - rgb_raw.b) / (max - min);
    if(rgb_raw.g == max) *h = 60 * (rgb_raw.b - rgb_raw.r) / (max - min) + 120;
    if(rgb_raw.b == max) *h = 60 * (rgb_raw.r - rgb_raw.g) / (max - min) + 240;
    /* 彩度S計算 */
    *s = 255 * (max - min) / max;
    /* 明度V計算 */
    *v = max;
    /* 結果表示 */
    printf("rgbhsv=, %3d, %3d, %3d, %3d, %3d, %3d, ",rgb_raw.r, rgb_raw.g, rgb_raw.b,*h,*s,*v);
}

