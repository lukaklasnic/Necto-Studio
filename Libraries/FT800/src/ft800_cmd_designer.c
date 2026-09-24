/****************************************************************************
**
** Copyright (C) 2025 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** This file is part of the mikroSDK package
**
** Commercial License Usage
**
** Licensees holding valid commercial NECTO compilers AI licenses may use this
** file in accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The MikroElektronika Company.
** For licensing terms and conditions see
** https://www.mikroe.com/legal/software-license-agreement.
** For further information use the contact form at
** https://www.mikroe.com/contact.
**
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used for
** non-commercial projects under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
** OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
/*!
 * @file  ft800.c
 * @brief FT800 controller source file.
 */

#include "ft800.h"
#include "ft800_cmd.h"
#include "ft800_reg.h"
#include "ft800_const.h"
#include "ft800_struct.h"
#include "ft800_cmd_str.h"

extern uint16_t cmdOffset;
static uint32_t ft800_ramg_ptr = 0;
static uint8_t  ft800_next_handle = 0;

void ft800_initialization( ft800_t *ctx,ft800_cfg_t *cfg, tp_drv_t *drv )
{
    cfg->cs_pin   = PA4;
    cfg->sck_pin  = PA5;
    cfg->miso_pin = PA6;
    cfg->mosi_pin = PA7;
    cfg->pd_pin   = PC15;
    ft800_default_cfg( cfg );
    ft800_init( ctx, cfg, drv );

    if(ft800_read_8_bits( ctx, FT800_REG_ID ) != 0x7C )
    {
            cfg->cs_pin   = PE8;
            cfg->sck_pin  = PA5;
            cfg->miso_pin = PA6;
            cfg->mosi_pin = PA7;
            cfg->pd_pin   = PE7;
            ft800_default_cfg( cfg );
            ft800_init( ctx, cfg, drv );
    }

    if(ft800_read_8_bits( ctx, FT800_REG_ID ) != 0x7C )
    {
        
            cfg->cs_pin   = PD13;
            cfg->sck_pin  = PA5;
            cfg->miso_pin = PA6;
            cfg->mosi_pin = PA7;
            cfg->pd_pin   = PD11;
            ft800_default_cfg( cfg );
            ft800_init( ctx, cfg, drv );
    }

    if( ft800_read_8_bits( ctx, FT800_REG_ID ) != 0x7C )
    {
            cfg->cs_pin   = PD15;
            cfg->sck_pin  = PC10;
            cfg->miso_pin = PC11;
            cfg->mosi_pin = PC12;
            cfg->pd_pin   = PD14;
            ft800_default_cfg( cfg );
            ft800_init( ctx, cfg, drv );
    }

    if(ft800_read_8_bits( ctx, FT800_REG_ID ) != 0x7C )
    {
            cfg->cs_pin   = PB3;
            cfg->sck_pin  = PE12;
            cfg->miso_pin = PE13;
            cfg->mosi_pin = PE14;
            cfg->pd_pin   = PE4;
            ft800_default_cfg( cfg );
            ft800_init( ctx, cfg, drv );
    }

    ft800_logo_animation( ctx );
}

uint8_t ft800_read_press( ft800_t *ctx, uint8_t tag )
{
    uint8_t last_state;
    uint8_t current_state;

    last_state = ft800_read_8_bits( ctx,  FT800_REG_TOUCH_TAG );
    Delay_ms( 50 );
    current_state = ft800_read_8_bits( ctx,  FT800_REG_TOUCH_TAG );

    if ( last_state == current_state ) {
        return current_state;  
    }

    return last_state;
}

uint8_t ft800_rgb_24_convert( uint32_t color, char *name ) 
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    if ( !strcmp( name, "red" ))
    {
        red = ( uint8_t )( ( ( color >> 16 ) & 0xFF ) );
        return red;
    }
    if ( !strcmp( name, "green" ) )
    {
        green = ( uint8_t )( ( ( color >> 8  ) & 0xFF ) );
        return green;
    }
    if( !strcmp( name, "blue" ) )
    {
        blue = ( uint8_t )( color & 0xFF ) ;
        return blue;
    }
}

