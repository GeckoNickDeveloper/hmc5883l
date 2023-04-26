#ifndef HMC5883L_H
#define HMC5883L_H

#include "driver/i2c.h"
#include "driver/gpio.h"

#define HMC5883L_WRITE_ADDRESS 0x3C
#define HMC5883L_READ_ADDRESS 0x3D

typedef void* hmc5883l_handle_t;

typedef enum {
	MAG_FS_0_88GA = 0,
	MAG_FS_1_3GA,
	MAG_FS_1_9GA,
	MAG_FS_2_5GA,
	MAG_FS_4_0GA,
	MAG_FS_4_7GA,
	MAG_FS_5_6GA,
	MAG_FS_8_1GA
} hmc5883l_fs_t;

typedef enum {
	MAG_MEASURMENT_MODE_NORMAL = 0,
	MAG_MEASURMENT_MODE_POS_BIAS,
	MAG_MEASURMENT_MODE_NEG_BIAS
} hmc5883l_measurment_mode_t;

typedef enum {
	MAG_CONTINUOUS = 0,
	MAG_SINGLE,
	MAG_IDLE,
	MAG_IDLE_2
} hmc5883l_mode_t;

typedef struct {
	float x;
	float y;
	float z;
} mag_field_t;



hmc5883l_handle_t hmc5883l_create(i2c_port_t port); // dev addresse are fixed
void hmc5883l_delete(hmc5883l_handle_t sensor);

esp_err_t hmc5883l_config(hmc5883l_handle_t sensor, const hmc5883l_fs_t scale, const hmc5883l_measurement_mode_t m_mode, const hmc5883l_mode_t mode); // TODO edit
esp_err_t hmc5883l_get_mag_field(hmc5883l_handle_t sensor, mag_field_t* mag);

#endif //! HMC5883L_H