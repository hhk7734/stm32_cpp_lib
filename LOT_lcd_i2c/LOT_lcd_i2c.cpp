/**
 * @file LOT_lcd_i2c.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C로 LCD 제어
 */

#include "LOT_lcd_i2c.h"

/// B7, B6, B5, B4, B3, B2, B1, B0
/// D7, D6, D5, D4, NULL, E, RW, RS
const uint8_t BACKLIGHT_OFF { 0 << 3 };
const uint8_t BACKLIGHT_ON { 1 << 3 };
const uint8_t ENABLE_SIGNAL { 1 << 2 };
const uint8_t READ_WRITE { 1 << 1 };
const uint8_t REGISTER_SELECT { 1 };
const uint8_t COMMAND { 0 };

const uint8_t DD_RAM_ADDRESS_SET { 1 << 7 };
const uint8_t CG_RAM_ADDRESS_SET { 1 << 6 };
const uint8_t FUNCTION_SET { 1 << 5 };
const uint8_t CURSOR_DISPLAY_SHIFT { 1 << 4 };
const uint8_t DISPLAY_CONTROL { 1 << 3 };
const uint8_t ENTRY_MODE_SET { 1 << 2 };
const uint8_t CURSOR_AT_HOME { 1 << 1 };
const uint8_t CLEAR_DISPLAY { 1 };

/// ENTRY_MODE_SET
const uint8_t ENTRY_CURSOR_RIGHT { 0 << 1 };
const uint8_t ENTRY_CURSOR_LEFT { 1 << 1 };
const uint8_t ENTRY_DISPLAY_SHIFT_OFF { 0 };
const uint8_t ENTRY_DISPLAY_SHIFT_ON { 1 };

/// DISPLAY_CONTROL
const uint8_t DISPLAY_OFF { 0 << 2 };
const uint8_t DISPLAY_ON { 1 << 2 };
const uint8_t CURSOR_OFF { 0 << 1 };
const uint8_t CURSOR_ON { 1 << 1 };
const uint8_t BLINK_CURSOR_OFF { 0 };
const uint8_t BLINK_CURSOR_ON { 1 };

/// CURSOR_SHIFT
const uint8_t CURSOR_SHIFT { 0 << 3 };
const uint8_t DISPLAY_SHIFT { 1 << 3 };
const uint8_t SHIFT_LEFT { 0 << 2 };
const uint8_t SHIFT_RIGHT { 1 << 2 };

/// FUNCTION_SET
const uint8_t DATA_4BIT_MODE { 0 << 4 };
const uint8_t DATA_8BIT_MODE { 1 << 4 };
const uint8_t DISPLAY_1_LINE { 0 << 3 };
const uint8_t DISPLAY_2_LINE { 1 << 3 };
// const uint8_t LOT_FONT_5X8DOTS { 0 << 2 };
// const uint8_t LOT_FONT_5X10DOTS { 1 << 2 };

LOT_lcd_i2c::LOT_lcd_i2c( uint8_t _address, uint8_t _columns, uint8_t _rows, uint8_t _font )
    : address( _address )
    , columns( _columns )
    , rows( _rows )
    , font( _font )
    , backlight_mask( BACKLIGHT_OFF )
    , entry_mode_set( ENTRY_CURSOR_LEFT | ENTRY_DISPLAY_SHIFT_OFF )
    , display_control( DISPLAY_ON | CURSOR_OFF | BLINK_CURSOR_OFF )
    , function_set( DATA_4BIT_MODE | DISPLAY_1_LINE | LOT_FONT_5X8DOTS )
{
}

void LOT_lcd_i2c::setup( void )
{
    time.delay_ms( 50 );

    transmit_4bit( 0x30 );
    time.delay_ms( 5 );
    transmit_4bit( 0x30 );
    time.delay_ms( 5 );
    transmit_4bit( 0x30 );
    time.delay_ms( 1 );

    transmit_4bit( 0x20 );

    if( rows != 1 ) { function_set |= DISPLAY_2_LINE; }
    if( font != LOT_FONT_5X8DOTS ) { function_set |= LOT_FONT_5X10DOTS; }
    transmit_8bit( FUNCTION_SET | function_set, COMMAND );

    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );

    clear();

    transmit_8bit( ENTRY_MODE_SET | entry_mode_set, COMMAND );

    home();
}

