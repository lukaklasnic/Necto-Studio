#include "ft800_demo.h"

ft800_cfg_t cfg;
ft800_t ctx;
extern uint16_t cmdOffset;
stopwatch_t stopwatch;
settings_t settings;

void display_initialization()
{
    tp_drv_t drv;
    digital_out_t cs_pin;
    digital_out_t pin;

    cfg.cs_pin = PE8;
    cfg.sck_pin = PA5;
    cfg.miso_pin = PA6;
    cfg.mosi_pin = PA7;
    cfg.pd_pin = PE7;

    digital_out_init(&pin, PC8);
    ft800_default_cfg( &cfg );
    ft800_init( &ctx, &cfg, &drv );
}

void set_init_values( gradient_colors_t *color, icon_position_t *position, slider_data_t *slider, flag_t *flag, calc_operators_t *operators, stopwatch_t *stopwatch, settings_t *settings )
{
    position -> x1 = ICON_INIT_VALUE_X1;
    position -> x2 = ICON_INIT_VALUE_X2;
    position -> x3 = ICON_INIT_VALUE_X3;
    position -> y1 = ICON_INIT_VALUE_Y1;
    position -> y2 = ICON_INIT_VALUE_Y2;
    position -> y3 = ICON_INIT_VALUE_Y3;
    color ->alpha = ALPHA_VALUE_INIT;
    slider -> val = SLIDER_INIT_VALUE;
    flag -> app_enter = false;
    flag -> awaiting_second = false;  
    flag -> result_ready = false;
    operators -> operator_1 = 0.0;
    operators -> operator_2 = 0.0;
    operators -> current_op = 0.0;
    settings->g_angle_1=0x8000;
    settings->g_angle_2=0x8000;
    settings-> g_val_1=50;
    settings-> g_val_2=0;
}

void gradient_background( gradient_colors_t *color, uint16_t val )
{
    if ( val <= SLIDER_VALUE_RANGE_1 ) 
    {
        float t = ( float )val / SLIDER_VALUE_RANGE_1;
        color -> s_red = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
        color -> s_green = t * COLOR_INTERPOLATION_SCALE_COEF;
        color -> s_blue = 0;
        color -> e_red = t * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_green = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_blue = 0;
    }
    else if ( val <= SLIDER_VALUE_RANGE_2 ) 
    {
        float t = ( float )( val - SLIDER_VALUE_RANGE_1 ) / SLIDER_VALUE_RANGE_1;
        color -> s_red = 0;
        color -> s_green = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
        color -> s_blue = t * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_red = 0;
        color -> e_green = t * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_blue = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
    }
    else 
    {
        float t = ( float )( val - SLIDER_VALUE_RANGE_2 ) / SLIDER_VALUE_RANGE_1;
        color -> s_red = t * COLOR_INTERPOLATION_SCALE_COEF;
        color -> s_green = 0;
        color -> s_blue = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_red = ( COLOR_INTERPOLATION_COEF - t ) * COLOR_INTERPOLATION_SCALE_COEF;
        color -> e_green = 0;
        color -> e_blue = t * COLOR_INTERPOLATION_SCALE_COEF;
    }
    
    ft800_cmd( &ctx, FT800_SCISSOR_XY( 0, 0 ) );
    ft800_cmd( &ctx, FT800_SCISSOR_SIZE( TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT ) );
    ft800_cmd_gradient( &ctx, 0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, color -> s_red, color -> s_green, color -> s_blue, color -> e_red, color -> e_green, color -> e_blue );
}

