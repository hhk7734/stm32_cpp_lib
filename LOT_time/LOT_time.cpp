/**
 * @file LOT_time.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 시간, 시간 지연
 */

#include "LOT_time.h"

extern HAL_TickFreqTypeDef uwTickFreq;

void HAL_IncTick( void ) { uwTick += uwTickFreq; }

void HAL_Delay( uint32_t Delay )
{
    uint32_t tickstart = HAL_GetTick();

    while( ( HAL_GetTick() - tickstart ) < Delay ) {}
}

LOT_time time;