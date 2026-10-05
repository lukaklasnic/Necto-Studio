/**
 * @file main.c
 * @brief Main source file for the FT800_Demo application.
 *
 * This is a basic project template providing a minimal structure
 * for initializing the MCU and writing embedded application logic.
 */

#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif
#include "ft800_demo.h"
#include "MikroSDK.Board"
#include "MikroSDK.Driver"

int main(void)
{
    /* Do not remove this line — it ensures correct MCU initialization. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    extern ft800_cfg_t cfg;
    extern ft800_t ctx;
    extern uint16_t cmdOffset;

    gradient_colors_t g_colors;
    icon_position_t position;
    slider_data_t slider;
    flag_t flag;
    calc_operators_t operators;
    stopwatch_t stopwatch;
    settings_t settings;

    display_initialization();
    set_init_values(&g_colors, &position, &slider, &flag, &operators, &stopwatch, &settings );

    while (1)
    {
        ft800_start_display_list( &ctx );

        //ft800_cmd( &ctx, FT800_COLOR_A( g_colors.alpha ) );  
        gradient_background( &g_colors, slider.val ) ;
        main_menu_slider(&slider);
        calculate_icon_position( &position, &slider );
        submenu_select( &flag, &operators, &g_colors );
    
        ft800_end_display_list( &ctx ); 
    }

    return 0;
}