void calc_ic( uint16_t x, uint16_t y )
{
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
    ft800_cmd( &ctx, FT800_TAG( CALC_IC_TAG ) );
    ft800_draw_edges_rectangle( &ctx, x, y, CALC_IC_REC_WIDTH, CALC_IC_REC_HEIGHT, CALC_IC_REC_EDG_RAD, EDG_COLOR,IC_EDG_WIDTH );
    ft800_cmd( &ctx, FT800_COLOR_RGB( 0, 0, CALC_IC_SIGN_COLOR ) );
    ft800_cmd_text( &ctx, x + CALC_IC_TXT_OFFSET_X_1, y + CALC_IC_TXT_OFFSET_Y_1, IC_TXT_SIZE, 0, "+" );
    ft800_cmd_text( &ctx, x + CALC_IC_TXT_OFFSET_X_2, y + CALC_IC_TXT_OFFSET_Y_2, IC_TXT_SIZE, 0, "-" );
    ft800_cmd_text( &ctx, x + CALC_IC_TXT_OFFSET_X_3, y + CALC_IC_TXT_OFFSET_Y_3, IC_TXT_SIZE, 0, "x" );
    ft800_cmd_text( &ctx, x + CALC_IC_TXT_OFFSET_X_4, y + CALC_IC_TXT_OFFSET_Y_4, IC_TXT_SIZE, 0, "=" );
    ft800_cmd_line( &ctx, x + CALC_IC_EDG_OFFSET_SX_1, y, x + CALC_IC_EDG_OFFSET_EX_1, y + CALC_IC_EDG_OFFSET_EY_1, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd_line( &ctx, x, y + CALC_IC_EDG_OFFSET_SY_2, x + CALC_IC_EDG_OFFSET_EX_2, y + CALC_IC_EDG_OFFSET_EY_2, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
    ft800_cmd( &ctx, FT800_COLOR_RGB( 0, 0, CALC_IC_SIGN_COLOR ) );
    ft800_cmd_text( &ctx, x - CALC_IC_TXT_OFFSET_X_5, y + CALC_IC_TXT_OFFSET_Y_5, IC_TXT_SIZE, 0, "Calculator" );
}

void stopwatch_ic( uint16_t x, uint16_t y )
{
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
    ft800_cmd( &ctx, FT800_TAG( STPWCH_IC_TAG ) );
    ft800_draw_edges_circle( &ctx, x, y, STPWCH_IC_CRC_RAD_1, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd_line( &ctx, x - STPWCH_IC_EDG_OFFSET_SX_1, y - STPWCH_IC_EDG_OFFSET_SY_1, x + STPWCH_IC_EDG_OFFSET_EX_1, y - STPWCH_IC_EDG_OFFSET_EY_1, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd_line( &ctx, x, y, x, y - STPWCH_IC_EDG_OFFSET_EY_2, EDG_COLOR, IC_EDG_WIDTH );
    ft800_draw_edges_circle( &ctx, x, y, STPWCH_IC_CRC_RAD_2, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
    ft800_cmd( &ctx, FT800_COLOR_RGB( STPWCH_IC_SIGN_COLOR, 0, 0 ) );
    ft800_cmd_text( &ctx, x - STPWCH_IC_TXT_OFFSET_X, y + STPWCH_IC_TXT_OFFSET_Y, IC_TXT_SIZE, 0, "Stopwatch" );
}

void settings_ic( uint16_t x, uint16_t y )
{
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
    ft800_cmd( &ctx, FT800_TAG( SET_IC_TAG ) );
    ft800_draw_edges_circle( &ctx, x, y, SET_IC_CRC_RAD_1, EDG_COLOR, IC_EDG_WIDTH );
    ft800_draw_edges_rectangle( &ctx, x - SET_IC_EDG_OFFSET_SX_1, y + SET_IC_EDG_OFFSET_SY_1, SET_IC_REC_WIDTH, SET_IC_REC_HEIGHT, SET_IC_REC_RAD, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd_line( &ctx, x - SET_IC_EDG_OFFSET_SX_2, y + SET_IC_EDG_OFFSET_SY_2, x + SET_IC_EDG_OFFSET_EX, y + SET_IC_EDG_OFFSET_EY_1, EDG_COLOR, 5 );
    ft800_cmd_line( &ctx, x, y + SET_IC_EDG_OFFSET_SY_3, x, y + SET_IC_EDG_OFFSET_EY_2, EDG_COLOR, IC_EDG_WIDTH );
    ft800_draw_edges_circle( &ctx, x, y + SET_IC_EDG_OFFSET_SY_4, SET_IC_CRC_RAD_2, EDG_COLOR, IC_EDG_WIDTH );
    ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
    ft800_cmd( &ctx, FT800_COLOR_RGB( 0, SET_IC_SIGN_COLOR, 0 ) );
    ft800_cmd_text( &ctx, x - SET_IC_TXT_OFFSET_X, y + SET_IC_TXT_OFFSET_Y, IC_TXT_SIZE, 0, "Brightness control" );
}

void calculate_icon_position( icon_position_t *position, slider_data_t *slider )
{
        if ( position -> x1 == IC_POSITION_X_1 )
        {
            position -> y1 = ( uint16_t )( IC_POSITION_Y_1_OFFSET + ( IC_POSITION_Y_SCALE_1 ) * slider -> val );
        }

        if( position -> x2 >= IC_POSITION_X_2 || position -> y2 == IC_POSITION_Y )
        {
            position -> x2 = ( uint16_t )( IC_POSITION_X_2_OFFSET - ( TFT_DISPLAY_WIDTH / SLIDER_HALF_RANGE ) * slider -> val );
            position -> y2 = IC_POSITION_Y;
        }

        if ( position -> x2 <= IC_POSITION_X_2 )
        {
            position -> y2 = ( uint16_t )( IC_POSITION_Y_2_OFFSET + ( IC_POSITION_Y_SCALE_2 ) * ( slider -> val - SLIDER_HALF_RANGE ) );
            position -> x2 = IC_POSITION_X_2;
        }
  
        if ( position -> x3 >= IC_POSITION_X_2 )
        {
            position -> x3 = ( uint16_t )( IC_POSITION_X_3_OFFSET - ( IC_POSITION_X_SCALE ) * slider -> val );
        }

        calc_ic( position-> x1, position-> y1 );
        //memset( input_buffer, 0, sizeof( input_buffer ) );
        //memset( display_buffer, 0, sizeof( display_buffer ) );
        if( position -> y2 <= TFT_DISPLAY_HEIGHT && position-> x2 >= IC_POSITION_X_2 && position-> x3 >= TFT_DISPLAY_WIDTH )
        {
            stopwatch_ic( position-> x2, position-> y2 );
        }

        if ( slider-> val >= SLIDER_2_3_RANGE )
        {
            settings_ic( position-> x3, position-> y3 );
        }
}

void main_menu_slider(slider_data_t *slider)
{
        slider -> tracker =  ft800_read_32_bits( &ctx, FT800_REG_TRACKER);
        if ( ( slider -> tracker & TRACKER_MASK ) == SLIDER_TAG )
        {
            slider -> val= slider -> tracker >> SHIFT_VAL;

        }
     
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( SLIDER_TAG ) );
        ft800_cmd( &ctx, FT800_COLOR_RGB( SLIDER_COLOR, 0, 0 ) );
        ft800_cmd_slider( &ctx, SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, 0, slider -> val, SLIDER_FULL_RANGE );
        ft800_cmd_track( &ctx, SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, SLIDER_TAG );
}
void submenu_select( flag_t *flag, calc_operators_t *operators,gradient_colors_t *color  )
{
    
    if ( ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG) ) == CALC_IC_TAG )
    {
        flag -> app_enter = true;
        calculator_app( flag, operators  );
    }
    if ( (ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG) ) == 3 )
    {
        flag -> app_enter = true;
        stopwatch_app( &flag );
    }
    if ( ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) ) == 4 )
    {
        flag -> app_enter = true;
         settings_app( &flag, &settings, &color  );
    }
}

