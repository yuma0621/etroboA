#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/* v850でコンパイルしているときにシミュレータとみなす */
#ifdef __v850
    #define ETROBOC_SIM 1
#endif

/* グローバル変数 */

/* センサーポートの定義 */
static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

/* モーターポートの定義 */
static const motor_port_t
    arm_motor       = EV3_PORT_A,
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B;

/* STACK_SIZE */
#ifndef STACK_SIZE
    #define STACK_SIZE  (4096)
#endif

/* タスクのプロトタイプ宣言 */
#ifndef TOPPERS_MACRO_ONLY
    extern void main_task(intptr_t exinf);              /* メインタスク(起動時にのみ関数コールされる) */
    extern void tracer_task(intptr_t exinf); 
    extern void odometry_task(intptr_t exinf); 
#endif

/* タスク優先度 */
#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1)
#define ODOMETRY_PRIORITY  (TMIN_APP_TPRI + 2)
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2)

/* タスク周期 */
#ifdef ETROBOC_SIM  /* シミュレータ */
    #define TRACER_PERIOD  (10 * 1000 / 0.6 )
    #define ODOMETRY_PERIOD  (10 * 1000 / 0.6 )
#else   /* RasPike実機 */
    #define TRACER_PERIOD  (10 * 1000 )
    #define ODOMETRY_PERIOD  (1 * 1000 )
#endif

/* 関数のプロトタイプ宣言 */
static void init_global();          /* グローバル変数の初期化 */
static void port_config();          /* ポート初期設定 */
void wait_start();           /* 動作開始待ち（シミュレータ：自動またはスペースキーで開始, 実機：BlueToothボタンで開始） */
static void laptime_tracer();       /* ラップタイム走行 */
void wait_msec(int32_t msec);       /* タスク停止関数（ミリ秒） */

#ifdef __cplusplus
}
#endif
