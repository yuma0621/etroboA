#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"

    int cur_gridX = 0; // 現在位置座標のX値
    int cur_gridY = 0; // 現在位置座標のY値
    float target_dir = 0.0; // 現在位置座標から目標座標までの距離
    float target_dis = 0.0; // 現在位置座標から目標座標までの方位
    float cur_dir = 0.0; // 方位計の現在値
    float cur_dis = 0.0; // 距離計の現在値
    int grid_count = 0;  // 目標座標構造体への参照カウンタ

/* 初期化関数 */
void Grid_init();
/* 座標aから座標bまでの移動距離を設定する関数 */
void Grid_setDistance(int aX, int aY, int bX, int bY);
/* 座標aから座標bまでの移動距離を取得する関数 */
float Grid_getDistance();

/* 目標座標の方位を設定する関数 */
void Grid_setDirection(int aX, int aY, int bX, int bY);
/* 目標座標の方位を取得する関数 */
float Grid_getDirection();

#endif

#ifdef __cplusplus
}
#endif