void calculator_app( flag_t *flag, calc_operators_t *operators )
{
    uint8_t current_tag = 0;
    uint8_t prev_tag = 0;
    char input_buffer[ CALC_INPUT_BUFF_SIZE ] = { 0 };    
    char display_buffer[ CALC_DISPLAY_BUFF_SIZE ] = { 0 };
    
    while ( flag -> app_enter )
    {
        ft800_start_display_list( &ctx );
        ft800_cmd( &ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( &ctx, FT800_SCISSOR_SIZE( TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT ) );
        ft800_cmd_gradient( &ctx, 0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, 0, 0, CALC_GRAD_COLOR_1, 0, CALC_GRAD_COLOR_2, CALC_GRAD_COLOR_2 );
        
        ft800_draw_edges_rectangle( &ctx, CALC_DISPLAY_FRAME_X, CALC_DISPLAY_FRAME_Y, CALC_DISPLAY_FRAME_WIDTH, CALC_DISPLAY_FRAME_HEIGHT, CALC_DISPLAY_FRAME_RAD, EDG_COLOR, CALC_DISPLAY_FRAME_EDGE_WIDTH );
        
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( ESC_BUTTON_TAG ) );
        ft800_cmd( &ctx, FT800_COLOR_RGB( CALC_ESC_BUTTON_COLOR_1, CALC_ESC_BUTTON_COLOR_2, CALC_ESC_BUTTON_COLOR_3 ) ); 
        ft800_cmd_button( &ctx, CALC_ESC_BUTTON_X, CALC_ESC_BUTTON_Y, CALC_ESC_BUTTON_WIDTH, CALC_ESC_BUTTON_HEIGHT, CALC_ESC_BUTTON_TXT_SIZE, 0, "ESC" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        
        ft800_cmd_keys( &ctx,  CALC_FIRST_ROW_KEYS_X, CALC_FIRST_ROW_KEYS_Y, CALC_FIRST_ROW_KEYS_WIDTH, CALC_FIRST_ROW_KEYS_HEIGHT, CALC_FIRST_ROW_KEYS_TXT_SIZE, 0, "123+-" );
        ft800_cmd_keys( &ctx,  CALC_SECOND_ROW_KEYS_X, CALC_SECOND_ROW_KEYS_Y, CALC_SECOND_ROW_KEYS_WIDTH, CALC_SECOND_ROW_KEYS_HEIGHT, CALC_SECOND_ROW_KEYS_TXT_SIZE, 0, "456x/" );
        ft800_cmd_keys( &ctx,  CALC_THIRD_ROW_KEYS_X, CALC_THIRD_ROW_KEYS_Y, CALC_THIRD_ROW_KEYS_WIDTH, CALC_THIRD_ROW_KEYS_HEIGHT, CALC_THIRD_ROW_KEYS_TXT_SIZE, 0, "7890=" );
        
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( 'C' ) );
        ft800_cmd( &ctx, FT800_COLOR_RGB( CALC_C_BUTTON_COLOR_1, CALC_C_BUTTON_COLOR_2 ,CALC_C_BUTTON_COLOR_3 ) ); 
        ft800_cmd_button( &ctx, CALC_C_BUTTON_X, CALC_C_BUTTON_Y, CALC_C_BUTTON_WIDTH, CALC_C_BUTTON_HEIGHT, CALC_C_BUTTON_TXT_SIZE, 0, "C" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( 'C' + 'E' ) );
        ft800_cmd_button( &ctx, CALC_EC_BUTTON_X, CALC_EC_BUTTON_Y, CALC_EC_BUTTON_WIDTH, CALC_EC_BUTTON_HEIGHT, CALC_EC_BUTTON_TXT_SIZE, 0, "CE" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( '.' ) );
        ft800_cmd_button( &ctx, CALC_DOT_BUTTON_X, CALC_DOT_BUTTON_Y, CALC_DOT_BUTTON_WIDTH, CALC_DOT_BUTTON_HEIGHT, CALC_DOT_BUTTON_TXT_SIZE, 0, "." );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );

        current_tag = ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG );

        if( current_tag == ESC_BUTTON_TAG )
        {
            flag -> app_enter = false;
            memset( input_buffer, 0, sizeof( input_buffer ) );
            memset( display_buffer, 0, sizeof( display_buffer ) );
        }
        
        if ( current_tag != 0 && current_tag != prev_tag ) 
        {
            calculation( current_tag, input_buffer, sizeof(input_buffer), display_buffer, sizeof(display_buffer), flag, operators );
        }

        ft800_cmd( &ctx, FT800_COLOR_RGB( CALC_DISPLAY_CHAR_COLOR, CALC_DISPLAY_CHAR_COLOR, CALC_DISPLAY_CHAR_COLOR ) );
        ft800_cmd_text( &ctx, CALC_DISPLAY_CHAR_X, CALC_DISPLAY_CHAR_Y, CALC_DISPLAY_CHAR_SIZE, 0, display_buffer );
        
        prev_tag = current_tag;
        ft800_end_display_list (&ctx ); 
    }
}

