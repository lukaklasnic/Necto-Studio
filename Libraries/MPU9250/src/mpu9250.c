#include "mpu9250.h"
#include "delays.h"

uint8_t mpu9250_read( mpu9250_cfg_t *cfg, uint8_t addr )
{
    uint8_t  rd_dat[ 1 ];
    uint8_t  wr_dat[ 1 ];
    wr_dat[ 0 ] = addr;

    i2c_master_write( &cfg -> i2c_master, &wr_dat, 1 );
    i2c_master_read( &cfg -> i2c_master, &rd_dat, 1 );

    return rd_dat[ 0 ];
}

void mpu9250_write( mpu9250_cfg_t *cfg, uint8_t addr, uint8_t data )
{
    uint8_t  wr_dat[ 2 ];
    wr_dat[ 0 ] = addr;
    wr_dat[ 1 ] = data;

    i2c_master_write( &cfg -> i2c_master, &wr_dat, 2 );
}

void mpu9250_reset( mpu9250_cfg_t *cfg )
{
    mpu9250_write( cfg, MPU9250_POWER_MGMT_1, MPU9250_RESET );
}

void mpu9250_set_power_mode( mpu9250_cfg_t *cfg )
{
    mpu9250_reset( cfg );
    mpu9250_write( cfg, MPU9250_POWER_MGMT_1, cfg -> ctx.pwr_mode ); 
    Delay_ms( 50 );
}

void mpu9250_set_dlpf_mode( mpu9250_cfg_t *cfg )
{
    mpu9250_write( cfg, MPU9250_CONFIG, cfg -> ctx.dlpf );
    mpu9250_write( cfg, MPU9250_ACCEL_CONFIG_2, cfg -> ctx.dlpf );
}

void mpu9250_set_gyro_range( mpu9250_cfg_t *cfg )
{
    mpu9250_write( cfg, MPU9250_GYRO_CONFIG, cfg -> ctx.gyro_range );
}

void mpu9250_set_accel_range( mpu9250_cfg_t *cfg )
{
    mpu9250_write( cfg, MPU9250_ACCEL_CONFIG, cfg -> ctx.accel_range );
}

void mpu9250_set_mag_mode( mpu9250_cfg_t *cfg )
{
    mpu9250_write( cfg, MPU9250_INT_PIN_CFG, cfg -> ctx.mag_mode );
}

void mpu9250_set_coef( mpu9250_cfg_t *cfg )
{
   switch( cfg -> ctx.accel_range )
    {
        
        case MPU9250_ACCEL_RANGE_2 :
            cfg -> ctx.accel_coef = MPU9250_ACCEL_RANGE_2_COEF;
            break;

        case MPU9250_ACCEL_RANGE_4:
            cfg -> ctx.accel_coef = MPU9250_ACCEL_RANGE_4_COEF;
            break;

        case MPU9250_ACCEL_RANGE_8 :
            cfg -> ctx.accel_coef = MPU9250_ACCEL_RANGE_8_COEF;
            break;

        case MPU9250_ACCEL_RANGE_16 :
            cfg -> ctx.accel_coef = MPU9250_ACCEL_RANGE_16_COEF;
            break;

        default:
            cfg -> ctx.accel_coef = MPU9250_ACCEL_RANGE_COEF_DEF;
            break;
    }

    switch( cfg -> ctx.gyro_range )
    {
        
        case MPU9250_GYRO_RANGE_250 :
            cfg -> ctx.gyro_coef = MPU9250_GYRO_RANGE_250_COEF;
            break;

        case MPU9250_GYRO_RANGE_500 :
            cfg -> ctx.gyro_coef = MPU9250_GYRO_RANGE_500_COEF;
            break;

        case MPU9250_GYRO_RANGE_1000 :
            cfg -> ctx.gyro_coef = MPU9250_GYRO_RANGE_1000_COEF;
            break;

        case MPU9250_GYRO_RANGE_2000 :
            cfg -> ctx.gyro_coef = MPU9250_GYRO_RANGE_2000_COEF;
            break;

        default:
            cfg -> ctx.gyro_coef = MPU9250_GYRO_RANGE_COEF_DEF;
            break;
    }
}

void mpu9250_cfg_default( mpu9250_cfg_t *cfg )
{
    cfg -> i2c_master_cfg.scl = MIKROBUS_1_SCL; 
    cfg -> i2c_master_cfg.sda = MIKROBUS_1_SDA; 
    cfg -> ctx.gyro_range = MPU9250_GYRO_RANGE_2000;
    cfg -> ctx.accel_range =MPU9250_ACCEL_RANGE_16;
    cfg -> ctx.pwr_mode = MPU9250_MODE_NORMAL;
    cfg -> ctx.dlpf = MPU9250_DLPF_GYRO_41_ACCEL_42;
    cfg -> ctx.mag_mode = MPU9250_BYPASS_EN;
    cfg -> i2c_master_cfg.speed = I2C_MASTER_SPEED_FULL;
    cfg -> i2c_master_cfg.timeout_pass_count = 0;
}

void mpu9250_init( mpu9250_cfg_t *cfg )
{
        i2c_master_open( &cfg -> i2c_master, &cfg -> i2c_master_cfg );
        i2c_master_set_timeout( &cfg -> i2c_master, cfg -> i2c_master_cfg.timeout_pass_count );
        i2c_master_set_speed( &cfg -> i2c_master, cfg -> i2c_master_cfg.speed );
        i2c_master_set_slave_address( &cfg -> i2c_master, MPU9250_I2C_ADDRESS );
        
        mpu9250_reset( cfg );
        mpu9250_set_power_mode( cfg );
        Delay_ms( MPU9250_DELAY_50_MS );
        mpu9250_set_dlpf_mode( cfg );
        mpu9250_set_gyro_range( cfg );
        mpu9250_set_accel_range( cfg );
        mpu9250_set_coef( cfg );
        mpu9250_set_mag_mode( cfg );
        Delay_ms( MPU9250_DELAY_10_MS );
}

