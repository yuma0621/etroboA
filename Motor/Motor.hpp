using namespace ev3api;
/* 円周率 */
#define PI 3.14159265358

class Motor{
public:
  Motor(); //コンストラクタ

  void terminate();
  void motor_control();
  void Distance_reset();
  void Distance_update();
  void Distance_getDistance();
  void Direction_reset();
  float Direction_getDirection();
  void Direction_update()

 // 方位を更新
void Direction_update();

/* 方位を取得(右旋回が正転) */
float Direction_getDirection();

 // 方位を更新
void Direction_update();

 /* モーターポートの定義 */
static const motor_port_t
    arm_motor       = EV3_PORT_A,
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B;

   static floatdistance;
   static float distance4msR;
   static float distance4msL;
   static float pre_angleL;
   static float pre_angleR;
   static float distance4ms;
   static float direction; //現在の方位

private:
  const int8_t mThreshold = 20;
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
};