void calculation( uint8_t tag, char *input_buffer, size_t input_max_len, char *display_buffer, size_t display_max_len, flag_t *flag, calc_operators_t *operators ) 
{
    if ( ( tag >= '0' && tag <= '9' ) || tag == '.' ) 
    {
        if ( strlen( input_buffer ) < input_max_len - CALC_INPUT_BUFF_OFFSET ) 
        {
            if ( tag != '.' || strchr( input_buffer, '.' ) == NULL ) 
            {
                size_t len = strlen( input_buffer );
                input_buffer[ len ] = tag;
                input_buffer[ len + CALC_INPUT_BUFF_OFFSET ] = '\0';
            }
        }

        if ( flag -> awaiting_second ) 
        {
            snprintf( display_buffer, display_max_len, "%g%c%s", ( float )operators -> operator_1, operators -> current_op, input_buffer );
        } else 
        {
            snprintf( display_buffer, display_max_len, "%s", input_buffer );
        }
    } 
    else if ( tag == '+' || tag == '-' || tag == 'x' || tag == '/' ) 
    {
        if ( !flag -> awaiting_second && strlen( input_buffer ) > 0 ) 
        {
            operators -> operator_1 = atof( input_buffer );  
            operators -> current_op = tag;
            flag -> awaiting_second = true;
            input_buffer[ 0 ] = '\0';  
            snprintf( display_buffer, display_max_len, "%g%c", operators -> operator_1, operators -> current_op );
        }
    } 
    else if ( tag == '=' ) 
    {
        if ( flag -> awaiting_second && strlen( input_buffer ) > 0 ) 
        {
            operators -> operator_2 = atof( input_buffer );
            float result = 0.0f;
            bool error = false;

            switch ( operators -> current_op ) 
            {
                case '+': 
                    result = operators -> operator_1 + operators -> operator_2; 
                    break;
                case '-': 
                    result = operators -> operator_1 - operators -> operator_2; 
                    break;
                case 'x': 
                    result = operators -> operator_1 * operators -> operator_2; 
                    break;
                case '/': 
                    if ( operators -> operator_2 != 0.0f ) 
                    {
                        result = operators -> operator_1 / operators -> operator_2;
                    } 
                    else 
                    {
                        error = true;
                    }
                    break;
            }

            if ( error ) 
            {
                snprintf( input_buffer, input_max_len, "INF" );
                snprintf( display_buffer, display_max_len, "INF" );
            } else 
            {
                snprintf( input_buffer, input_max_len, "%g", result );
                snprintf( display_buffer, display_max_len, "%g", result );
            }
            flag -> awaiting_second = false;
            flag -> result_ready = true;
        }
    } 
    else if ( tag == 'C' ) 
    {
        operators -> operator_1 = operators -> operator_2 = 0.0f;
        operators -> current_op = 0;
        flag -> awaiting_second = false;
        flag -> result_ready = false;
        input_buffer[ 0 ] = '\0';
        display_buffer[ 0 ] = '\0';
    } 
    else if ( tag == ('C' + 'E') ) 
    {  
        size_t len = strlen( input_buffer );
        if ( len > 0 ) 
        {
            input_buffer[ len - CALC_INPUT_BUFF_OFFSET ] = '\0';
            if ( flag -> awaiting_second )
            {
                snprintf( display_buffer, display_max_len, "%g%c%s", operators -> operator_1, operators -> current_op, input_buffer );
            } else {
                snprintf( display_buffer, display_max_len, "%s", input_buffer );
            }
        }
    }
}

