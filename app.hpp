
#ifdef __cplusplus
extern "C" {
#endif

#include "Motor.hpp"
#include "ev3api.h"

#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1)
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2)
#define CALIBRATION_PRIORITY  (TMIN_APP_TPRI + 2) /* キャリブレーションタスク */

/* タスク周期の定義 */
#define LINE_TRACER_PERIOD  (100 * 1000 / 0.6 ) /* ライントレースタスク:100msec周期 */
#define CALIBRATION_PERIOD  (100 * 1000 / 0.6) /* キャリブレーションタスク:100msec周期 */

/* センサーポートの定義 */
static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

#ifndef STACK_SIZE
#define STACK_SIZE      (4096)
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

Motor motor;

extern void main_task(intptr_t exinf);

static void main_init();     /*初期設定*/
static void wait_start();/* 動作開始待ち */
static void calibration();/* キャリブレーション */
static void p_tracer();/*P制御ライントレース*/
void wait_msec(int32_t msec);/* タスク停止関数(ミリ秒) */

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
