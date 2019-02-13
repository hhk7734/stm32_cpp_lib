/**
 * @file LOT_time.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 시간, 시간 지연
 */

#ifndef _LOT_TIME_H_
#define _LOT_TIME_H_

#if defined( STM32F103xB )
#    include "stm32f1xx_hal.h"
#endif

/**
 * set system clock
 */
#define COTEX_TIMER_SYSTEM_DIV 1UL
#define SYSTEM_CORE_CLOCK 72000000UL
#define COUNT_PER_MICROSECOND ( SYSTEM_CORE_CLOCK / 1000000UL / COTEX_TIMER_SYSTEM_DIV )
#define ASSEMBLY_CYCLE 6UL                                                // delay_us()
#define US_TO_CYCLE ( SYSTEM_CORE_CLOCK / 1000000UL / ASSEMBLY_CYCLE )    // 72 / 6 == 12

extern __IO uint32_t uwTick;

class LOT_time {
public:
    /**
     * @brief 전원이 인가된 이후 흐른 시간(최대 49 시간 40 분)
     * @return uint32_t ms
     */
    uint32_t millis( void );

    /**
     * @brief 전원이 인가된 이후 흐른 시간(최대 71 분 30 초)
     * @return uint32_t us
     */
    uint32_t micros( void );

    /**
     * @brief Delay ms 시간 동안 시간 지연
     * @param uint32_t ms
     */
    void delay_ms( uint32_t ms );

    /**
     * @brief Delay us 시간동안 시간 지연
     * @param uint32_t us
     */
    void delay_us( uint16_t us );
};

inline uint32_t LOT_time::millis( void ) { return uwTick; }

inline uint32_t LOT_time::micros( void )
{
    uint32_t ms;
    uint32_t count;

    do
    {
        ms    = millis();
        count = SysTick->VAL;
        /// for interrupt
        __ASM volatile( "nop" );
        __ASM volatile( "nop" );
    } while( ms != millis() );

    return ( ( ms * 1000UL ) + ( SysTick->LOAD + 1 - count ) / COUNT_PER_MICROSECOND );
}

inline void LOT_time::delay_ms( uint32_t ms ) { HAL_Delay( ms ); }

inline void LOT_time::delay_us( uint16_t us )
{
    if( us > 0 )
    {
        uint32_t Delay = us * US_TO_CYCLE;
        --Delay;

        /// 8bit, 32bit 연산이 16bit 연산보다 느림
        /// @todo 103 / 120 보다 정확한 값 찾기
        if( ( Delay < 256 ) || ( Delay > 65536 ) ) { Delay = Delay * 103 / 120; }

        /// r0 = us
        /// label 1: r0 -= 1
        /// branch. if high, go to label 1 backward
        /// maybe 6 cycle
        __ASM volatile( "   mov r0, %[Delay] \n\t"
                        "1: subs r0, #1 \n\t"
                        "   bhi 1b \n\t" ::[Delay] "r"( Delay )
                        : "r0" );
    }
}

extern LOT_time time;

#endif    // _LOT_TIME_H_