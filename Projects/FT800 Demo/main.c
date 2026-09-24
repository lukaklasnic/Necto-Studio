/**
 * @file main.c
 * @brief Main function for New_FT800_Demo application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif
#include "MikroSDK.Ft800"
#include "MikroSDK.Board"
#include "MikroSDK.Driver"
#include "MikroSDK.Conversions"
#include "MikroSDK.Driver.GPIO.Out"
#include "Stdio.h"



char input_buffer[ 10 ] = { 0 };   
char display_buffer[ 10 ] = { 0 };

bool awaiting_second = false;  
bool result_ready = false;

float operand1 = 0.0;
float operand2 = 0.0;
char current_op = 0; 
uint8_t alpha = 255;
uint8_t prev_tag = 0;
uint8_t  sec = 0;
uint8_t min = 0;
uint8_t hr = 0; 

bool flag1 = true;
bool flag2 = false;
bool flag3=false;
bool flag4=true;

static uint32_t msCount = 0;
uint32_t timeout = 0;

uint16_t ticks_global = 0;


void gradient_background( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, uint8_t s_red, uint8_t s_green, uint8_t s_blue, uint8_t e_red, uint8_t e_green, uint8_t e_blue, uint16_t val )
{
    if ( val <= 21845 ) 
    {
        float t = ( float )val / 21845.0;
        s_red = ( 1.0 - t ) * 255;
        s_green = t * 255;
        s_blue = 0;
        e_red = t * 255;
        e_green = ( 1.0 - t ) * 255;
        e_blue = 0;
    }
    else if ( val <= 43690 ) 
    {
        float t = ( float )( val - 21845 ) / 21845.0;
        s_red = 0;
        s_green = ( 1.0 - t ) * 255;
        s_blue = t * 255;
        e_red = 0;
        e_green = t * 255;
        e_blue = ( 1.0 - t ) * 255;
    }
    else 
    {
        float t = ( float )( val - 43690 ) / 21845.0;
        s_red = t * 255;
        s_green = 0;
        s_blue = ( 1.0 - t ) * 255;
        e_red = ( 1.0 - t ) * 255;
        e_green = 0;
        e_blue = t * 255;
    }
    
    ft800_cmd( ctx, FT800_SCISSOR_XY( 0, 0 ) );
    ft800_cmd( ctx, FT800_SCISSOR_SIZE( 480, 272 ) );
    ft800_cmd_gradient( ctx, 0, 0, 480, 272, s_red, s_green, s_blue, e_red, e_green, e_blue );
}

void calc_ic( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, uint16_t x, uint16_t y )
{
    ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
    ft800_cmd( ctx, FT800_TAG( 2 ) );
    ft800_draw_edges_rectangle( ctx, x, y, 120, 120, 10, 0x0000,5 );
    ft800_cmd( ctx, FT800_COLOR_RGB( 0, 0, 255 ) );
    ft800_cmd_text( ctx, x + 20, y + 7, 31, 0, "+" );
    ft800_cmd_text( ctx, x + 80, y + 7, 31, 0, "-" );
    ft800_cmd_text( ctx, x + 20, y + 65, 31, 0, "x" );
    ft800_cmd_text( ctx, x + 78, y + 65, 31, 0, "=" );
    ft800_cmd_line( ctx, x + 60, y, x + 60, y + 120, 0x0000, 5 );
    ft800_cmd_line( ctx, x, y + 60, x + 120, y + 60, 0x0000, 5 );
    ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    ft800_cmd( ctx, FT800_COLOR_RGB( 0, 0, 255 ) );
    ft800_cmd_text( ctx, x - 30, y + 130, 31, 0, "Calculator" );
}

void stopwatch_ic( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, uint16_t x, uint16_t y )
{
    ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
    ft800_cmd( ctx, FT800_TAG( 3 ) );
    ft800_draw_edges_circle( ctx, x, y, 120, 0x0000, 5 );
    ft800_cmd_line( ctx, x - 10, y - 70, x + 10, y - 70, 0x0000, 5 );
    ft800_cmd_line( ctx, x, y, x, y - 45, 0x0000, 5 );
    ft800_draw_edges_circle( ctx, x, y, 10, 0x0000, 5 );
    ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    ft800_cmd( ctx, FT800_COLOR_RGB( 255, 0, 0 ) );
    ft800_cmd_text( ctx, x - 95, y + 70, 31, 0, "Stopwatch" );
}

void settings_ic( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, uint16_t x, uint16_t y )
{
    ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
    ft800_cmd( ctx, FT800_TAG( 4 ) );
    ft800_draw_edges_circle( ctx, x, y, 100, 0x0000, 5 );
    ft800_draw_edges_rectangle( ctx, x - 25, y + 50, 50, 30, 5, 0x0000, 4 );
    ft800_cmd_line( ctx, x - 5, y + 85, x + 5, y + 85, 0x0000, 5 );
    ft800_cmd_line( ctx, x, y + 15, x, y + 50, 0x0000, 3 );
    ft800_draw_edges_circle( ctx, x, y + 15, 5, 0x0000, 5 );
    ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    ft800_cmd( ctx, FT800_COLOR_RGB( 0, 255, 0 ) );
    ft800_cmd_text( ctx, x - 160, y + 85, 31, 0, "Brightness control" );
}

void calculation( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, uint8_t tag ) 
{
    if ( ( tag >= '0' && tag <= '9' ) || tag == '.' ) 
    {
        if ( strlen( input_buffer ) < sizeof( input_buffer ) - 1 ) 
        {
            if ( tag != '.' || strchr( input_buffer, '.' ) == NULL ) 
            {
                size_t len = strlen( input_buffer );
                input_buffer[ len ] = tag;
                input_buffer[ len + 1 ] = '\0';
            }
        }

        if ( awaiting_second ) 
        {
            snprintf( display_buffer, sizeof( display_buffer ), "%g%c%s", ( float )operand1, current_op, input_buffer );
        } else 
        {
            snprintf( display_buffer, sizeof( display_buffer ), "%s", input_buffer );
        }
    } 
    else if ( tag == '+' || tag == '-' || tag == 'x' || tag == '/' ) 
    {
        if ( !awaiting_second && strlen( input_buffer ) > 0 ) 
        {
            operand1 = atof( input_buffer );  
            current_op = tag;
            awaiting_second = true;
            input_buffer[ 0 ] = '\0';  
            snprintf( display_buffer, sizeof( display_buffer ), "%g%c", operand1, current_op );
        }
    } 
    else if ( tag == '=' ) 
    {
        if ( awaiting_second && strlen( input_buffer ) > 0 ) 
        {
            operand2 = atof( input_buffer );
            float result = 0.0f;
            bool division = false;
            bool error = false;

            switch ( current_op ) 
            {
                case '+': 
                    result = operand1 + operand2; 
                    break;
                case '-': 
                    result = operand1 - operand2; 
                    break;
                case 'x': 
                    result = operand1 * operand2; 
                    break;
                case '/': 
                    division = true;
                    if ( operand2 != 0.0f ) 
                    {
                        result = operand1 / operand2;
                    } 
                    else 
                    {
                        error = true;
                    }
                    break;
            }

            if ( error ) 
            {
                strcpy( input_buffer, "INFINITELY" );
                strcpy( display_buffer, "INFINITELY" );
            } else 
            {
                snprintf( input_buffer, sizeof( input_buffer ), "%g", result );
                memset( input_buffer, 0, sizeof( input_buffer ) );
                snprintf( display_buffer, sizeof( display_buffer ), "%g", result );
            }
            awaiting_second = false;
            result_ready = true;
        }
    } 
    else if ( tag == 'C' ) 
    {
        operand1 = operand2 = 0.0f;
        current_op = 0;
        awaiting_second = false;
        result_ready = false;
        input_buffer[ 0 ] = '\0';
        display_buffer[ 0 ] = '\0';
    } 
    else if ( tag == 'C'+'E' ) 
    {  
        size_t len = strlen( input_buffer );
        if ( len > 0 ) 
        {
            input_buffer[ len - 1 ] = '\0';
            if ( awaiting_second )
            {
                snprintf( display_buffer, sizeof( display_buffer ), "%g%c%s", operand1, current_op, input_buffer );
            } else {
                snprintf( display_buffer, sizeof( display_buffer ), "%s", input_buffer );
            }
        }
    }
}

void calculator( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, bool flag2 )
{
    while ( flag2 )
    {
        ft800_start_display_list( ctx );
        ft800_cmd( ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( ctx, FT800_SCISSOR_SIZE( 480, 272 ) );
        ft800_cmd_gradient( ctx, 0, 0, 480, 272, 0, 0, 255, 0, 126, 126 );
        ft800_cmd( ctx, FT800_COLOR_A( alpha ) );
        ft800_draw_edges_rectangle( ctx, 10, 10, 460, 110, 5, 0x0000, 4 );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 5 ) );
        ft800_cmd( ctx, FT800_COLOR_RGB( 47, 51, 62 ) ); 
        ft800_cmd_button( ctx, 20, 20, 50, 20, 20, 0, "ESC" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd_keys( ctx,  5, 128, 400, 43, 20, 0, "123+-" );
        ft800_cmd_keys( ctx,  5, 174, 400, 43, 20, 0, "456x/" );
        ft800_cmd_keys( ctx,  5, 220, 400, 43, 20, 0, "7890=" );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 'C' ) );
        ft800_cmd( ctx, FT800_COLOR_RGB(47,51,62) ); 
        ft800_cmd_button( ctx, 407, 128, 68, 43, 20, 0, "C" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 'C' + 'E' ) );
        ft800_cmd_button( ctx, 407, 174, 68, 43, 20, 0, "CE" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( '.' ) );
        ft800_cmd_button( ctx, 407, 220, 68, 43, 20, 0, "." );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );

        if( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) == 5 )
        {
            flag2 = false;
        }
        if ( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) != 0 && ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) != prev_tag ) 
        {
            calculation( ctx, cfg, cmdOffset, ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) );
        }

        ft800_cmd( ctx, FT800_COLOR_RGB( 255, 255, 255 ) );
        ft800_cmd_text( ctx, 30, 45, 30, 0, display_buffer );
        prev_tag = ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG );
        ft800_end_display_list( ctx ); 
    }
}

void stopwatch( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, bool flag2 )
{
    uint8_t h=0, m=0, s=0, h_mem=0, s_mem=0, m_mem=0;

    while( flag2 )
    {
        ft800_start_display_list( ctx );
        ft800_cmd( ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( ctx, FT800_SCISSOR_SIZE( 480, 272 ) );
        ft800_cmd_gradient( ctx, 0, 0, 480, 272, 50, 50, 50, 150, 150, 150 );
        ft800_cmd( ctx, FT800_COLOR_A( alpha ) );
        ft800_cmd_clock( ctx, 90, 121, 70, 16384, hr, 0, h, 0 );
        ft800_cmd_clock( ctx, 240, 121, 70, 16384, hr, 0, m, 0 );
        ft800_cmd_clock( ctx, 390, 121, 70, 16384, hr, 0, s, 0 );
        
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 5 ) );
        ft800_cmd_button( ctx, 10, 10, 50, 20, 20, 0, "ESC" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 9 ) );
        ft800_cmd_button( ctx, 40, 205, 100, 50, 30, 0, "Start" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 10 ) );
        ft800_cmd_button( ctx, 190, 205, 100, 50, 30, 0, "Pause" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 11 ) );
        ft800_cmd_button( ctx, 340, 205, 100, 50, 30, 0, "Reset" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_COLOR_RGB( 255, 255, 255 ) );
        ft800_cmd_text( ctx, 75, 130, 26, 0, "Hour" );
        ft800_cmd_text( ctx, 215, 132, 26, 0, "Minutes" );
        ft800_cmd_text( ctx, 365, 132, 26, 0, "Seconds" );

        if ( ft800_read_8_bits( ctx,  FT800_REG_TOUCH_TAG ) == 5 )
        {
            flag2=false;
            flag3=false;
            hr = 0, sec = 0, min = 0;
        }
        if ( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) == 9 )
        {
            if( flag3 == false )
            {
                flag3 = true;
                hr = 0, min = 0, sec = 0;
            }
            flag4 = true;
            hr = h_mem;
            min = m_mem;
            sec = s_mem;
        }

        if ( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) == 10 )
        {
            flag4 = false;
            s_mem = s;
            m_mem = m;
            h_mem = h;
        }
    
        if ( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) == 11 )
        {
            h = 0;
            m = 0;
            s = 0;
            hr = 0;
            min = 0;
            sec = 0;
        }
        
        if ( flag3 && flag4 )
        {
            h = hr;
            m = min;
            s = sec;
        }

        ft800_end_display_list(ctx);            
    }
}

void settings( ft800_t *ctx, ft800_cfg_t *cfg, uint16_t *cmdOffset, bool flag2, uint32_t tracker, uint16_t angle1, uint16_t angle2, uint16_t g_val1, uint16_t g_val2 )
{
    while ( flag2 )
    {
        ft800_start_display_list( ctx );
        ft800_cmd( ctx, FT800_SCISSOR_XY( 0, 0 ) );
        ft800_cmd( ctx, FT800_SCISSOR_SIZE( 480, 272 ) );
        ft800_cmd_gradient( ctx, 0, 0, 480, 272, 255, 47, 0, 39, 37, 30 );
        ft800_cmd( ctx, FT800_COLOR_A( alpha ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 5 ) );
        
        ft800_cmd_button( ctx, 10, 10, 50, 20, 20, 0, "ESC" );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        tracker=ft800_read_8_bits( ctx, FT800_REG_TRACKER );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 6 ) );
        ft800_cmd_dial( ctx, 90, 180, 50, 0, angle1 );
        ft800_cmd_track( ctx, 90, 180, 1, 1, 6 );
        ft800_cmd( ctx, FT800_TAG( 6 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( 7 ) );
        ft800_cmd_dial( ctx, 390, 180, 50, 0, angle2 );
        ft800_cmd_track( ctx, 390, 180, 1, 1, 7 );
        ft800_cmd( ctx, FT800_TAG( 7 ) );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        ft800_cmd_gauge( ctx, 170, 100, 60, 0, 5, 4, g_val1, 100 );
        ft800_cmd_gauge( ctx, 310, 100, 60, 0, 5, 4, g_val2, 100 );
                
        if ( ( tracker & 0xFF ) == 6 && angle1 >= 100 && angle1 <= 65500 )
        {
            angle1 = tracker >> 16;
            if ( angle1 > 65500 )
            {
                angle1 = 65500;
            }
            if ( angle1 < 100)
            {
                angle1 = 100;
            }
        }
        if ( ( tracker & 0xFF ) == 7 )
        {
            angle2 = tracker >> 16;
        }
        uint8_t bright = ( uint8_t )( 128.0 * ( ( float )angle1 / 65535.0 ) );
        if ( bright < 12 )
        {
            bright = 12;
        }
        ft800_write_8_bits( ctx, 0x1024C4, bright);
        g_val1 = ( uint16_t )( 100.0 * ( ( float )bright / 128.0 ) );
        alpha = ( uint8_t )( 255.0* ( ( float )angle2 / 65535.0 ) );
        if ( alpha < 26 )
        {
            alpha = 26;
        }
        g_val2 = ( uint8_t )( 100.0 * ( ( float )alpha / 255.0 ) );
        if ( ft800_read_8_bits( ctx, FT800_REG_TOUCH_TAG ) == 5 )
        {
            flag2 = false;
        }
        ft800_end_display_list( ctx );
                
    }
}

int main(void)
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif



    static ft800_cfg_t config;
    static ft800_t ctx;
    static tp_drv_t drv;
    static digital_out_t cs_pin;
    static digital_out_t pin;

    config.cs_pin = PA4;
    config.sck_pin = PA5;
    config.miso_pin = PA6;
    config.mosi_pin = PA7;
    config.pd_pin = PC15;
    

    digital_out_init(&pin, PC8);
    ft800_default_cfg( &config );
    ft800_init( &ctx, &config, &drv );

    uint8_t s_red = 0;
    uint8_t s_green = 0; 
    uint8_t s_blue = 0;
    uint8_t e_red = 0; 
    uint8_t e_green = 0; 
    uint8_t e_blue = 0; 
    uint16_t cmdOffset = 0;
    uint16_t angle1 = 0x8000;
    uint16_t angle2 = 0x8000;
    
    uint16_t g_val1 = 50;
    uint16_t g_val2 = 0;
    uint16_t x1 = 180;
    uint16_t x2 = 720;
    uint16_t x3 = 1150;
    uint16_t y1 = 60;
    uint16_t y2 = 120;
    uint16_t y3 = 105;

    uint32_t tracker;
    uint16_t val = 0x0001;
    



    while (1)
    {
        tracker = ft800_read_8_bits( &ctx, FT800_REG_TRACKER );
        if ( ( tracker & 0xFF ) == 8 )
        {
            val= tracker >> 16;

        }

        if ( ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG) ) == 2 )
        {
            flag2 = true;
            calculator( &ctx, &config, &cmdOffset, flag2 );
        }
        if ( (ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG) ) == 3 )
        {
            flag2 = true;
            stopwatch( &ctx, &config, &cmdOffset, flag2 );
        }
        if ( ( ft800_read_8_bits( &ctx, FT800_REG_TOUCH_TAG ) ) == 4 )
        {
            flag2 = true;
            settings( &ctx, &config, &cmdOffset, flag2, tracker, angle1, angle2, g_val1, g_val2 );
        }
        
        ft800_start_display_list( &ctx );

  
        gradient_background( &ctx, &config, &cmdOffset, s_red, s_green, s_blue, e_red, e_green, e_blue, val );
        ft800_cmd( &ctx, FT800_COLOR_A( alpha ) );  
        //val=read_gesture(&ctx1,8);
        tracker =  ft800_read_32_bits( &ctx, FT800_REG_TRACKER);
        if ( ( tracker & 0xFF ) == 8 )
        {
            val= tracker >> 16;

        }
     
        ft800_cmd( &ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( &ctx, FT800_TAG( 8 ) );
        ft800_cmd( &ctx, FT800_COLOR_RGB( 255, 0, 0 ) );
        ft800_cmd_slider( &ctx, 70, 27, 340, 12, 0, val, 65535 );
        ft800_cmd_track( &ctx, 70, 27, 340, 12, 8 );
        ft800_cmd( &ctx, FT800_TAG( 1 ) );
        ft800_cmd( &ctx, FT800_TAG_MASK( 0 ) );
        
        if ( x1 == 180)
        {
            y1 = ( uint16_t )( 70 + ( 660 / 65535.0 ) * val );
        }

        if( x2 >= 240 || y2 == 120 )
        {
            x2 = ( uint16_t )( 820 - ( 480 / 32767.5 ) * val );
            y2 = 120;
        }

        if ( x2 <= 240 )
        {
            y2 = ( uint16_t )( 80 + ( 329 / 32767.5 ) * ( val - 32767.5 ) );
            x2 = 240;
        }
  
        if ( x3 >= 240 )
        {
            x3 = ( uint16_t )( 1150 - ( 910 / 65535.0 ) * val );
        }
    
        calc_ic( &ctx, &config, &cmdOffset, x1, y1 );
        memset( input_buffer, 0, sizeof( input_buffer ) );
        memset( display_buffer, 0, sizeof( display_buffer ) );
        if( y2 <= 272 && x2 >= 240 && x3 >= 480 ){
            stopwatch_ic( &ctx, &config, &cmdOffset, x2, y2 );
        }

        
        if ( val >= 20000 )
        {
            settings_ic( &ctx, &config, &cmdOffset, x3, y3 );
        }
        
        ft800_end_display_list( &ctx ); 

    }

    return 0;
}
