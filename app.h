#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/* v850でコンパイルしているときにシミュレータとみなす */
#ifdef __v850
    #define ETROBOC_SIM 1
#endif

/* グローバル変数 */
extern int16_t s_min;          /* 彩度Sの平均値（目標値） */
extern int16_t s_max;          /* 彩度Sの平均値（目標値） */
extern int16_t s_ave;          /* 彩度Sの平均値（目標値） */
extern int16_t v_min;          /* 明度Vの平均値（目標値） */
extern int16_t v_max;          /* 明度Vの平均値（目標値） */
extern int16_t v_ave;          /* 明度Vの平均値（目標値） */

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
    extern void v_sensor_calib_task(intptr_t unused);   /* 光センサV値キャリブレーションタスク）(10msec周期) */
    extern void s_sensor_calib_task(intptr_t unused);   /* 光センサS値キャリブレーションタスク）(10msec周期) */
    extern void tracer_task(intptr_t exinf);            /* ライントレースタスク(10msec周期） */
    extern void odometry_task(intptr_t exinf); 
#endif

/* タスク優先度 */
#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1)
#define V_CALIB_PRIORITY  (TMIN_APP_TPRI + 2)
#define S_CALIB_PRIORITY  (TMIN_APP_TPRI + 2)
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2)
#define ODOMETRY_PRIORITY  (TMIN_APP_TPRI + 2)

/* タスク周期 */
#ifdef ETROBOC_SIM  /* シミュレータ */
    #define LINE_TRACER_PERIOD  (10 * 1000 / 0.6 )
    #define V_CALIB_PERIOD  (10 * 1000 / 0.6 )
    #define S_CALIB_PERIOD  (10 * 1000 / 0.6 )
    #define ODOMETRY_PERIOD  (10 * 1000 / 0.6 )
#else   /* RasPike実機 */
    #define LINE_TRACER_PERIOD  (10 * 1000 )
    #define V_CALIB_PERIOD  (10 * 1000 )
    #define S_CALIB_PERIOD  (10 * 1000 )
    #define ODOMETRY_PERIOD  (10 * 1000 / 0.6 )
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
