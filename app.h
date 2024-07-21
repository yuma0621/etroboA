#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1)
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2)
#define CALIBRATION_PRIORITY  (TMIN_APP_TPRI + 2) /* キャリブレーションタスク */
#define ODOMETRY_PRIORITY  (TMIN_APP_TPRI + 2)

/* タスク周期の定義 */
#define TRACER_PERIOD  (100 * 1000 / 0.6 ) /* ライントレースタスク:100msec周期 */
#define CALIBRATION_PERIOD  (100 * 1000 / 0.6) /* キャリブレーションタスク:100msec周期 */
#define ODOMETRY_PERIOD  (100 * 1000 / 0.6 )

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

extern void main_task(intptr_t exinf); /*メインタスク*/
extern void tracer_task(intptr_t exinf);/* トレーサータスク*/
extern void calibration_task(intptr_t unused);/* キャリブレーションタスク */
extern void odometry_task(intptr_t exinf); /*オドメトリタスク*/

#endif /* TOPPERS_MACRO_ONLY */

void main_init();     /*初期設定*/
void wait_start();/* 動作開始待ち */
void calibration();/* キャリブレーション */
void p_tracer();/*P制御ライントレース*/
void wait_msec(int32_t msec);/* タスク停止関数(ミリ秒) */

#ifdef __cplusplus
}
#endif
