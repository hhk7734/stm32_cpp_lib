/**
 * @file LOT_uart2.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART2 통신
 */

#include "LOT_uart2.h"

LOT_uart2::LOT_uart2()
    : rx_buf_head( 0 )
    , rx_buf_tail( 0 )
    , tx_buf_head( 0 )
    , tx_buf_tail( 0 )
{
}

void LOT_uart2::transmit_basic( uint8_t data )
{
    /**
 * HAL_UART_Transmit_IT()
 * 
 * TC == 0
 * TXEIT == 1
 *      UART_Transmit_IT()
 *      data
 *      ...
 *      TXEIT == 0
 *      TCIT == 1
 * TC == 1
 * 
 * UART_EndTransmit_IT()
 * TCIT == 0
 * TxCpltCallback()
 */
    if( ( tx_buf_head == tx_buf_tail ) && __HAL_UART_GET_FLAG( &huart2, UART_FLAG_TC ) )
    { HAL_UART_Transmit_IT( &huart2, &data, 1 ); }
    else
    {
        tx_buf[tx_buf_head] = data;
        tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART2_TX_BUF_SIZE;
        /// 처리속도 때문에 따라잡기 직전에 비교해서 기다려야 함
        while( tx_buf_head
               == ( ( tx_buf_tail + LOT_UART2_TX_BUF_SIZE - 1 ) % LOT_UART2_TX_BUF_SIZE ) )
        {}
    }
}

uint16_t LOT_uart2::receive_basic( uint8_t *data, uint16_t max_size )
{
    uint8_t temp = ( LOT_UART2_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART2_RX_BUF_SIZE;
    if( temp < max_size ) { max_size = temp; }
    for( uint8_t i = 0; i < max_size; ++i )
    {
        data[i]     = rx_buf[rx_buf_tail];
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART2_RX_BUF_SIZE;
    }
    return max_size;
}

uint16_t LOT_uart2::receive_size( void )
{
    return ( LOT_UART2_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART2_RX_BUF_SIZE;
}

uint8_t LOT_uart2::receive_basic( void )
{
    uint8_t data = rx_buf[rx_buf_tail];
    rx_buf_tail  = ( rx_buf_tail + 1 ) % LOT_UART2_RX_BUF_SIZE;
    return data;
}

LOT_uart2 uart2;