void mpu9250_initialization( mpu9250_cfg_t *cfg )
{
    pin_name_t scl_pins[] = { MIKROBUS_1_SCL, MIKROBUS_2_SCL, MIKROBUS_3_SCL, MIKROBUS_4_SCL };
    pin_name_t sda_pins[] = { MIKROBUS_1_SDA, MIKROBUS_2_SDA, MIKROBUS_3_SDA, MIKROBUS_4_SDA };
    
    mpu9250_cfg_default( cfg );

    for ( uint8_t i = 0; i < MPU9250_MIKROBUS_NUMS; i++ )
    {
        cfg->i2c_master_cfg.scl = scl_pins[ i ];
        cfg->i2c_master_cfg.sda = sda_pins[ i ];
        
        i2c_master_open( &cfg -> i2c_master, &cfg -> i2c_master_cfg );
        mpu9250_init( cfg );
        if ( mpu9250_read( cfg, MPU9250_WHO_AM_I ) == MPU9250_WHO_I_AM_RESPONDE )
        {
            log_printf( cfg -> log, "MPU9250 found on mikroBUS_%d!\n", i + 1 );
            return; 
        }
    }
    
    log_printf( cfg -> log, "MPU9250 not found on mikroBUS!\n" );
    log_printf( cfg -> log, "Try manualy to set a pins!\n" );
}

void mpu9250_read_data( mpu9250_cfg_t *cfg, mpu9250_data_t *data )
{
    uint8_t buffer[ MPU9250_DATA_READ_BUFFER_SIZE ];
    uint8_t reg_addr = MPU9250_ACCEL_XOUT_H;

    i2c_master_write( &cfg -> i2c_master, &reg_addr, 1 );
    i2c_master_read( &cfg -> i2c_master, buffer, MPU9250_DATA_READ_BUFFER_SIZE );

    int16_t ax = ( int16_t )( ( ( uint16_t )buffer[ 0 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 1 ] );
    int16_t ay = ( int16_t )( ( ( uint16_t )buffer[ 2 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 3 ] );
    int16_t az = ( int16_t )( ( ( uint16_t )buffer[ 4 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 5 ] );

    int16_t temp = ( int16_t )( ( ( uint16_t )buffer[ 6 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 7 ] );

    int16_t gx = ( int16_t )( ( ( uint16_t )buffer[ 8 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 9 ] );
    int16_t gy = ( int16_t )( ( ( uint16_t )buffer[ 10 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 11 ] );
    int16_t gz = ( int16_t )( ( ( uint16_t )buffer[ 12 ] << MPU9250_SHIFT_8_BITS ) | buffer[ 13 ] );

    data -> accel_x = ( float )ax / cfg -> ctx.accel_coef - cfg -> ctx.calibration.accel_bias_x;
    data -> accel_y = ( float )ay / cfg -> ctx.accel_coef - cfg -> ctx.calibration.accel_bias_y;
    data -> accel_z = ( float )az / cfg -> ctx.accel_coef - cfg -> ctx.calibration.accel_bias_z;

    data -> temp = ( ( float )temp / MPU9250_TEMP_COEF_1 ) + MPU9250_TEMP_COEF_2;

    data -> gyro_x = ( float )gx / cfg -> ctx.gyro_coef - cfg -> ctx.calibration.gyro_bias_x;
    data -> gyro_y = ( float )gy / cfg -> ctx.gyro_coef - cfg -> ctx.calibration.gyro_bias_y;
    data -> gyro_z = ( float )gz / cfg -> ctx.gyro_coef - cfg -> ctx.calibration.gyro_bias_z;
}

void mpu9250_gyro_calibrate( mpu9250_cfg_t *cfg, mpu9250_data_t *data )
{
    float err_x = 0;
    float err_y = 0;
    float err_z = 0;

    for( uint16_t i = 0; i < MPU9250_CALIBRATION_SEMPLES_NUM; i++ )
    {
        mpu9250_read_data( cfg, data );
        err_x += data -> gyro_x ;
        err_y += data -> gyro_y ;
        err_z += data -> gyro_z ;
        Delay_ms( MPU9250_DELAY_4_MS );
    }

    cfg -> ctx.calibration.gyro_bias_x = err_x / MPU9250_CALIBRATION_SEMPLES_NUM;
    cfg -> ctx.calibration.gyro_bias_y = err_y / MPU9250_CALIBRATION_SEMPLES_NUM;
    cfg -> ctx.calibration.gyro_bias_z = err_z / MPU9250_CALIBRATION_SEMPLES_NUM;
}

void mpu9250_accel_calibrate( mpu9250_cfg_t *cfg, mpu9250_data_t *data )
{
    float err_x = 0;
    float err_y = 0;
    float err_z = 0;

    for( uint16_t i = 0; i < MPU9250_CALIBRATION_SEMPLES_NUM; i++ )
    {
        mpu9250_read_data( cfg, data );
        err_x += data -> accel_x ;
        err_y += data -> accel_y ;
        err_z += data -> accel_z ;
        Delay_ms( MPU9250_DELAY_4_MS );
    }

    cfg -> ctx.calibration.accel_bias_x = err_x / MPU9250_CALIBRATION_SEMPLES_NUM;
    cfg -> ctx.calibration.accel_bias_y = err_y / MPU9250_CALIBRATION_SEMPLES_NUM;
    cfg -> ctx.calibration.accel_bias_z = err_z / MPU9250_CALIBRATION_SEMPLES_NUM - 1;
}