void stopwatch_app( flag_t *flag )
{
    stopwatch.h=0; 
    stopwatch.m=0; 
    stopwatch.s=0; 
    stopwatch.h_mem=0; 
    stopwatch.s_mem=0; 
    stopwatch.m_mem=0;

    timer6_init( 1000 );

    while( flag -> app_enter )
    {
        ft800_start_display_list( &ctx );
        ft800_cmd( &ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( &ctx, FT800_SCISSOR_SIZE( TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT ) );
        ft800_cmd_gradient( &ctx, 0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, STPWCH_GRAD_COLOR_1, STPWCH_GRAD_COLOR_2, STPWCH_GRAD_COLOR_3, STPWCH_GRAD_COLOR_4, STPWCH_GRAD_COLOR_5, STPWCH_GRAD_COLOR_6 );
        //ft800_cmd( &ctx, FT800_COLOR_A( alpha ) );
        ft800_cmd_clock( &ctx, STPWCH_CLOCK_1_X, STPWCH_CLOCK_1_Y, STPWCH_CLOCK_1_RAD, STPWCH_CLOCK_DEF_OPT, stopwatch.hr, 0, stopwatch.h, 0 );
        ft800_cmd_clock( &ctx, STPWCH_CLOCK_2_X, STPWCH_CLOCK_2_Y, STPWCH_CLOCK_2_RAD, STPWCH_CLOCK_DEF_OPT, stopwatch.hr, 0, stopwatch.m, 0 );
        ft800_cmd_clock( &ctx, STPWCH_CLOCK_3_X, STPWCH_CLOCK_3_Y, STPWCH_CLOCK_3_RAD, STPWCH_CLOCK_DEF_OPT, stopwatch.hr, 0, stopwatch.s, 0 );
        
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( ESC_BUTTON_TAG ) );
        ft800_cmd_button( &ctx, STPWCH_ESC_BUTTON_X, STPWCH_ESC_BUTTON_Y, STPWCH_ESC_BUTTON_WIDTH, STPWCH_ESC_BUTTON_HEIGHT, STPWCH_ESC_BUTTON_TXT_SIZE, 0, "ESC" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( STPWCH_START_BUTTON_TAG ) );
        ft800_cmd_button( &ctx, STPWCH_START_BUTTON_X, STPWCH_START_BUTTON_Y, STPWCH_START_BUTTON_WIDTH, STPWCH_START_BUTTON_HEIGHT, STPWCH_START_BUTTON_TXT_SIZE, 0, "Start" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( STPWCH_PAUSE_BUTTON_TAG ) );
        ft800_cmd_button( &ctx, STPWCH_PAUSE_BUTTON_X, STPWCH_PAUSE_BUTTON_Y, STPWCH_PAUSE_BUTTON_WIDTH, STPWCH_PAUSE_BUTTON_HEIGHT, STPWCH_PAUSE_BUTTON_TXT_SIZE, 0, "Pause" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( STPWCH_RESET_BUTTON_TAG ) );
        ft800_cmd_button( &ctx, STPWCH_RESET_BUTTON_X, STPWCH_RESET_BUTTON_Y, STPWCH_RESET_BUTTON_WIDTH, STPWCH_RESET_BUTTON_HEIGHT, STPWCH_RESET_BUTTON_TXT_SIZE, 0, "Reset" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd( &ctx, FT800_COLOR_RGB( STPWTCH_TXT_COLOR, STPWTCH_TXT_COLOR, STPWTCH_TXT_COLOR ) );
        ft800_cmd_text( &ctx, STPWCH_SEC_TXT_X, STPWCH_SEC_TXT_Y, STPWCH_SEC_TXT_SIZE, 0, "Hour" );
        ft800_cmd_text( &ctx, STPWCH_MIN_TXT_X, STPWCH_MIN_TXT_Y, STPWCH_MIN_TXT_SIZE, 0, "Minutes" );
        ft800_cmd_text( &ctx, STPWCH_HR_TXT_X, STPWCH_MIN_TXT_Y, STPWCH_HR_TXT_SIZE, 0, "Seconds" );

        if ( ft800_read_8_bits( &ctx,  FT800_REG_TOUCH_TAG ) == ESC_BUTTON_TAG )
        {
            flag -> app_enter = false;
            flag -> stpwch_reset = false;
            stopwatch.hr = 0;
            stopwatch.sec = 0; 
            stopwatch.min = 0;
        }
        if ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) == STPWCH_START_BUTTON_TAG )
        {
            if( flag -> stpwch_reset == false )
            {
                flag -> stpwch_reset = true;
                stopwatch.hr = 0;
                stopwatch.min = 0; 
                stopwatch.sec = 0;
            }
            flag ->stpwch_pause = true;
            stopwatch.hr = stopwatch.h_mem;
            stopwatch.min = stopwatch.m_mem;
            stopwatch.sec = stopwatch.s_mem;
        }

        if ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) == STPWCH_PAUSE_BUTTON_TAG )
        {
            flag -> stpwch_pause = false;
            stopwatch.s_mem = stopwatch.s;
            stopwatch.m_mem = stopwatch.m;
            stopwatch.h_mem = stopwatch.h;
        }
    
        if ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) == STPWCH_RESET_BUTTON_TAG )
        {
            stopwatch.h = 0;
            stopwatch.m = 0;
            stopwatch.s = 0;
            stopwatch.hr = 0;
            stopwatch.min = 0;
            stopwatch.sec = 0;
        }
        
        if ( flag -> stpwch_reset && flag -> stpwch_pause )
        {
            stopwatch.h = stopwatch.hr;
            stopwatch.m = stopwatch.min;
            stopwatch.s = stopwatch.sec;
        }

        ft800_end_display_list(&ctx);            
    }
}

