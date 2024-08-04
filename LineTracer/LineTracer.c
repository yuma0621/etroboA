#include "app.h"
#include "LineTracer.h"
#include "Color.h"
#include <stdio.h>

/* 構造体　エッジ状態ごとのパラメータ */
static struct EDGE_PARAM edge_table[] = {
    { "BLACK"  },
    { "BLUE "  },
    { "IN   "  },
    { "OUT  "  },
};

/* 構造体　シーン状態ごとのパラメータ */
static struct SCENE_PARAM scene_param[] = {
    /* 現在シーン、遷移先シーン、走行エッジ、旋回量（EDGE_IN）、旋回量（EDGE_OUT） */
#ifdef ETROBOC_SIM
        {"START_1",   SCENE_START_2,  RIGHT_EDGE,     -2,     -2  },
        {"START_2",   SCENE_WLOOP_1,  LEFT_EDGE,      -4,     -4  },
        {"WLOOP_1",   SCENE_WLOOP_2,  RIGHT_EDGE,     -6,     -4  },
        {"WLOOP_2",   SCENE_WLOOP_3,  LEFT_EDGE,      -0,     -0  },
        {"WLOOP_3",   SCENE_WLOOP_4,  RIGHT_EDGE,     -0,     -0  },
        {"WLOOP_4",   SCENE_WLOOP_4,  LEFT_EDGE,      -0,     -0  },
#else
        {"START_1",   SCENE_START_2,  RIGHT_EDGE,     -5,     -5  },
        {"START_2",   SCENE_WLOOP_1,  LEFT_EDGE,      -9,     -9  },
        {"WLOOP_1",   SCENE_WLOOP_2,  RIGHT_EDGE,     -5,     -5  },
        {"WLOOP_2",   SCENE_WLOOP_3,  LEFT_EDGE,      -5,     -5  },
        {"WLOOP_3",   SCENE_WLOOP_4,  RIGHT_EDGE,     -5,     -5  },
        {"WLOOP_4",   SCENE_WLOOP_4,  LEFT_EDGE,      -5,     -5  },
#endif
};

/***************************************************************************************************
    タスク定義
 ***************************************************************************************************/
/* ライントレースタスク(10msec周期） */
void tracer_task(intptr_t unused) {

    /* ライントレース状態値（列挙型） */
    static enum SCENE_STATE scene_state = SCENE_START_2;    /* シーン状態 */
    static enum EDGE_STATE edge_state = EDGE_BLACK;         /* エッジ状態 */
    
    rgb_raw_t rgb_raw;  /* RGB値 */
    int16_t h, s ,v;    /* 色相H *//* 彩度S *//* 明度V */

    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);   /* RGB値を取得 */
    adjust_rgb(&rgb_raw);                                   /* RGB値を補正 */
    rgb2hsv(rgb_raw, &h, &s, &v);                           /* RGB値をHSV値に変換 */
    state_transition(&scene_state, &edge_state, s, v);      /* シーン状態とエッジ状態の遷移 */
    steering(scene_state, edge_state, s, v);                /* 旋回 */
    ext_tsk();                                              /* タスク終了 */ 
}

/***************************************************************************************************
    内部関数
 ***************************************************************************************************/

/* シーン状態とエッジ状態の遷移関数 */
static void state_transition(enum SCENE_STATE *scene_state, enum EDGE_STATE *edge_state, int16_t s, int16_t v){
    switch(*edge_state){
        case EDGE_BLACK:                            /* 黒線走行状態 */
            if(s > S_AVERAGE && v > V_DARK){       /* 青線を検知したら */
                *edge_state = EDGE_BLUE;             /* 次の走行状態に遷移 */
            }
            break;
        case EDGE_BLUE:                             /* 青線走行状態 */ 
            if(s < S_COLORFUL && v > V_DARK){       /* 黒線を検知したら */
                *edge_state = EDGE_IN;               /* 次の走行状態に遷移 */
            }
            break;
        case EDGE_IN:                               /* 黒線突入状態*/
            if(v <= V_BLACK){                       /* 黒線中央を検知したら */
                *edge_state = EDGE_OUT;              /* 次の走行状態に遷移 */
            }
            break;
        case EDGE_OUT:                              /* 黒線脱出状態 */
            if(v >= v_ave){                         /* 反対エッジを検知したら */
                *edge_state = EDGE_BLACK;           /* 黒線走行状態に戻る */
                /* 次のシーンに遷移 */
                *scene_state = scene_param[*scene_state].next_scene;
            }
            break;
        default:
            break;
    }
    printf("scene=%s, ",scene_param[*scene_state].current_scene);     /* シーン状態の表示 */
    printf("edge=%s, \n",edge_table[*edge_state].current_edge);           /* エッジ状態の表示 */
}

/* 旋回関数 */
static void steering(enum SCENE_STATE scene_state, enum EDGE_STATE edge_state, int16_t s, int16_t v){
    int rotation = 0;
    int power_L;                                           
    int power_R;
    /* 旋回量の計算 */
    switch(edge_state){
        case EDGE_BLACK:    rotation = get_pid_v(v);    break;  /* 明度Vで走行 */
        case EDGE_BLUE:     rotation = get_pid_s(s);    break;  /* 彩度Sで走行 */
        case EDGE_IN:       rotation = get_rot_in(scene_state);    break;  /* 固定値 */
        case EDGE_OUT:      rotation = get_rot_out(scene_state);   break;  /* 固定値 */
        default: break;
    }
    /* モータパワーの計算 */                                           
    power_L = (int)(BASE_SPEED + (rotation * scene_param[scene_state].edge));
    power_R = (int)(BASE_SPEED - (rotation * scene_param[scene_state].edge));
    /* モータパワーのセット */
    ev3_motor_set_power(left_motor, power_L);
    ev3_motor_set_power(right_motor, power_R);
}

/* 明度VによるPID制御値取得関数 */
static int get_pid_v(int16_t v){
    int16_t p;
    static float i = 0;
    float d;
    static int16_t v_prev;
    p = (v_ave - v);            /* 比例制御P */
    i += p * 0.01f;                     /* 積分制御I */
    d = (v_prev - v)/0.01f;    /* 比例制御D */
    
    if(i > 10000){
        printf("too much integral value\n");
    }
    v_prev = v;
    return (int)(V_KP * p + V_KI * i + V_KD * d);
}

/* 彩度SによるPID制御値取得関数 */
static int get_pid_s(int16_t s){
    return (int)((s - s_ave)* EDGE_BLUE_COEF);      /* 現在P制御のみ */
}

/* 黒線進入時の旋回量（固定値）取得関数 */
static int get_rot_in(enum SCENE_STATE scene_state){
    return (int)scene_param[scene_state].in;
}

/* 黒線離脱時の旋回量（固定値）取得関数 */
static int get_rot_out(enum SCENE_STATE scene_state){
    return (int)scene_param[scene_state].out;
}
