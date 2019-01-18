/**
 * @file LOT_lcd_i2c.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C로 LCD 제어
 */

/// PCF8574T 주소 0x20 ~ 0x27, SCL 100 KHz
/// 4bit 제어

#ifndef _LOT_LCD_I2C_H_
#define _LOT_LCD_I2C_H_

#include "LOT_time.h"
#include "LOT_transmit.h"

#include "LOT_i2c1.h"
#define LCD_I2C i2c1;

class LOT_lcd_i2c : public LOT_transmit {
public:
    LOT_lcd_i2c( uint8_t _address );

    void setup( void );

    virtual void transmit_basic( uint8_t data );

protected:
private:
    uint8_t address;
};

#endif    // _LOT_LCD_I2C_H_