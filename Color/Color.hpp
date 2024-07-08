#include "ev3.h"

using namespace ev3api;

class Color {
public:
  Color(); //コンストラクタ
  extern void calibration_task(intptr_t exinf);
  static int16_t get_rgb_diff();
  //void init();←コンストラクタで十分か？
  //void terminate();

 /* RGB値 */
 extern rgb_raw_t rgb_max;
 extern rgb_raw_t rgb_min;
 extern rgb_raw_t rgb_ave;
}
