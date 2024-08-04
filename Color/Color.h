#ifdef __cplusplus
extern "C" {
#endif

/* シミュレータ */
#ifdef ETROBOC_SIM
    #define RGB_GAIN_RED        1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_GREEN      1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_BLUE       0.7     /* RGBセンサ補正値 */
/* RasPike実機 */
#else
    #define RGB_GAIN_RED        1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_GREEN      1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_BLUE       1.0     /* RGBセンサ補正値 */
#endif

/* 関数プロトタイプ宣言 */
void adjust_rgb(rgb_raw_t *rgb_raw);
void rgb2hsv(rgb_raw_t rgb_raw, int16_t *h, int16_t *s, int16_t *v);

#ifdef __cplusplus
}
#endif
