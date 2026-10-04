#ifndef FT800_DEMO_H
#define FT800_DEMO_H
#include "MikroSDK.Ft800"
#include "tmr.h"
#include "string.h"
#include "stdio.h"

typedef struct
{
    uint8_t s_red;
    uint8_t s_green; 
    uint8_t s_blue;
    uint8_t e_red; 
    uint8_t e_green; 
    uint8_t e_blue; 
    uint8_t alpha;

}gradient_colors_t;

typedef struct
{
    uint16_t x1;
    uint16_t x2;
    uint16_t x3;
    uint16_t y1;
    uint16_t y2;
    uint16_t y3;
}icon_position_t;

typedef struct
{
    uint16_t val;
    uint32_t tracker;
}slider_data_t;

typedef struct
{
    bool app_enter;
    bool awaiting_second;  
    bool result_ready;
    bool stpwch_reset;
    bool stpwch_pause;

}flag_t;

typedef struct
{
    float operator_1;
    float operator_2;
    char current_op; 
}calc_operators_t;

typedef struct
{
    uint8_t s;
    uint8_t m;
    uint8_t h;
    uint8_t s_mem;
    uint8_t m_mem;
    uint8_t h_mem;
    uint8_t sec;
    uint8_t min;
    uint8_t hr;

}stopwatch_t;

typedef struct
{
    uint16_t g_angle_1;
    uint16_t g_angle_2;
    uint16_t g_val_1;
    uint16_t g_val_2;
    uint32_t tracker;
}settings_t;

#define ICON_INIT_VALUE_X1 180
#define ICON_INIT_VALUE_X2 720
#define ICON_INIT_VALUE_X3 1150
#define ICON_INIT_VALUE_Y1 60
#define ICON_INIT_VALUE_Y2 120
#define ICON_INIT_VALUE_Y3 105
#define ALPHA_VALUE_INIT  255
#define SLIDER_INIT_VALUE 0x0001
#define SLIDER_VALUE_RANGE_1 21845
#define SLIDER_VALUE_RANGE_2 43690
#define COLOR_INTERPOLATION_COEF 1.0
#define COLOR_INTERPOLATION_SCALE_COEF 255
#define TAG_MASK_SET 1
#define TAG_MASK_RESET 0
#define IC_TXT_SIZE 31
#define EDG_COLOR 0x0000
#define IC_EDG_WIDTH 5
#define ESC_BUTTON_TAG 5

#define CALC_IC_TAG 2
#define CALC_IC_REC_WIDTH 120
#define CALC_IC_REC_HEIGHT 120
#define CALC_IC_REC_EDG_RAD 10
#define CALC_IC_SIGN_COLOR 255
#define CALC_IC_TXT_OFFSET_X_1 20
#define CALC_IC_TXT_OFFSET_X_2 80
#define CALC_IC_TXT_OFFSET_X_3 20
#define CALC_IC_TXT_OFFSET_X_4 78
#define CALC_IC_TXT_OFFSET_X_5 30
#define CALC_IC_TXT_OFFSET_Y_1 7
#define CALC_IC_TXT_OFFSET_Y_2 7
#define CALC_IC_TXT_OFFSET_Y_3 65
#define CALC_IC_TXT_OFFSET_Y_4 65
#define CALC_IC_TXT_OFFSET_Y_5 130
#define CALC_IC_EDG_OFFSET_SX_1 60
#define CALC_IC_EDG_OFFSET_EX_1 60
#define CALC_IC_EDG_OFFSET_EY_1 120
#define CALC_IC_EDG_OFFSET_SY_2 60
#define CALC_IC_EDG_OFFSET_EX_2 120
#define CALC_IC_EDG_OFFSET_EY_2 60

#define STPWCH_IC_TAG 3
#define STPWCH_IC_CRC_RAD_1 120
#define STPWCH_IC_CRC_RAD_2 10
#define STPWCH_IC_SIGN_COLOR 255

