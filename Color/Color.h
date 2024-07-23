#pragma once

#include "ev3api.h"

//using namespace ev3api;

//public:
  void color_init(); //コンストラクタ
  int16_t color_get_rgb_diff();
  //void init();←コンストラクタで十分か?
  //void terminate();

/* RGB値 */
/* センサーポートの定義 */
sensor_port_t color_sensor;

rgb_raw_t rgb_max;
rgb_raw_t rgb_min;
rgb_raw_t rgb_ave;