uint16_t ft800_rgb24_to_rgb565( uint32_t rgb )
{
    uint8_t r = ( rgb >> 16 ) & 0xFF;
    uint8_t g = ( rgb >> 8 )  & 0xFF;
    uint8_t b =  rgb & 0xFF;

    return ( uint16_t )( ( ( r & 0xF8 ) << 8 ) |  ( ( g & 0xFC ) << 3 ) |  ( b >> 3 ) );
}

void ft800_gradcolor( ft800_t *ctx, uint32_t c )
{
    ft800_cmd( ctx, FT800_CMD_GRADCOLOR );
    ft800_cmd( ctx, c );
}

void ft800_fgcolor( ft800_t *ctx, uint32_t c )
{
    ft800_cmd( ctx, FT800_CMD_FGCOLOR );
    ft800_cmd( ctx, c );
}

void ft800_bgcolor( ft800_t *ctx, uint32_t c )
{
    ft800_cmd( ctx, FT800_CMD_BGCOLOR );
    ft800_cmd( ctx, c );
}

void ft800_logo_animation( ft800_t *ctx )
{
    ft800_start_display_list( ctx );
    ft800_cmd( ctx, FT800_CMD_LOGO );
    ft800_wait_coprocessor( ctx );
    ft800_end_display_list( ctx );
}