#define STPWCH_IC_EDG_OFFSET_SX_1 10
#define STPWCH_IC_EDG_OFFSET_SY_1 70 
#define STPWCH_IC_EDG_OFFSET_EX_1 10
#define STPWCH_IC_EDG_OFFSET_EY_1 70
#define STPWCH_IC_EDG_OFFSET_EY_2 45
#define STPWCH_IC_TXT_OFFSET_X 95
#define STPWCH_IC_TXT_OFFSET_Y 70

#define SET_IC_TAG 4
#define SET_IC_CRC_RAD_1 100
#define SET_IC_CRC_RAD_2 5
#define SET_IC_REC_WIDTH 50
#define SET_IC_REC_HEIGHT 30
#define SET_IC_REC_RAD 5

#define SET_IC_EDG_OFFSET_SX_1 25
#define SET_IC_EDG_OFFSET_SX_2 5
#define SET_IC_EDG_OFFSET_SY_1 50
#define SET_IC_EDG_OFFSET_SY_2 85
#define SET_IC_EDG_OFFSET_SY_3 15
#define SET_IC_EDG_OFFSET_SY_4 15
#define SET_IC_EDG_OFFSET_EX 5
#define SET_IC_EDG_OFFSET_EY_1 85
#define SET_IC_EDG_OFFSET_EY_2 50
#define SET_IC_SIGN_COLOR 255

#define SET_IC_TXT_OFFSET_X 160
#define SET_IC_TXT_OFFSET_Y 85

#define IC_POSITION_X_1 180
#define IC_POSITION_X_2 240
#define IC_POSITION_Y 120
#define IC_POSITION_Y_SCALE_1 660 / 65535.0
#define IC_POSITION_Y_SCALE_2 329 / 32767.5
#define IC_POSITION_X_SCALE 910 / 65535.0
#define IC_POSITION_Y_1_OFFSET 70
#define IC_POSITION_Y_2_OFFSET 80
#define IC_POSITION_X_2_OFFSET 820
#define IC_POSITION_X_3_OFFSET 1150

#define SLIDER_HALF_RANGE 32767.5
#define SLIDER_FULL_RANGE 65535
#define SLIDER_2_3_RANGE 20000
#define TRACKER_MASK 0xFF
#define SLIDER_TAG 8
#define SHIFT_VAL 16
#define SLIDER_COLOR 255
#define SLIDER_X 70
#define SLIDER_Y 27
#define SLIDER_WIDTH 340
#define SLIDER_HEIGHT 12

#define CALC_INPUT_BUFF_SIZE 10
#define CALC_DISPLAY_BUFF_SIZE 10
#define CALC_INPUT_BUFF_OFFSET 1
#define CALC_GRAD_COLOR_1 255
#define CALC_GRAD_COLOR_2 126
#define CALC_GRAD_COLOR_3 126
#define CALC_DISPLAY_FRAME_X 10
#define CALC_DISPLAY_FRAME_Y 10
#define CALC_DISPLAY_FRAME_WIDTH 460
#define CALC_DISPLAY_FRAME_HEIGHT 110
#define CALC_DISPLAY_FRAME_RAD 5 
#define CALC_DISPLAY_FRAME_EDGE_WIDTH 4
#define CALC_ESC_BUTTON_COLOR_1 47
#define CALC_ESC_BUTTON_COLOR_2 51
#define CALC_ESC_BUTTON_COLOR_3 62
#define CALC_ESC_BUTTON_X 20
#define CALC_ESC_BUTTON_Y 20
#define CALC_ESC_BUTTON_WIDTH 50
#define CALC_ESC_BUTTON_HEIGHT 20
#define CALC_ESC_BUTTON_TXT_SIZE 20

