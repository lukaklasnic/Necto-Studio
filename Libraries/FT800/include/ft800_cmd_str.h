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
 * @file  ft800_reg.h
 * @brief FT800 Display Controller Structures.
 */

/*!
 * @addtogroup middlewaregroup Middleware
 * @{
 */

/*!
 * @addtogroup ft800 FT800 Display Controller Driver
 * @{
 */

/*!
 * @addtogroup ft800_registers FT800 Display Controller Structures
 * @brief FT800 Display Controller Structures List
 * @{
 */

#ifndef FT800_CMD_STR_H
#define FT800_CMD_STR_H

#include <stdint.h>
#include "ft800_const.h"

typedef enum{
    FT800_GRID_LINE,
    FT800_GRID_DOTS
}ft800_designer_grid_type;

typedef enum{
    FT800_TOP_BOTTOM_GRADIENT,
    FT800_BOTTOM_TOP_GRADIENT,
    FT800_LEFT_RIGHT_GRADIENT,
    FT800_RIGHT_LEFT_GRADIENT
}ft800_designer_gradient_type;

typedef enum{
    FT800_KEYBOARD_NUM,
    FT800_KEYBOARD_QUERTZ
}ft800_designer_key_type;

typedef enum{
    FT800_3D = 0,
    FT800_FLAT = 256
}ft800_designer_3d;

typedef enum{
    FT800_ON = 65535,
    FT800_OFF = 0
}ft800_designer_state;

typedef enum{
    FT800_ALIGN_LEFT = 0,
    FT800_ALIGN_HORISONTALY = 512,
    FT800_ALIGN_VERTICALY = 1024,
    FT800_ALIGN_CENTER = 1536,
    FT800_ALIGN_RIGHT = 2048
}ft800_alignment;

typedef struct
{
    char *text_caption;              
    uint32_t text_color;           
    uint8_t text_size;                  
} ft800_designer_text;

typedef struct 
{
    uint16_t width;
    uint32_t color;
}ft800_designer_pen;

typedef struct 
{
    bool filled;
    uint32_t start_color;
    uint32_t end_color;
    ft800_designer_gradient_type gradient_type;
    uint16_t x0_gradient;
    uint16_t y0_gradient;
    uint16_t x1_gradient;
    uint16_t y1_gradient;
}ft800_designer_gradient;

typedef struct
{
    bool visible;
    uint32_t background_color;
    bool grid_enable;
    ft800_designer_grid_type type;
    uint32_t grid_color;
    uint8_t grid_size;
}ft800_designer_screen;

typedef struct 
{
    bool visible;
    uint16_t x1_coord;
    uint16_t y1_coord;
    uint16_t x2_coord;
    uint16_t y2_coord;
    ft800_designer_pen pen;
    uint8_t tag;
}ft800_designer_line;

typedef struct 
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    ft800_designer_pen pen;
    ft800_designer_gradient gradient;
    uint8_t tag;
}ft800_designer_rectangle;

typedef struct 
{
    bool visible;
    uint16_t cx_coord;
    uint16_t cy_coord;
    uint16_t diameter;
    ft800_designer_pen pen;
    ft800_designer_gradient gradient;
    uint8_t tag;
}ft800_designer_circle;

typedef struct
{
    bool visible;
    uint16_t cx_coord;
    uint16_t cy_coord;
    uint16_t width;
    uint16_t height;
    ft800_designer_pen pen;
    ft800_designer_gradient gradient;
    uint8_t tag;
}ft800_designer_ellipse;

typedef struct 
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    ft800_designer_gradient gradient;
    ft800_designer_3d _3d;
    ft800_designer_text text;
    uint8_t tag
}ft800_designer_button;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    ft800_designer_gradient gradient;
    ft800_designer_3d _3d;
    ft800_designer_text text;
    ft800_designer_key_type type;
}ft800_designer_keys;

typedef struct 
{
    bool visible;
    uint16_t cx_coord;
    uint16_t cy_coord;
    uint16_t diameter;
    uint32_t background_color;
    uint32_t face_color;
    ft800_designer_3d _3d;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
}ft800_designer_clock;


typedef struct 
{
    bool visible;
    uint16_t cx_coord;
    uint16_t cy_coord;
    uint16_t diameter;
    uint32_t background_color;
    uint32_t face_color;
    ft800_designer_3d _3d;
    uint16_t major;
    uint16_t minor;
    uint16_t val;
    uint16_t range;
}ft800_designer_gauge;

typedef struct 
{
    bool visible;
    uint16_t cx_coord;
    uint16_t cy_coord;
    uint16_t diameter;
    uint32_t background_color;
    uint32_t pointer_color;
    ft800_designer_3d _3d;
    uint16_t val;
    uint8_t tag;
}ft800_designer_dial;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint32_t background_color;
    uint32_t thumb_color;
    ft800_designer_text text;
    ft800_designer_3d _3d;
    uint32_t state;
    uint8_t tag;
}ft800_designer_toggle;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    uint32_t background_color;
    uint32_t thumb_color;
    ft800_designer_3d _3d;
    uint32_t val;
    uint32_t size;
    uint8_t tag;
}ft800_designer_scroll_bar;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    uint32_t background_color_left;
    uint32_t background_color_right;
    uint32_t thumb_color;
    ft800_designer_3d _3d;
    uint16_t val;
    uint8_t tag;
}ft800_designer_slider;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    uint32_t background_color;
    uint32_t progress_color;
    ft800_designer_3d _3d;
    uint16_t val;
    uint16_t range;
}ft800_designer_progress_bar;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    uint16_t width;
    uint16_t height;
    uint8_t *image_data;
    uint32_t ramg_address;
    uint8_t  bitmap_handle;
    uint8_t  loaded;
    bool frame;
    ft800_designer_pen pen;
}ft800_designer_image;

typedef struct
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    ft800_designer_text text;
    ft800_alignment alignment;
}ft800_designer_label;

typedef struct 
{
    bool visible;
    uint16_t x_coord;
    uint16_t y_coord;
    int32_t num;
    uint32_t num_color;
    uint8_t num_size;
    ft800_alignment alignment;
}ft800_designer_numeric;

  /*! @} */ // ft800_structures
 /*! @} */ // ft800
/*! @} */ // mwgroup

#endif
// ------------------------------------------------------------------------- END
