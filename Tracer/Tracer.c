#include "ev3api.h"
#include "app.h"
#include "Odometry.h"
#include "Tracer.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

// 構造体に格納されている目標座標の数
#define GRID_NUM 4
#define GRID_SIZE 352.0 //座標のマス幅（352mm）

/**
 * Global variables
 */
typedef enum {
    TURN, // 目標座標の方位へ向くまで旋回
    MOVE, // 目標座標に到達するまで前進
    END   // 構造体に格納されている座標を移動しきったら停止
} RUN_STATE;
static RUN_STATE state = TURN;

//目標座標情報の構造体
struct GRID_XY {
    int gridX;
    int gridY;
};
 struct GRID_XY target_grid[GRID_NUM] = {{0,1},
                                            {1,1},
                                            {1,2},
                                            {0,2} };

static float grid_distance = 0.0; //現在座標から目標座標までの距離
static float grid_direction = 90.0;//現在座標から目標座標の方位

int bias;
extern float angle_diff;

void tracer_task(intptr_t unused) {

        // 計測器の現在値を格納
        cur_dis = odom_Distance_getDistance();
        cur_dir = odom_Direction_getDirection();

        if(5 > abs(angle_diff)){
            bias = 0;
        }
        else{
            if(angle_diff>=0){
                bias = -5;
            }
            else{
                bias = 5;
            }
        }
    
        switch(state) {
        case TURN:
            // 指定方位まで旋回する
            if(cur_dir < target_dir) {
                ev3_motor_set_power(left_motor, 69);
                ev3_motor_set_power(right_motor, -55);
            } else {
                ev3_motor_set_power(left_motor, -69);
                ev3_motor_set_power(right_motor, 55);
            }
            // 指定方位の一定範囲内に収まったら,移動開始
            if( (cur_dir > (target_dir-1.0)) && (cur_dir < (target_dir+1.0)) ) {;
                state = MOVE;
                printf("state = MOVE\n");
            }
            break;
        case MOVE:
            ev3_motor_set_power(left_motor, 45 + bias);
            ev3_motor_set_power(right_motor, 45 - bias);

            // 指定位置までたどり着いたら状態遷移
            if( (cur_dis > target_dis)  && (grid_count < (GRID_NUM-1)) ) {
                // 現在位置座標を更新
                cur_gridX = target_grid[grid_count].gridX;
                cur_gridY = target_grid[grid_count].gridY;
                /* 計測器情報のリセット */
                //motorをストップ
                ev3_motor_stop(left_motor, true);
                ev3_motor_stop(right_motor, true);
                //一旦オドメトリタスクをストップ&待ち
                stp_cyc(ODOMETRY_TASK_CYC);
                wait_msec(50);
                // 距離値&方向のリセット
                odom_Distance_reset();
                odom_Direction_setDirection(target_dir);//←これいる？
                // 次の座標までの方位,距離を格納する
                grid_count++;
                Grid_setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                Grid_setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                target_dis = Grid_getDistance();
                target_dir = Grid_getDirection();

                // 再度,次座標への旋回を開始
                state = TURN;
                sta_cyc(ODOMETRY_TASK_CYC);
                wait_msec(50);
                printf("state = TURN\n");
            } else
                if( (cur_dis > target_dis) && (grid_count >= (GRID_NUM-1)) ) {
                    state = END;
                    printf("state = END\n");
                }
            break;
        case END:
            // モータを停止
            ev3_motor_stop(left_motor, true);
            ev3_motor_stop(right_motor, true);
            break;
        default:
            break;
        }
}


/* 初期化関数 */
void Grid_init() {
    grid_distance = 0.0;
    grid_direction = 0.0;
}

/* 座標aから座標bまでの移動距離を設定する関数 */
void Grid_setDistance(int aX, int aY, int bX, int bY) {
    grid_distance = sqrt( pow((float)(bX-aX),2) + pow((float)(bY-aY),2) ) *  GRID_SIZE;
}

/* 座標aから座標bまでの移動距離を取得する関数 */
float Grid_getDistance() {
    return grid_distance;
}

/* 目標座標の方位を設定する関数 */
void Grid_setDirection(int aX, int aY, int bX, int bY) {
    float targetDir = 0.0;// 目標方位

    //　座標aから座標bへの方位（ラジアン）を取得
    targetDir = atan2((float)(bY-aY), (float)(bX-aX));
    //ラジアンから度に変換
    targetDir = targetDir * 180.0 / PI;

    grid_direction = targetDir;
}

/* 目標座標の方位を取得する関数 */
float Grid_getDirection() {
    return grid_direction;
}