#define CALC_FIRST_ROW_KEYS_X 5
#define CALC_FIRST_ROW_KEYS_Y 128
#define CALC_FIRST_ROW_KEYS_WIDTH 400
#define CALC_FIRST_ROW_KEYS_HEIGHT 43
#define CALC_FIRST_ROW_KEYS_TXT_SIZE 20
#define CALC_SECOND_ROW_KEYS_X 5
#define CALC_SECOND_ROW_KEYS_Y 174
#define CALC_SECOND_ROW_KEYS_WIDTH 400
#define CALC_SECOND_ROW_KEYS_HEIGHT 43
#define CALC_SECOND_ROW_KEYS_TXT_SIZE 20
#define CALC_THIRD_ROW_KEYS_X 5
#define CALC_THIRD_ROW_KEYS_Y 220
#define CALC_THIRD_ROW_KEYS_WIDTH 400
#define CALC_THIRD_ROW_KEYS_HEIGHT 43
#define CALC_THIRD_ROW_KEYS_TXT_SIZE 20

#define CALC_C_BUTTON_COLOR_1 47
#define CALC_C_BUTTON_COLOR_2 51
#define CALC_C_BUTTON_COLOR_3 62
#define CALC_C_BUTTON_X 407
#define CALC_C_BUTTON_Y 128
#define CALC_C_BUTTON_WIDTH 68
#define CALC_C_BUTTON_HEIGHT 43
#define CALC_C_BUTTON_TXT_SIZE 20

#define CALC_EC_BUTTON_X 407
#define CALC_EC_BUTTON_Y 174
#define CALC_EC_BUTTON_WIDTH 68
#define CALC_EC_BUTTON_HEIGHT 43
#define CALC_EC_BUTTON_TXT_SIZE 20

#define CALC_DOT_BUTTON_X 407
#define CALC_DOT_BUTTON_Y 220
#define CALC_DOT_BUTTON_WIDTH 68
#define CALC_DOT_BUTTON_HEIGHT 43
#define CALC_DOT_BUTTON_TXT_SIZE 20

#define CALC_DISPLAY_CHAR_COLOR 255
#define CALC_DISPLAY_CHAR_X 30
#define CALC_DISPLAY_CHAR_Y 45
#define CALC_DISPLAY_CHAR_SIZE 30

#define STPWCH_CLOCK_DEF_OPT 16384

#define STPWCH_CLOCK_1_X 90
#define STPWCH_CLOCK_1_Y 121
#define STPWCH_CLOCK_1_RAD 70

#define STPWCH_CLOCK_2_X 240
#define STPWCH_CLOCK_2_Y 121
#define STPWCH_CLOCK_2_RAD 70

#define STPWCH_CLOCK_3_X 390
#define STPWCH_CLOCK_3_Y 121
#define STPWCH_CLOCK_3_RAD 70

#define STPWCH_GRAD_COLOR_1 50
#define STPWCH_GRAD_COLOR_2 50
#define STPWCH_GRAD_COLOR_3 50
#define STPWCH_GRAD_COLOR_4 150
#define STPWCH_GRAD_COLOR_5 150
#define STPWCH_GRAD_COLOR_6 150
#define STPWCH_ESC_BUTTON_X 10
#define STPWCH_ESC_BUTTON_Y 10
#define STPWCH_ESC_BUTTON_WIDTH 50
#define STPWCH_ESC_BUTTON_HEIGHT 20
#define STPWCH_ESC_BUTTON_TXT_SIZE 20

#define STPWCH_START_BUTTON_TAG 9
#define STPWCH_START_BUTTON_X 40
#define STPWCH_START_BUTTON_Y 205
#define STPWCH_START_BUTTON_WIDTH 100
#define STPWCH_START_BUTTON_HEIGHT 50
#define STPWCH_START_BUTTON_TXT_SIZE 30

#define STPWCH_PAUSE_BUTTON_TAG 10
#define STPWCH_PAUSE_BUTTON_X 190
#define STPWCH_PAUSE_BUTTON_Y 205
#define STPWCH_PAUSE_BUTTON_WIDTH 100
#define STPWCH_PAUSE_BUTTON_HEIGHT 50
#define STPWCH_PAUSE_BUTTON_TXT_SIZE 30

