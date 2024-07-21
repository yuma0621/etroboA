#include "ev3.h"

//using namespace ev3api;

class Color {
public:
  Color(); //コンストラクタ
  static int16_t get_rgb_diff();
  //void init();←コンストラクタで十分か?
  //void terminate();

/* RGB値 */
rgb_raw_t rgb_max;
rgb_raw_t rgb_min;
rgb_raw_t rgb_ave;
};