void ft800_line_edges( ft800_t *ctx, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t line_width, uint32_t color )
{
        ft800_cmd( ctx, FT800_BEGIN( FT800_LINES ) );
        ft800_cmd( ctx, FT800_LINE_WIDTH( line_width * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( color, "red" ), ft800_rgb_24_convert( color, "green" ), ft800_rgb_24_convert( color, "blue" ) ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x1 * FT800_POINT_SIZE_SCALE, y1 * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x2 * FT800_POINT_SIZE_SCALE, y2 * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_END() );
}

void ft800_designer_rectangle_edges( ft800_t *ctx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t line_width, uint32_t color )
{
        ft800_cmd( ctx, FT800_BEGIN( FT800_LINES ) );
        ft800_cmd( ctx, FT800_LINE_WIDTH( line_width * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( color, "red" ), ft800_rgb_24_convert( color, "green" ), ft800_rgb_24_convert( color, "blue" ) ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x * FT800_POINT_SIZE_SCALE, y * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( ( x + width ) * FT800_POINT_SIZE_SCALE, y * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( ( x + width ) * FT800_POINT_SIZE_SCALE, y * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( ( x + width ) * FT800_POINT_SIZE_SCALE, ( y + height ) * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( ( x + width ) * FT800_POINT_SIZE_SCALE, ( y + height ) * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x * FT800_POINT_SIZE_SCALE, ( y + height ) * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x * FT800_POINT_SIZE_SCALE, ( y + height ) * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_VERTEX2F( x * FT800_POINT_SIZE_SCALE, y * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_END() );    
}

void ft800_designer_circle_edges( ft800_t *ctx, uint16_t cx, uint16_t cy, uint16_t diameter, uint8_t line_width, uint32_t color )
{
        float a = diameter / FT800_CIRCLE_EDGES_WIDTH_SCALE;

        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( color, "red" ), ft800_rgb_24_convert( color, "green" ), ft800_rgb_24_convert( color, "blue" ) ) );
        ft800_cmd( ctx, FT800_LINE_WIDTH( line_width * FT800_POINT_SIZE_SCALE ) );
        ft800_cmd( ctx, FT800_BEGIN( FT800_LINE_STRIP ) );

        uint16_t segments = FT800_CIRCLE_EDGES_SEGMENTS;

        for ( uint16_t i = 0; i <= segments; i++ )
        {
            float theta = ( FT800_CIRCLE_EDGES_THETA_SCALE * i ) / segments;
            float x = cx + a * cos( theta );
            float y = cy + a * sin( theta );

            ft800_cmd( ctx, FT800_VERTEX2F( ( uint16_t )( x * FT800_POINT_SIZE_SCALE ), ( uint16_t )( y * FT800_POINT_SIZE_SCALE ) ) );
        }

        ft800_cmd( ctx, FT800_END() );
}

void ft800_designer_circle_fill( ft800_t *ctx, uint16_t cx, uint16_t cy, uint16_t diameter, uint32_t color )
{
    ft800_cmd( ctx, FT800_BEGIN( FT800_POINTS ) );
    ft800_cmd( ctx, FT800_POINT_SIZE( ( (uint16_t)diameter/2) * FT800_POINT_SIZE_SCALE ) );
    ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( color, "red" ), ft800_rgb_24_convert( color, "green" ), ft800_rgb_24_convert( color, "blue" ) ) );
    ft800_cmd( ctx, FT800_VERTEX2F( cx * FT800_POINT_SIZE_SCALE, cy * FT800_POINT_SIZE_SCALE ) );
    ft800_cmd( ctx, FT800_END() );
}

void ft800_image_load( ft800_t *ctx, ft800_designer_image *image )
{
    if ( !ctx || !image || !image->image_data )
        return;

    if ( image->loaded )
        return;

    uint32_t image_size_bytes = image->width * image->height * FT800_IMAGE_SIZE_CONST_1 + FT800_IMAGE_SIZE_CONST_2;

    ft800_ramg_ptr = ( ft800_ramg_ptr + 3 ) & ~3;
    image->ramg_address = ft800_ramg_ptr;
    image->bitmap_handle = ft800_next_handle++;

    ft800_write_ram_g( ctx, image->ramg_address, image->image_data, image_size_bytes );

    ft800_ramg_ptr += image_size_bytes;
    image->loaded = 1;
}

void ft800_image_draw( ft800_t *ctx, ft800_designer_image *image )
{
    if ( !ctx || !image || !image->loaded )
        return;

    ft800_cmd( ctx, FT800_BITMAP_HANDLE( image->bitmap_handle ));
    ft800_cmd( ctx, FT800_BITMAP_SOURCE( image->ramg_address ));
    ft800_cmd( ctx, FT800_BITMAP_LAYOUT( FT800_IMAGE_BITMAP_LAYOUT_FORMAT, image->width * FT800_IMAGE_SIZE_CONST_1, image->height ) );

    ft800_cmd( ctx, FT800_BITMAP_SIZE( 0, 0, 0, image->width, image->height ) );

    ft800_cmd( ctx, FT800_BEGIN( FT800_BITMAPS ));
    ft800_cmd( ctx, FT800_VERTEX2F( image->x_coord * FT800_POINT_SIZE_SCALE, image->y_coord  * FT800_POINT_SIZE_SCALE ) );
    ft800_cmd( ctx, FT800_END() );
}

void ft800_designer_screen_settings( ft800_t *ctx, ft800_designer_screen *screen )
{
    ft800_cmd( ctx, FT800_CLEAR_COLOR_RGB( ft800_rgb_24_convert( screen->background_color, "red" ),ft800_rgb_24_convert( screen->background_color, "green" ), ft800_rgb_24_convert( screen->background_color, "blue" ) ) );
    ft800_cmd( ctx, FT800_FULL_CLEAR );
    if( screen->grid_enable )
    {
        if( screen->type == FT800_GRID_LINE )
        {
            for( uint16_t i = 0; i <= 272; i += screen->grid_size )
            {
                ft800_cmd_line( ctx, 0, i, 480, i, ft800_rgb24_to_rgb565( screen->grid_color ), 1 );
            }
            for( uint16_t i = 0; i <= 480; i += screen->grid_size )
            {
                ft800_cmd_line( ctx, i, 0, i, 272, ft800_rgb24_to_rgb565( screen->grid_color ), 1);
            }
        }
        else if( screen->type == FT800_GRID_DOTS )
        {
            for( uint16_t j = 0; j <= 272; j += screen->grid_size )
            {
                for( uint16_t i = 0; i <= 480; i += screen->grid_size )
                {
                    ft800_cmd_points( ctx, i, j, 1, screen->grid_color );
                }
            }
        }
    }
}

void ft800_line_designer( ft800_t *ctx, ft800_designer_line *line )
{
    if( line->visible )
    {
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( line->tag ) );
        ft800_line_edges( ctx, line->x1_coord, line->y1_coord, line->x2_coord, line->y2_coord, line->pen.width, line->pen.color );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_rectangle_designer( ft800_t *ctx, ft800_designer_rectangle *rectangle )
{
    if( rectangle->visible )
    {
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( rectangle->tag ) );
        if( rectangle->gradient.filled )
        {
            switch ( rectangle->gradient.gradient_type )
            {
                case FT800_LEFT_RIGHT_GRADIENT:
                    rectangle->gradient.x0_gradient = rectangle->x_coord;
                    rectangle->gradient.y0_gradient = rectangle->y_coord;
                    rectangle->gradient.x1_gradient = rectangle->x_coord;
                    rectangle->gradient.y1_gradient = rectangle->y_coord + rectangle->width;
                    break;
            
                case FT800_RIGHT_LEFT_GRADIENT:
                    rectangle->gradient.x0_gradient = rectangle->x_coord;
                    rectangle->gradient.y0_gradient = rectangle->y_coord + rectangle->width;
                    rectangle->gradient.x1_gradient = rectangle->x_coord;
                    rectangle->gradient.y1_gradient = rectangle->y_coord;
                    break;
            
                case FT800_TOP_BOTTOM_GRADIENT:
                    rectangle->gradient.x0_gradient = rectangle->x_coord;
                    rectangle->gradient.y0_gradient = rectangle->y_coord;
                    rectangle->gradient.x1_gradient = rectangle->x_coord + rectangle->width;
                    rectangle->gradient.y1_gradient = rectangle->y_coord;
                    break;
            
                case FT800_BOTTOM_TOP_GRADIENT:
                    rectangle->gradient.x0_gradient = rectangle->x_coord + rectangle->width;
                    rectangle->gradient.y0_gradient = rectangle->y_coord;
                    rectangle->gradient.x1_gradient = rectangle->x_coord;
                    rectangle->gradient.y1_gradient = rectangle->y_coord;
                    break;
            
                default:
                    return;
            }
            ft800_cmd( ctx, FT800_SCISSOR_XY( rectangle->x_coord, rectangle->y_coord ) );
            ft800_cmd( ctx, FT800_SCISSOR_SIZE( rectangle->width, rectangle->height ) );
            ft800_cmd_gradient( ctx, rectangle->gradient.x0_gradient + rectangle->pen.width, rectangle->gradient.y0_gradient + rectangle->pen.width, rectangle->gradient.x1_gradient - 2 * rectangle->pen.width, rectangle->gradient.y1_gradient - 2 * rectangle->pen.width, ft800_rgb_24_convert( rectangle->gradient.start_color, "red" ), ft800_rgb_24_convert( rectangle->gradient.start_color, "green" ), ft800_rgb_24_convert( rectangle->gradient.start_color, "blue" ), ft800_rgb_24_convert( rectangle->gradient.end_color, "red" ), ft800_rgb_24_convert( rectangle->gradient.end_color, "green" ), ft800_rgb_24_convert( rectangle->gradient.end_color, "blue" ) );
        }
        ft800_designer_rectangle_edges( ctx, rectangle->x_coord, rectangle->y_coord, rectangle->width, rectangle->height, rectangle->pen.width, rectangle->pen.color );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_circle_designer( ft800_t *ctx, ft800_designer_circle *circle )
{
    if( circle->visible ){
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( circle->tag ) );
        if( circle->gradient.filled )
        {
            ft800_designer_circle_fill( ctx, circle->cx_coord, circle->cy_coord, circle->diameter, circle->gradient.start_color );
        }
        ft800_designer_circle_edges( ctx, circle->cx_coord, circle->cy_coord, circle->diameter, circle->pen.width, circle->pen.color );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_ellipse_designer(ft800_t *ctx,ft800_designer_ellipse *ellipse)
{
    if( ellipse-> visible )
    {
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( ellipse->tag ) );
        if( ellipse->gradient.filled )
        {
            ft800_draw_gradient_ellipse( ctx, ellipse->cx_coord , ellipse->cy_coord ,ellipse->width, ellipse->height, ft800_rgb24_to_rgb565( ellipse->gradient.start_color ), ft800_rgb24_to_rgb565( ellipse->gradient.end_color ), ellipse->gradient.gradient_type );
        }
        ft800_draw_edges_ellipse( ctx, ellipse->cx_coord , ellipse->cy_coord ,ellipse->width, ellipse->height, ft800_rgb24_to_rgb565( ellipse->pen.color ), ellipse->pen.width );
        ft800_cmd( ctx, FT800_END() );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_button_designer( ft800_t *ctx, ft800_designer_button *button )
{
    if( button->visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( button->text.text_color , "red" ), ft800_rgb_24_convert( button->text.text_color, "green" ), ft800_rgb_24_convert( button->text.text_color, "blue" ) ) ); 
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( button->tag ) );
        if(ft800_read_press( ctx, button-> tag ) == button->tag )
        {
            button->_3d = FT800_FLAT;
        }
        else{
            button->_3d = FT800_3D;
        }
        ft800_fgcolor( ctx,  button->gradient.end_color);
        ft800_gradcolor( ctx, button->gradient.start_color );
        ft800_cmd_button( ctx, button->x_coord, button->y_coord, button->width,button->height, button->text.text_size, button->_3d, button->text.text_caption );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}


void ft800_keys_designer(ft800_t *ctx,ft800_designer_keys *keys )
{
    if( keys->visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( keys->text.text_color, "red" ), ft800_rgb_24_convert( keys->text.text_color, "green" ), ft800_rgb_24_convert( keys->text.text_color, "blue" ) ) );
        ft800_fgcolor( ctx, keys->gradient.end_color );
        ft800_gradcolor( ctx, keys->gradient.start_color );
        
        if( keys->type == FT800_KEYBOARD_NUM )
        {
            ft800_cmd_keys( ctx, keys->x_coord, keys->y_coord, keys->width, (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "789" );
            ft800_cmd_keys( ctx, keys->x_coord, keys->y_coord + (uint16_t)( keys->height / 4 ) + 2, keys->width, (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "456" );
            ft800_cmd_keys( ctx, keys->x_coord, keys->y_coord + 2 * (uint16_t)( keys->height / 4 ) + 4, keys->width, (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "123" );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
            ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
            ft800_cmd( ctx, FT800_TAG( 48 ) );
            ft800_cmd_button( ctx, keys->x_coord, (uint16_t)(keys->y_coord+3 * (uint16_t)( keys->height / 4 ) + 6 ),(uint16_t)(keys->width*2/3),(uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "0" );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
            ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
            ft800_cmd( ctx, FT800_TAG( 252 ) );
            ft800_cmd_button( ctx, (uint16_t)(keys->x_coord+keys->width*2/3+2), (uint16_t)(keys->y_coord+3*(uint16_t)( keys->height / 4 )+6),(uint16_t)(keys->width/3-1),(uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "." );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        }
        else if ( keys->type == FT800_KEYBOARD_QUERTZ )
        {
            ft800_cmd_keys( ctx, keys->x_coord, keys->y_coord, keys->width, (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "QWERTZUIOP" );
            ft800_cmd_keys( ctx, (uint16_t)(keys->x_coord+keys->width/20), keys->y_coord+(uint16_t)( keys->height / 4 )+2, (uint16_t)(keys->width*9/10), (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "ASDFGHJKL" );
            ft800_cmd_keys( ctx, (uint16_t)(keys->x_coord+keys->width/10), keys->y_coord+2*(uint16_t)( keys->height / 4 )+4, (uint16_t)(keys->width*8/10), (uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, "YXCVBNM" );
            ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
            ft800_cmd( ctx, FT800_TAG( 255 ) );
            ft800_cmd_button( ctx, keys->x_coord, (uint16_t)(keys->y_coord+3*(uint16_t)( keys->height / 4 )+6),keys->width,(uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, " SPACE" );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
            ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
            ft800_cmd( ctx, FT800_TAG( 253 ) );
            ft800_cmd_button( ctx, keys->x_coord, keys->y_coord+2*(uint16_t)( keys->height / 4 )+4,(uint16_t)(keys->width/11),(uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, " Ent" );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
            ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
            ft800_cmd( ctx, FT800_TAG( 254 ) );
            ft800_cmd_button( ctx, (uint16_t)(keys->x_coord+keys->width-keys->width*4/35+8), keys->y_coord+2*(uint16_t)( keys->height / 4 )+4,(uint16_t)(keys->width/11),(uint16_t)( keys->height / 4 ), keys->text.text_size, keys->_3d, " Del" );
            ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        }
    }
}

void ft800_clock_designer( ft800_t *ctx, ft800_designer_clock *clock )
{
    if ( clock->visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( clock->face_color , "red" ), ft800_rgb_24_convert( clock->face_color, "green" ), ft800_rgb_24_convert( clock->face_color, "blue" ) ) ); 
        ft800_bgcolor( ctx, clock->background_color );
        ft800_cmd_clock( ctx, clock->cx_coord, clock->cy_coord, ( uint16_t )( clock->diameter / 2 ), clock->_3d, clock->hours, clock->minutes, clock->seconds, 0 );
    }
}

void ft800_gauge_designer( ft800_t *ctx, ft800_designer_gauge *gauge )
{
    if( gauge->visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( gauge->face_color , "red" ), ft800_rgb_24_convert( gauge->face_color, "green" ), ft800_rgb_24_convert( gauge->face_color, "blue" ) ) ); 
        ft800_bgcolor( ctx, gauge->background_color );
        ft800_cmd_gauge( ctx, gauge->cx_coord, gauge->cy_coord, ( uint16_t )( gauge->diameter / 2 ), gauge->_3d, gauge-> major, gauge-> minor, gauge-> val, gauge-> range );
    }
}

void ft800_dial_designer( ft800_t *ctx, ft800_designer_dial *dial )
{
    if ( dial->visible )
    {
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( dial->tag ) );
        ft800_cmd_track( ctx, dial->cx_coord, dial->cy_coord, 1, 1, dial->tag );
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( dial->pointer_color , "red" ), ft800_rgb_24_convert( dial->pointer_color, "green" ), ft800_rgb_24_convert( dial->pointer_color, "blue" ) ) ); 
        ft800_fgcolor( ctx, dial->background_color );
        ft800_cmd_dial( ctx, dial->cx_coord, dial->cy_coord, ( uint16_t ) ( dial->diameter / 2 ), dial->_3d, dial->val );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
        if ( ( ft800_read_32_bits( ctx, FT800_REG_TRACKER ) & 0xFF ) == dial->tag )
        {
            dial->val =  ft800_read_32_bits( ctx, FT800_REG_TRACKER ) >> 16;

        }
    }
}

void ft800_toggle_designer( ft800_t *ctx, ft800_designer_toggle *toggle )
{
    if( toggle-> visible )
    {
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( toggle->tag ) );
        if((ft800_read_press( ctx, toggle->tag ) == toggle->tag ) && ( toggle->state == FT800_OFF ) )
        {
            toggle->state = FT800_ON;
        }
        else if((ft800_read_press( ctx, toggle->tag ) == toggle->tag ) && ( toggle->state == FT800_ON ) )
        {
            toggle->state = FT800_OFF;
        }
        ft800_bgcolor( ctx, toggle->background_color );
        ft800_fgcolor( ctx, toggle->thumb_color );
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( toggle->text.text_color , "red" ), ft800_rgb_24_convert( toggle->text.text_color, "green" ), ft800_rgb_24_convert( toggle->text.text_color, "blue" ) ) ); 
        ft800_cmd_toggle( ctx, toggle->x_coord + 17, toggle->y_coord + 9, toggle->width - 30, FT800_TOGGLE_FONT_SIZE , toggle->_3d, toggle->state, FT800_TOGGLE_LABEL );
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_scroll_bar_designer( ft800_t *ctx, ft800_designer_scroll_bar *scroll_bar )
{
    if( scroll_bar -> visible )
    {
        if ( ( ft800_read_32_bits( ctx, FT800_REG_TRACKER ) & 0xFF ) == scroll_bar->tag )
        {
            scroll_bar->val =  ft800_read_32_bits( ctx, FT800_REG_TRACKER ) >> 16;

        }

        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( scroll_bar->tag ) );
        ft800_bgcolor( ctx, scroll_bar->background_color );
        ft800_fgcolor( ctx, scroll_bar->thumb_color );
        ft800_cmd( ctx, FT800_COLOR_RGB( 255, 0, 0 ) );
        if( scroll_bar -> width > scroll_bar -> height )
        {
            ft800_cmd_scrollbar( ctx, ( uint16_t )( scroll_bar -> x_coord + scroll_bar -> height / 2 ), scroll_bar -> y_coord, scroll_bar -> width - scroll_bar -> height , scroll_bar -> height, scroll_bar -> _3d, scroll_bar -> val, ( uint16_t )( scroll_bar-> size * FT800_SCROLL_BAR_MAX_RANGE / 100 ), FT800_SCROLL_BAR_MAX_RANGE );
            ft800_cmd_track( ctx,( uint16_t )( scroll_bar -> x_coord + scroll_bar-> height / 2 ), scroll_bar -> y_coord, scroll_bar-> width - scroll_bar -> height, scroll_bar -> height, scroll_bar -> tag );
        } 
        
        else
        {
            ft800_cmd_scrollbar( ctx, scroll_bar-> x_coord, ( uint16_t )( scroll_bar-> y_coord + scroll_bar-> width / 2 ), scroll_bar-> width, scroll_bar-> height - scroll_bar-> width , scroll_bar-> _3d, scroll_bar-> val, ( uint16_t )( scroll_bar-> size * FT800_SCROLL_BAR_MAX_RANGE / 100 ), FT800_SCROLL_BAR_MAX_RANGE );
            ft800_cmd_track( ctx, scroll_bar-> x_coord, ( uint16_t )( scroll_bar-> y_coord + scroll_bar-> width / 2 ), scroll_bar-> width, scroll_bar-> height - scroll_bar-> width, scroll_bar->tag );
        }
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_slider_designer( ft800_t *ctx, ft800_designer_slider *slider )
{
    if( slider-> visible )
    {
        if ( ( ft800_read_32_bits( ctx, FT800_REG_TRACKER ) & 0xFF ) == slider->tag )
        {
            slider->val =  ft800_read_32_bits( ctx, FT800_REG_TRACKER ) >> 16;

        }
        
        ft800_cmd( ctx, FT800_TAG_MASK( 1 ) );
        ft800_cmd( ctx, FT800_TAG( slider->tag ) );
        ft800_bgcolor( ctx, slider->background_color_right );
        ft800_fgcolor( ctx, slider->thumb_color );
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( slider->background_color_left, "red" ), ft800_rgb_24_convert( slider->background_color_left, "green" ), ft800_rgb_24_convert( slider->background_color_left, "blue" ) ) ); 
        if( slider -> width > slider -> height )
        {
            ft800_cmd_slider( ctx, ( uint16_t )( slider->x_coord + slider->height / 2 ), ( uint16_t )( slider->y_coord + slider->height / 4 ), slider->width -  slider->height,( uint16_t )( slider->height / 2 ), slider->_3d, slider->val, FT800_SLIDER_MAX_RANGE );
            ft800_cmd_track( ctx, ( uint16_t )( slider->x_coord + slider->height / 2 ), ( uint16_t )( slider->y_coord + slider->height / 4 ), slider->width - slider->height, ( uint16_t )( slider->height / 2 ), slider->tag );
        }

        else
        {
            ft800_cmd_slider( ctx, ( uint16_t )( slider->x_coord + slider->width / 4 ), ( uint16_t )( slider->y_coord + slider->width / 2 ), ( uint16_t )( slider->width / 4 ), ( uint16_t )( slider->height - slider->width / 2 ), slider->_3d, slider->val, FT800_SLIDER_MAX_RANGE );
            ft800_cmd_track( ctx, ( uint16_t )( slider->x_coord + slider->width / 4 ), ( uint16_t )( slider->y_coord + slider->width / 2 ), ( uint16_t )( slider->width / 4 ), ( uint16_t )( slider->height - slider->width / 2 ), slider->tag );
        }
        
        ft800_cmd( ctx, FT800_TAG_MASK( 0 ) );
    }
}

void ft800_progress_bar_designer( ft800_t *ctx, ft800_designer_progress_bar *progress_bar )
{
    if( progress_bar -> visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( progress_bar->progress_color , "red" ), ft800_rgb_24_convert( progress_bar->progress_color, "green" ), ft800_rgb_24_convert( progress_bar->progress_color, "blue" ) ) );
        ft800_bgcolor( ctx, progress_bar->background_color );

        if( progress_bar -> width > progress_bar -> height )
        {
        ft800_cmd_progress( ctx,( uint16_t )( progress_bar -> x_coord + progress_bar->height / 2 ), progress_bar->y_coord, progress_bar->width - progress_bar -> height, progress_bar->height, progress_bar->_3d, progress_bar->val, progress_bar->range );
        }

        else
        {
        ft800_cmd_progress( ctx, progress_bar->x_coord, ( uint16_t )( progress_bar -> y_coord + progress_bar->width / 2 ), progress_bar->width, progress_bar->height - progress_bar -> width, progress_bar->_3d, progress_bar->val, progress_bar->range );            
        }
        
    }
}

void ft800_image_designer( ft800_t *ctx,ft800_designer_image *image )
{
    if( image-> visible )
    {   ft800_cmd( ctx, FT800_COLOR_RGB( 255, 255, 255 ) );
        ft800_image_draw( ctx, image );
    }
    if( image->frame ){
        ft800_designer_rectangle_edges( ctx, image->x_coord, image->y_coord, image->width, image->height, image->pen.width, image->pen.color );
    }
}

void ft800_label_designer( ft800_t *ctx, ft800_designer_label *label )
{
    if( label-> visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( label->text.text_color, "red" ), ft800_rgb_24_convert( label->text.text_color, "green" ), ft800_rgb_24_convert( label->text.text_color, "blue" ) ) ); 
        ft800_cmd_text( ctx, label->x_coord, label->y_coord, label->text.text_size, label->alignment, label->text.text_caption );
    }
}

void ft800_numeric_designer( ft800_t *ctx,ft800_designer_numeric *num )
{
    if( num->visible )
    {
        ft800_cmd( ctx, FT800_COLOR_RGB( ft800_rgb_24_convert( num->num_color, "red" ), ft800_rgb_24_convert( num->num_color, "green" ), ft800_rgb_24_convert( num->num_color, "blue" ) ) ); 
        ft800_cmd_number( ctx, num->x_coord, num->y_coord, num->num_size,  num->alignment, num->num );
    }
}

// ------------------------------------------------------------------------- END
