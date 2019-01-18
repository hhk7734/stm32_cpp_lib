/**
 * @file LOT_spi1.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief SPI1 마스터 통신
 */

#ifndef _LOT_SPI1_H_
#define _LOT_SPI1_H_

#if defined( STM32F103xB )
#    include "stm32f1xx_hal.h"
#endif

extern SPI_HandleTypeDef hspi1;

/// 레지스터의 값을 읽어올 때, 레지스터 주소에 마스크를 설정해야 하는 경우가 많음
#define LOT_SPI1_READ_MASK 0x80

class LOT_spi1 {
public:
    inline LOT_spi1() {}

    /**
     * @brief 데이터 송신
     * @param uint8_t data
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transmit( uint8_t data );

    /**
     * @brief 레지스터에 데이터 쓰기, 연속 쓰기가 가능한 경우 연속적인 레지스터에 접근 가능
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transmit( uint8_t register_address, uint8_t *data, uint8_t size );

    /**
     * @brief 레지스터에 데이터 쓰기
     * @param uint8_t register_address
     * @param uint8_t data
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transmit( uint8_t register_address, uint8_t data );

    /**
     * @brief 레지스터의 데이터 읽기, 연속 읽기가 가능한 경우 연속적인 레지스터에 접근 가능
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef receive( uint8_t register_address, uint8_t *data, uint8_t size );

    /**
     * @brief 레지스터의 데이터 읽기
     * @param uint8_t register_address
     * @return uint8_t 수신 데이터
     */
    uint8_t receive( uint8_t register_address );

    /**
     * @brief 데이터 송수신
     * @param uint8_t *data 입력 데이터를 송신하고, 수신 데이터를 저장
     * @param uint8_t size
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transceive( uint8_t *data, uint8_t size );

    /**
     * @brief 데이터 송수신
     * @param uint8_t data 송신 데이터
     * @return uint8_t uint8_t 수신 데이터
     */
    uint8_t transceive( uint8_t data );

protected:
private:
};

inline HAL_StatusTypeDef LOT_spi1::transmit( uint8_t data )
{
    return HAL_SPI_Transmit( &hspi1, &data, 1, 10 );
}

inline HAL_StatusTypeDef LOT_spi1::transmit( uint8_t register_address, uint8_t *data, uint8_t size )
{
    transmit( register_address );
    return HAL_SPI_Transmit( &hspi1, data, size, 10 * size );
}

inline HAL_StatusTypeDef LOT_spi1::transmit( uint8_t register_address, uint8_t data )
{
    return transmit( register_address, &data, 1 );
}

inline HAL_StatusTypeDef LOT_spi1::receive( uint8_t register_address, uint8_t *data, uint8_t size )
{
    transmit( register_address | LOT_SPI1_READ_MASK );
    return HAL_SPI_Receive( &hspi1, data, size, 10 * size );
}

inline uint8_t LOT_spi1::receive( uint8_t register_address )
{
    uint8_t temp;
    receive( register_address, &temp, 1 );
    return temp;
}

inline HAL_StatusTypeDef LOT_spi1::transceive( uint8_t *data, uint8_t size )
{
    return HAL_SPI_TransmitReceive( &hspi1, data, data, size, 10 * size );
}

inline uint8_t LOT_spi1::transceive( uint8_t data )
{
    transceive( &data, 1 );
    return data;
}

extern LOT_spi1 spi1;
#endif    // _LOT_SPI1_H_