void LOT_lcd_i2c::transmit_basic( uint8_t data ) { transmit_8bit( data, REGISTER_SELECT ); }

void LOT_lcd_i2c::backlight_off( void )
{
    backlight_mask = BACKLIGHT_OFF;
    i2c_transmit( 0 );
}

void LOT_lcd_i2c::backlight_on( void )
{
    backlight_mask = BACKLIGHT_ON;
    i2c_transmit( 0 );
}

void LOT_lcd_i2c::clear( void ) { transmit_8bit( CLEAR_DISPLAY, COMMAND ); }

void LOT_lcd_i2c::home( void ) { transmit_8bit( CURSOR_AT_HOME, COMMAND ); }

void LOT_lcd_i2c::left_to_right( void )
{
    entry_mode_set |= ENTRY_CURSOR_LEFT;
    transmit_8bit( ENTRY_MODE_SET | entry_mode_set, COMMAND );
}

void LOT_lcd_i2c::right_to_left( void )
{
    entry_mode_set &= ~ENTRY_CURSOR_LEFT;
    transmit_8bit( ENTRY_MODE_SET | entry_mode_set, COMMAND );
}

void LOT_lcd_i2c::autoscroll_off( void )
{
    entry_mode_set &= ~ENTRY_DISPLAY_SHIFT_ON;
    transmit_8bit( ENTRY_MODE_SET | entry_mode_set, COMMAND );
}

void LOT_lcd_i2c::autoscroll_on( void )
{
    entry_mode_set |= ENTRY_DISPLAY_SHIFT_ON;
    transmit_8bit( ENTRY_MODE_SET | entry_mode_set, COMMAND );
}

void LOT_lcd_i2c::display_off( void )
{
    display_control &= ~DISPLAY_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}

void LOT_lcd_i2c::display_on( void )
{
    display_control |= DISPLAY_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}

void LOT_lcd_i2c::cursor_off( void )
{
    display_control &= ~CURSOR_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}

void LOT_lcd_i2c::cursor_on( void )
{
    display_control |= CURSOR_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}

void LOT_lcd_i2c::blink_off( void )
{
    display_control &= ~BLINK_CURSOR_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}
void LOT_lcd_i2c::blink_on( void )
{
    display_control |= BLINK_CURSOR_ON;
    transmit_8bit( DISPLAY_CONTROL | display_control, COMMAND );
}

void LOT_lcd_i2c::cursor_left( void )
{
    transmit_8bit( CURSOR_DISPLAY_SHIFT | CURSOR_SHIFT | SHIFT_LEFT, COMMAND );
}

void LOT_lcd_i2c::cursor_right( void )
{
    transmit_8bit( CURSOR_DISPLAY_SHIFT | CURSOR_SHIFT | SHIFT_RIGHT, COMMAND );
}
void LOT_lcd_i2c::display_left( void )
{
    transmit_8bit( CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT | SHIFT_LEFT, COMMAND );
}
void LOT_lcd_i2c::display_right( void )
{
    transmit_8bit( CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT | SHIFT_RIGHT, COMMAND );
}

void LOT_lcd_i2c::set_cursor( uint8_t columns, uint8_t rows )
{
    uint8_t row_offset[] = { 0x00, 0x40 };
    transmit_8bit( DD_RAM_ADDRESS_SET | ( columns + row_offset[rows] ), COMMAND );
}

void LOT_lcd_i2c::i2c_transmit( uint8_t data )
{
    data |= backlight_mask;
    LOT_LCD_I2C.transmit( address, &data, 1 );
}

void LOT_lcd_i2c::enable_signal( uint8_t data )
{
    i2c_transmit( data | ENABLE_SIGNAL );
    time.delay_us( 1 );
    i2c_transmit( data & ~ENABLE_SIGNAL );
    time.delay_us( 7 );
}

void LOT_lcd_i2c::transmit_4bit( uint8_t data )
{
    i2c_transmit( data );
    enable_signal( data );
}

void LOT_lcd_i2c::transmit_8bit( uint8_t data, uint8_t mode )
{
    transmit_4bit( ( data & 0xF0 ) | mode );
    transmit_4bit( ( ( data << 4 ) & 0xF0 ) | mode );
}