/**
 * @file LOT_lcd_i2c.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C로 LCD 제어
 */

#include "LOT_lcd_i2c.h"

const uint8_t REGISTER_SELECT { 1 };
const uint8_t READ_WRITE { 1 << 1 };
const uint8_t ENABLE_SIGNAL { 1 << 2 };

const uint8_t CLEAR_DISPLAY { 1 };
const uint8_t CURSOR_AT_HOME { 1 << 1 };
const uint8_t ENTRY_MODE_SET { 1 << 2 };
const uint8_t DISPLAY_CONTROL { 1 << 3 };
const uint8_t CURSOR_SHIFT { 1 << 4 };
const uint8_t FUNCTION_SET { 1 << 5 };
const uint8_t CG_RAM_ADDRESS_SET { 1 << 6 };
const uint8_t DD_RAM_ADDRESS_SET { 1 << 7 };

LOT_lcd_i2c::LOT_lcd_i2c( uint8_t _address )
    : address( _address )
{
}

void LOT_lcd_i2c::setup( void ) {}

void LOT_lcd_i2c::transmit_basic( uint8_t data ) {}