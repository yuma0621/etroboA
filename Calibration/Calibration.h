#ifdef __cplusplus
extern "C" {
#endif

/* 関数プロトタイプ宣言 */
void v_calibration();                       /* V値キャリブレーション関数 */
void s_calibration();                       /* S値キャリブレーション関数 */
void v_sensor_calib_task(intptr_t unused);  /* 光センサV値キャリブレーションタスク）(10msec周期) */
void s_sensor_calib_task(intptr_t unused);  /* 光センサS値キャリブレーションタスク）(10msec周期) */
void calib_movement();                      /* キャリブレーション */
#ifdef __cplusplus
}
#endif
