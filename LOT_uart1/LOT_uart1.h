
/**
 * @file LOT_uart1.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART0 통신
 */

#ifndef _LOT_UART1_H_
#define _LOT_UART1_H_

#include "stm32f1xx_hal.h"
#include "LOT_transmit.h"
#include "LOT_receive.h"

/// 버퍼 크기는 32, 64, 128 ... 2의 지수로 설정
#define LOT_UART1_TX_BUF_SIZE 64
#define LOT_UART1_RX_BUF_SIZE 64

extern UART_HandleTypeDef huart1;

class LOT_uart1
    : public LOT_transmit,
      public LOT_receive
{
  public:
    LOT_uart1();

    void rx_cplt_callback(UART_HandleTypeDef *huart);
    void tx_cplt_callback(UART_HandleTypeDef *huart);

    virtual void transmit_basic(uint8_t data);

    virtual uint16_t receive_basic(uint8_t *data, uint16_t max_size);
    virtual uint16_t receive_size(void);
    virtual uint8_t receive_basic(void);

  protected:
    volatile uint8_t rx_buf_head;
    volatile uint8_t rx_buf_tail;
    volatile uint8_t tx_buf_head;
    volatile uint8_t tx_buf_tail;

    uint8_t rx_buf[LOT_UART1_RX_BUF_SIZE];
    uint8_t tx_buf[LOT_UART1_TX_BUF_SIZE];

  private:
};

inline void LOT_uart1::rx_cplt_callback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        rx_buf_head = (rx_buf_head + 1) % LOT_UART1_RX_BUF_SIZE;
        if (rx_buf_head == rx_buf_tail)
        {
            rx_buf_tail = (rx_buf_tail + 1) % LOT_UART1_RX_BUF_SIZE;
        }
        HAL_UART_Receive_IT(&huart1, &rx_buf[rx_buf_head], 1);
    }
}

inline void LOT_uart1::tx_cplt_callback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (tx_buf_head != tx_buf_tail)
        {
            HAL_UART_Transmit_IT(&huart1, &tx_buf[tx_buf_tail], 1);
            tx_buf_tail = (tx_buf_tail + 1) % LOT_UART1_TX_BUF_SIZE;
        }
    }
}

extern LOT_uart1 uart1;

#endif // _LOT_UART0_H_