#ifdef __cplusplus
extern "C" {
#endif

/* シミュレータ */
#ifdef ETROBOC_SIM
    #define BASE_SPEED         30       /* 基準前進速度 */
    #define V_KP 2.00f
    #define V_KI 0.000f
    #define V_KD 0.005f
    #define EDGE_BLACK_COEF    0.40f    /* 旋回比例係数（黒線走行用） */
    #define EDGE_BLUE_COEF     0.20f    /* 旋回比例係数（青線走行用） */ 
    #define S_AVERAGE           90      /* 彩度目標値（青線走行用） */
    #define V_AVERAGE           50      /* 明度目標値（黒線走行用） */
    #define S_COLORFUL          60      /* 彩度閾値（黒線orカラーマーカー） */
    #define V_DARK              10      /* 明度閾値（HSVの計算に誤差が生じるほど暗い） */
    #define V_BLACK             30      /* 明度閾値（黒線の中央付近判定用） */
/* RasPike実機 */
#else
    #define BASE_SPEED         35       /* 基準前進速度 */
    #define V_KP 0.20f
    #define V_KI 0.000f
    #define V_KD 0.005f
    #define EDGE_BLACK_COEF    0.20f    /* 旋回比例係数（黒線走行用） */
    #define EDGE_BLUE_COEF     0.40f    /* 旋回比例係数（青線走行用） */ 
    #define S_AVERAGE           80      /* 彩度目標値（青線走行用） */
    #define V_AVERAGE           50      /* 明度目標値（黒線走行用） */
    #define S_COLORFUL          60      /* 彩度閾値（黒線orカラーマーカー） */
    #define V_DARK              50      /* 明度閾値（HSVの計算に誤差が生じるほど暗い） */
    #define V_BLACK             30      /* 明度閾値（黒線の中央付近判定用） */
#endif

/* 左右コース切り替え*/
#define LEFT_COURSE     1
#define RIGHT_COURSE    -1

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1) 
#define RIGHT_EDGE        (1) 

/* 列挙型　エッジ状態 */
typedef enum EDGE_STATE{
    EDGE_BLACK,             /* 黒線走行中 */
    EDGE_BLUE,              /* 青線走行中 */
    EDGE_IN,                /* エッジ切り替え（開始から黒線中央まで） */
    EDGE_OUT,               /* エッジ切り替え（黒線中央から反対エッジまで） */
};

/* 列挙型　シーン状態 */
typedef enum SCENE_STATE{
    SCENE_START_1,          /* シミュレータ用（逆エッジから開始するため） */
    SCENE_START_2,          /* 外側エッジを走行 */
    SCENE_WLOOP_1,          /* ダブルループ１ */
    SCENE_WLOOP_2,          /* ダブルループ２ */
    SCENE_WLOOP_3,          /* ダブルループ３ */
    SCENE_WLOOP_4,          /* ダブルループ４ */
};

/* 構造体　エッジ状態ごとのパラメータ */
struct EDGE_PARAM{
    char current_edge[16];          /* エッジ状態名 */
};

/* 構造体 　シーン状態ごとのパラメータ */
struct SCENE_PARAM{
    char current_scene[32];         /* シーン状態名 */
    enum SCENE_STATE next_scene;    /* 遷移先シーン */
    int edge;                       /* 走行エッジ */
    int in;                         /* 旋回量（EDGE_IN） */
    int out;                        /* 旋回量（EDGE_OUT） */
};

/* 関数プロトタイプ宣言 */
static void adjust_rgb(rgb_raw_t *rgb_raw);
static void rgb2hsv(rgb_raw_t rgb_raw, int16_t *h, int16_t *s, int16_t *v);
static void steering(enum SCENE_STATE scene_state, enum EDGE_STATE edge_state, int16_t s, int16_t v);
static int get_pid_v(int16_t v);
static int get_pid_s(int16_t s);
static int get_rot_in(enum SCENE_STATE scene_state);
static int get_rot_out(enum SCENE_STATE scene_state);
static void state_transition(enum SCENE_STATE *scene_state, enum EDGE_STATE *edge_state, int16_t s, int16_t v);
#ifdef __cplusplus
}
#endif
