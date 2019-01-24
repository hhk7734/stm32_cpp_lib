/**
 * @file LOT_lcd_i2c.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C로 LCD 제어
 */

/// PCF8574T 주소 0x20 ~ 0x27, SCL 100 KHz
/// 4bit 제어
/// B7, B6, B5, B4, B3, B2, B1, B0
/// D7, D6, D5, D4, NULL, E, RW, RS

#ifndef _LOT_LCD_I2C_H_
#define _LOT_LCD_I2C_H_

#include "LOT_time.h"
#include "LOT_transmit.h"

#include "LOT_i2c1.h"
#define LOT_LCD_I2C i2c1
#warning "SCL 100KHz, standard mode"

const uint8_t LOT_FONT_5X8DOTS { 0 << 2 };
const uint8_t LOT_FONT_5X10DOTS { 1 << 2 };

class LOT_lcd_i2c : public LOT_transmit {
public:
    LOT_lcd_i2c( uint8_t _address,
                 uint8_t _columns,
                 uint8_t _rows,
                 uint8_t _font = LOT_FONT_5X8DOTS );

    void setup( void );

    virtual void transmit_basic( uint8_t data );
    virtual void transmit_basic( uint8_t *data, uint16_t size );

    /**
     * @brief backlight 끔
     */
    void backlight_off( void );

    /**
     * @brief backlight 켬
     */
    void backlight_on( void );

    /**
     * @brief DDRAM을 지우고, 커서를 0번 으로 이동
     */
    void clear( void );

    /**
     * @brief 화면과 커서를 0번 으로 이동
     */
    void home( void );

    /**
     * @brief 왼쪽에서 오른쪽으로 글 쓰기
     */
    void left_to_right( void );

    /**
     * @brief 오른쪽에서 왼쪽으로 글 쓰기
     */
    void right_to_left( void );

    /**
     * @brief 화면 이동 없이 글 쓰기(커서 위치 이동이 보임)
     */
    void autoscroll_off( void );

    /**
     * @brief 화면 이동시키며 글 쓰기(커서 위치가 고정된 것으로 보임)
     */
    void autoscroll_on( void );

    /*
     * @brief 화면 표시 끄기
     */
    void display_off( void );

    /**
     * @brief 화면 표시 켜기
     */
    void display_on( void );

    /**
     * @brief 커서 끄기
     */
    void cursor_off( void );

    /**
     * @brief 커서 켜기
     */
    void cursor_on( void );

    /**
     * @brief 커서 깜빡임 끄기
     */
    void blink_off( void );

    /**
     * @brief 커서 깜빡임 켜기
     */
    void blink_on( void );

    /**
     * @brief 커서 왼쪽으로 이동
     */
    void cursor_left( void );

    /**
     * @brief 커서 오른쪽으로 이동
     */
    void cursor_right( void );

    /**
     * @brief 화면 왼쪽으로 이동
     */
    void display_left( void );

    /**
     * @brief 화면 오른쪽으로 이동
     */
    void display_right( void );

    /**
     * @brief 커서 위치 이동
     * @param uint8_t columns 0 ~ 열
     * @param uint8_t rows 0 ~ 행
     */
    void set_cursor( uint8_t columns, uint8_t rows );

    /**
     * @brief 진행 막대, max + 1 칸 필요
     * @param uint8_t max 최대 칸 수
     * @param uint8_t bar 진행 정도
     * @param uint8_t columns 시작 열
     * @param uint8_t rows 시작 행
     * @param uint8_t start_mark 시작 표식 유무
     */
    void
        progress_bar( uint8_t max, uint8_t bar, uint8_t columns, uint8_t rows, uint8_t start_mark );

protected:
private:
    uint8_t address;
    uint8_t columns;
    uint8_t rows;
    uint8_t font;
    uint8_t backlight_mask;
    uint8_t entry_mode_set;
    uint8_t display_control;
    uint8_t function_set;

    /**
     * @brief i2c 통신으로 데이터 송신, 옵션에 backlight_mask 적용
     * @param uint8_t 4 bit 데이터와 옵션
     */
    void i2c_transmit( uint8_t data );

    /**
     * @brief enable 신호
     * @param uint8_t 4 bit 데이터와 옵션
     */
    void enable_signal( uint8_t data );

    /**
     * @brief 4 bit 데이터 송신 시작 후, enable_signal
     * @param uint8_t 4 bit 데이터와 옵션
     */
    void transmit_4bit( uint8_t data );

    /**
     * @brief 8 bit 데이터를 상위 4 bit와 하위 4 bit로 나눠 송신
     * @param uint8_t 8 bit 데이터
     * @param uint8_t 옵션
     */
    void transmit_8bit( uint8_t data, uint8_t mode );
};

#endif    // _LOT_LCD_I2C_H_