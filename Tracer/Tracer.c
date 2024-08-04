typedef enum {
    AHEAD,
    TURN,
    END
} RUN_STATE;

void tracer_task(intptr_t unused) {
    static RUN_STATE state = TURN;

    /* 計測器初期化 */
    Distance_reset();
    Direction_reset();

    while(1) {
        /* 計測器更新 */
        Distance_update();
        Direction_update();

        switch(state) {
        case TURN:
            ev3_motor_set_power(left_motor, 30);
            ev3_motor_set_power(right_motor, -30);
            if( Direction_getDirection() > 180.0) {;
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
}
