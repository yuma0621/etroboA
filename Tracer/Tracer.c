#include "app.h"
#include "Odometry.h"
#include "Tracer.h"


typedef enum {
    AHEAD,
    TURN,
    END
} RUN_STATE;

void tracer_task(intptr_t unused) {
    static RUN_STATE state = AHEAD;
    static RUN_STATE state = TURN;

    /* 計測器初期化 */
    //Distance_reset();
    //Direction_reset();

    switch(state) {
        case AHEAD:
            //左右車輪駆動
            ev3_motor_set_power(left_motor, 30);
            ev3_motor_set_power(right_motor, 30);
            //400mm以上前進したら，次状態遷移
            if(Distance_getDistance() > 400.0) {
                state = END;
            }
            break;
        case TURN:
            ev3_motor_set_power(left_motor, 30);
            ev3_motor_set_power(right_motor, -30);
            if( Direction_getDirection() > 90.0) {;
                state = END;
            }
            break;
        case END:
            ev3_motor_stop(left_motor, true);
            ev3_motor_stop(right_motor, true);
            break;
        default:
            break;
    }
}
