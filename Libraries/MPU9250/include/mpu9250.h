#ifndef MPU9250_H
#define MPU9250_H

#include "MikroSDK.Driver.I2C.Master"
#include "MikroSDK.Board"
#include "MikroSDK.Log"

#define MPU9250_CONFIG                   0x1A
#define MPU9250_GYRO_CONFIG              0x1B
#define MPU9250_ACCEL_CONFIG             0x1C
#define MPU9250_ACCEL_CONFIG_2           0x1D
#define MPU9250_INT_PIN_CFG              0x37
#define MPU9250_I2C_ADDRESS              0x68 
#define MPU9250_POWER_MGMT_1             0x6B
#define MPU9250_WHO_AM_I                 0x75
#define MPU9250_RESET                    0x80
#define MPU9250_DLPF_GYRO_250_ACCEL_218  0x00
#define MPU9250_DLPF_GYRO_184_ACCEL_218  0x01
#define MPU9250_DLPF_GYRO_92_ACCEL_99    0x02
#define MPU9250_DLPF_GYRO_41_ACCEL_42    0x03
#define MPU9250_DLPF_GYRO_20_ACCEL_20    0x04
#define MPU9250_DLPF_GYRO_10_ACCEL_10    0x05
#define MPU9250_DLPF_GYRO_5_ACCEL_5      0x06
#define MPU9250_DLPF_GYRO_3600_ACCEL_420 0x07
#define MPU9250_OFF                      0x00
#define MPU9250_BYPASS_EN                0x02
#define MPU9250_FSYNC_INT_MODE_EN        0x22
#define MPU9250_MODE_SLEEP               0x40
#define MPU9250_MODE_LOW_POWER           0x21
#define MPU9250_MODE_NORMAL              0x01
#define MPU9250_ACCEL_RANGE_2            0x00
#define MPU9250_ACCEL_RANGE_4            0x08
#define MPU9250_ACCEL_RANGE_8            0x10
#define MPU9250_ACCEL_RANGE_16           0x18
#define MPU9250_GYRO_RANGE_250           0x00
#define MPU9250_GYRO_RANGE_500           0x08
#define MPU9250_GYRO_RANGE_1000          0x10
#define MPU9250_GYRO_RANGE_2000          0x18
#define MPU9250_ACCEL_XOUT_H             0x3B
#define MPU9250_CALIBRATION_SEMPLES_NUM  500
#define MPU9250_MIKROBUS_NUMS            4
#define MPU9250_WHO_I_AM_RESPONDE        0x70
#define MPU9250_DATA_READ_BUFFER_SIZE    14
#define MPU9250_TEMP_COEF_1              333.87f
#define MPU9250_TEMP_COEF_2              21.0f
#define MPU9250_ACCEL_RANGE_2_COEF       16384.0f
#define MPU9250_ACCEL_RANGE_4_COEF       8192.0f
#define MPU9250_ACCEL_RANGE_8_COEF       4096.0f
#define MPU9250_ACCEL_RANGE_16_COEF      2048.0f
#define MPU9250_ACCEL_RANGE_COEF_DEF     131.0f 
#define MPU9250_GYRO_RANGE_250_COEF      131.0f
#define MPU9250_GYRO_RANGE_500_COEF      65.5f
#define MPU9250_GYRO_RANGE_1000_COEF     32.8f
#define MPU9250_GYRO_RANGE_2000_COEF     16.4f
#define MPU9250_GYRO_RANGE_COEF_DEF      131.0f 
#define MPU9250_DELAY_50_MS              50
#define MPU9250_DELAY_10_MS              10
#define MPU9250_DELAY_4_MS               4
#define MPU9250_SHIFT_8_BITS             8
        

typedef struct
{
    float accel_x;
    float accel_y;
    float accel_z;
    float temp; 
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float mag_x;
    float mag_y;
    float mag_z;
} mpu9250_data_t;

typedef struct
{
    float accel_bias_x;
    float accel_bias_y;
    float accel_bias_z;
    float gyro_bias_x;
    float gyro_bias_y;
    float gyro_bias_z;
}mpu9250_calibration_t;

typedef struct
{
    uint8_t accel_range;
    uint8_t gyro_range;
    uint8_t pwr_mode;
    uint8_t mag_mode;
    uint8_t dlpf;
    uint16_t accel_coef;
    float gyro_coef;
    mpu9250_calibration_t calibration;
}mpu9250_t;

typedef struct
{
    pin_name_t scl_pin;
    pin_name_t sda_pin;
    i2c_master_t i2c_master;
    i2c_master_config_t i2c_master_cfg;
    log_t *log;
    mpu9250_t ctx;
}mpu9250_cfg_t;

void mpu9250_write( mpu9250_cfg_t *cfg, uint8_t addr, uint8_t data );
uint8_t mpu9250_read( mpu9250_cfg_t *cfg, uint8_t addr );
void mpu9250_init( mpu9250_cfg_t *cfg );
void mpu9250_read_data( mpu9250_cfg_t *cfg, mpu9250_data_t *data );
void mpu9250_cfg_default( mpu9250_cfg_t *cfg );
void mpu9250_initialization( mpu9250_cfg_t *cfg );
void mpu9250_reset( mpu9250_cfg_t *cfg );
void mpu9250_set_power_mode( mpu9250_cfg_t *cfg );
void mpu9250_gyro_calibrate( mpu9250_cfg_t *cfg, mpu9250_data_t *data );
void mpu9250_accel_calibrate( mpu9250_cfg_t *cfg, mpu9250_data_t *data );
#endif