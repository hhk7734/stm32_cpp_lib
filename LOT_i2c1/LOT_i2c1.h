/**
 * @file LOT_i2c1.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C1 마스터 통신
 */

#ifndef _LOT_I2C1_H_
#define _LOT_I2C1_H_

#if defined( STM32F103xB )
#    include "stm32f1xx_hal.h"
#endif

extern I2C_HandleTypeDef hi2c1;

class LOT_i2c1 {
public:
    inline LOT_i2c1() {}

    /**
     * @brief slave 장치로 데이터 송신
     * @param uint8_t slave_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transmit( const uint8_t slave_address, uint8_t *data, uint8_t size );
    /**
     * @brief slave 장치의 register로 송신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size 연속 쓰기가 가능한 경우 연속된 레지스터 수
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef transmit( const uint8_t slave_address,
                                const uint8_t register_address,
                                uint8_t *     data,
                                uint8_t       size );
    /**
     * @brief slave 장치의 register로 송신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t data
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef
        transmit( const uint8_t slave_address, const uint8_t register_address, uint8_t data );

    /**
     * @brief slave 장치에서 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t *data 수신 데이터를 저장
     * @param uint8_t size 수신할 데이터 수
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef receive( const uint8_t slave_address, uint8_t *data, uint8_t size );
    /**
     * @brief slave 장치의 register 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t *data 레지스터의 데이터
     * @param uint8_t size 연속 읽기가 가능한 경우 연속된 레지스터 수
     * @return HAL_StatusTypeDef
     */
    HAL_StatusTypeDef receive( const uint8_t slave_address,
                               const uint8_t register_address,
                               uint8_t *     data,
                               uint8_t       size );
    /**
     * @brief slave 장치의 register 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @return uint8_t 레지스터의 데이터
     */
    uint8_t receive( const uint8_t slave_address, const uint8_t register_address );

protected:
private:
};

inline HAL_StatusTypeDef
    LOT_i2c1::transmit( const uint8_t slave_address, uint8_t *data, uint8_t size )
{
    return HAL_I2C_Master_Transmit( &hi2c1, slave_address << 1, data, size, 10 * size );
}

inline HAL_StatusTypeDef LOT_i2c1::transmit( const uint8_t slave_address,
                                             const uint8_t register_address,
                                             uint8_t *     data,
                                             uint8_t       size )
{
    return HAL_I2C_Mem_Write( &hi2c1, slave_address << 1, register_address, I2C_MEMADD_SIZE_8BIT,
                              data, size, 10 * size );
}

inline HAL_StatusTypeDef
    LOT_i2c1::transmit( const uint8_t slave_address, const uint8_t register_address, uint8_t data )
{
    return HAL_I2C_Mem_Write( &hi2c1, slave_address << 1, register_address, I2C_MEMADD_SIZE_8BIT,
                              &data, 1, 10 );
}

inline HAL_StatusTypeDef
    LOT_i2c1::receive( const uint8_t slave_address, uint8_t *data, uint8_t size )
{
    return HAL_I2C_Master_Receive( &hi2c1, slave_address << 1, data, size, 10 * size );
}

inline HAL_StatusTypeDef LOT_i2c1::receive( const uint8_t slave_address,
                                            const uint8_t register_address,
                                            uint8_t *     data,
                                            uint8_t       size )
{
    return HAL_I2C_Mem_Read( &hi2c1, slave_address << 1, register_address, I2C_MEMADD_SIZE_8BIT,
                             data, size, 10 * size );
}

inline uint8_t LOT_i2c1::receive( const uint8_t slave_address, const uint8_t register_address )
{
    uint8_t temp;
    HAL_I2C_Mem_Read( &hi2c1, slave_address << 1, register_address, I2C_MEMADD_SIZE_8BIT, &temp, 1,
                      10 );
    return temp;
}

extern LOT_i2c1 i2c1;

#endif    // _LOT_I2C1_H_