#define STPWCH_RESET_BUTTON_TAG 11
#define STPWCH_RESET_BUTTON_X 340
#define STPWCH_RESET_BUTTON_Y 205
#define STPWCH_RESET_BUTTON_WIDTH 100
#define STPWCH_RESET_BUTTON_HEIGHT 50
#define STPWCH_RESET_BUTTON_TXT_SIZE 30

#define STPWTCH_TXT_COLOR 255

#define STPWCH_SEC_TXT_X 75
#define STPWCH_SEC_TXT_Y 132
#define STPWCH_SEC_TXT_SIZE 26

#define STPWCH_MIN_TXT_X 215
#define STPWCH_MIN_TXT_Y 132
#define STPWCH_MIN_TXT_SIZE 26

#define STPWCH_HR_TXT_X 365
#define STPWCH_HR_TXT_Y 132
#define STPWCH_HR_TXT_SIZE 26

#define SET_GRAD_COLOR_1 255
#define SET_GRAD_COLOR_2 47
#define SET_GRAD_COLOR_3 0
#define SET_GRAD_COLOR_4 39
#define SET_GRAD_COLOR_5 37
#define SET_GRAD_COLOR_6 30

#define SET_ESC_BUTTON_X 10
#define SET_ESC_BUTTON_Y 10
#define SET_ESC_BUTTON_WIDTH 50
#define SET_ESC_BUTTON_HEIGHT 20
#define SET_ESC_BUTTON_TXT_SIZE 20

#define SET_DIAL_1_TAG 6
#define SET_DIAL_2_TAG 7
#define SET_DIAL_1_X 90
#define SET_DIAL_1_Y 180
#define SET_DIAL_1_RAD 50

#define SET_DIAL_2_X 390
#define SET_DIAL_2_Y 180
#define SET_DIAL_2_RAD 50
#define SET_DIAL_TRACK_COEF 1

#define SET_GAUGE_1_X 170
#define SET_GAUGE_1_Y 100
#define SET_GAUGE_1_RAD 60
#define SET_GAUGE_1_MAJOR 5
#define SET_GAUGE_1_MINOR 4
#define SET_GAUGE_1_RANGE 100
#define SET_GAUGE_2_X 310
#define SET_GAUGE_2_Y 100
#define SET_GAUGE_2_RAD 60
#define SET_GAUGE_2_MAJOR 5
#define SET_GAUGE_2_MINOR 4
#define SET_GAUGE_2_RANGE 100

#define SET_GAUGE_GAUGE_1_LOWER_RANGE 100
#define SET_GAUGE_GAUGE_1_HIGHER_RANGE 65500
#define SET_BRIGHT_SCALE_1 128
#define SET_BRIGHT_SCALE_2 65535
#define SET_BRIGHT_VALUE 12
#define SET_GAUGE_1_BRIGHT_SCALE 100

void gradient_background( gradient_colors_t *color, uint16_t val );
void display_initialization();
void calc_ic( uint16_t x, uint16_t y );
void stopwatch_ic( uint16_t x, uint16_t y );
void settings_ic( uint16_t x, uint16_t y );
void set_init_values( gradient_colors_t *color, icon_position_t *position, slider_data_t *slider, flag_t *flag, calc_operators_t *operators,stopwatch_t *stopwatch, settings_t *settings );
void calculate_icon_position( icon_position_t *position, slider_data_t *slider );
void main_menu_slider(slider_data_t *slider);
void submenu_select( flag_t *flag, calc_operators_t *operators,gradient_colors_t *color  );
void calculator_app( flag_t *flag, calc_operators_t *operators );
void calculation( uint8_t tag, char *input_buffer, size_t input_max_len, char *display_buffer, size_t display_max_len, flag_t *flag, calc_operators_t *operators );
void stopwatch_app( flag_t *flag );
void settings_app( flag_t *flag, settings_t *settings,gradient_colors_t *color );
#endif