void settings_app( flag_t *flag, settings_t *settings,gradient_colors_t *color )
{
    while ( flag -> app_enter )
    {
        ft800_start_display_list( &ctx );
        ft800_cmd( &ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( &ctx, FT800_SCISSOR_SIZE( TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT ) );
        ft800_cmd_gradient( &ctx, 0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, SET_GRAD_COLOR_1, SET_GRAD_COLOR_2, SET_GRAD_COLOR_3, SET_GRAD_COLOR_4, SET_GRAD_COLOR_5, SET_GRAD_COLOR_6 );
        //ft800_cmd( ctx, FT800_COLOR_A( alpha ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( ESC_BUTTON_TAG ) );
        
        ft800_cmd_button( &ctx, SET_ESC_BUTTON_X, SET_ESC_BUTTON_Y, SET_ESC_BUTTON_WIDTH, SET_ESC_BUTTON_HEIGHT, SET_ESC_BUTTON_TXT_SIZE, 0, "ESC" );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        settings->tracker=ft800_read_8_bits( &ctx, FT800_REG_TRACKER );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( SET_DIAL_1_TAG ) );
        ft800_cmd_dial( &ctx, SET_DIAL_1_X, SET_DIAL_1_Y, SET_DIAL_1_RAD, 0, settings->g_angle_1 );
        ft800_cmd_track( &ctx, SET_DIAL_1_X, SET_DIAL_1_Y, SET_DIAL_TRACK_COEF, SET_DIAL_TRACK_COEF, SET_DIAL_1_TAG );
        ft800_cmd( &ctx, FT800_TAG( SET_DIAL_1_TAG ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_SET ) );
        ft800_cmd( &ctx, FT800_TAG( SET_DIAL_2_TAG ) );
        ft800_cmd_dial( &ctx, SET_DIAL_2_X, SET_DIAL_2_Y, SET_DIAL_2_RAD, 0, settings->g_angle_2 );
        ft800_cmd_track( &ctx, SET_DIAL_2_X, SET_DIAL_2_Y, SET_DIAL_TRACK_COEF, SET_DIAL_TRACK_COEF, SET_DIAL_2_TAG );
        ft800_cmd( &ctx, FT800_TAG( SET_DIAL_2_TAG ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( TAG_MASK_RESET ) );
        ft800_cmd_gauge( &ctx, SET_GAUGE_1_X, SET_GAUGE_1_Y,SET_GAUGE_1_RAD , 0, SET_GAUGE_1_MAJOR, SET_GAUGE_1_MINOR, settings->g_val_1, SET_GAUGE_1_RANGE );
        ft800_cmd_gauge( &ctx, SET_GAUGE_2_X, SET_GAUGE_2_Y, SET_GAUGE_2_RAD, 0, SET_GAUGE_2_MAJOR, SET_GAUGE_2_MINOR, settings->g_val_1, SET_GAUGE_2_RANGE );
                
        if ( ( settings->tracker & TRACKER_MASK ) == SET_DIAL_1_TAG && settings->g_angle_1 >= SET_GAUGE_GAUGE_1_LOWER_RANGE && settings->g_angle_1 <= SET_GAUGE_GAUGE_1_HIGHER_RANGE )
        {
            settings->g_angle_1 = settings->tracker >> SHIFT_VAL;
            if ( settings->g_angle_1 > SET_GAUGE_GAUGE_1_HIGHER_RANGE )
            {
                settings->g_angle_1 = SET_GAUGE_GAUGE_1_HIGHER_RANGE;
            }
            if (settings-> g_angle_1 < SET_GAUGE_GAUGE_1_LOWER_RANGE)
            {
                settings->g_angle_1 = SET_GAUGE_GAUGE_1_LOWER_RANGE;
            }
        }
        if ( ( settings->tracker & TRACKER_MASK ) == SET_DIAL_2_TAG )
        {
            settings->g_angle_2 = settings->tracker >> SHIFT_VAL;
        }
        uint8_t bright = ( uint8_t )( SET_BRIGHT_SCALE_1 * ( ( float )settings->g_angle_1 / SET_BRIGHT_SCALE_2) );
        if ( bright < SET_BRIGHT_VALUE )
        {
            bright = SET_BRIGHT_VALUE;
        }
        ft800_write_8_bits( &ctx, 0x1024C4, bright);
        settings->g_val_1 = ( uint16_t )( SET_GAUGE_1_BRIGHT_SCALE * ( ( float )bright / SET_BRIGHT_SCALE_1 ) );
        color->alpha = ( uint8_t )( 255.0* ( ( float )settings->g_angle_2 / 65535.0 ) );
        if ( color->alpha < 26 )
        {
            color->alpha = 26;
        }
        settings->g_val_2 = ( uint8_t )( 100.0 * ( ( float )color->alpha / 255.0 ) );
        if ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) == ESC_BUTTON_TAG )
        {
            flag -> app_enter = false;
        }
        ft800_end_display_list( &ctx );
                
    }
}