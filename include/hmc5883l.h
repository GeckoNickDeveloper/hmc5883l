#ifndef HMC5883L_H
#define HMC5883L_H

#include "driver/i2c.h"
#include "driver/gpio.h"

#define HMC5883L_ADDRESS 0x1E

typedef void* hmc5883l_handle_t;

typedef enum {
	MAG_FS_0_88GA	= 0,
	MAG_FS_1_3GA	= 1,
	MAG_FS_1_9GA	= 2,
	MAG_FS_2_5GA	= 3,
	MAG_FS_4_0GA	= 4,
	MAG_FS_4_7GA	= 5,
	MAG_FS_5_6GA	= 6,
	MAG_FS_8_1GA	= 7
} hmc5883l_fs_t; // TODO RENAME

typedef enum {
	MAG_MEASURMENT_MODE_NORMAL		= 0,
	MAG_MEASURMENT_MODE_POS_BIAS	= 1,
	MAG_MEASURMENT_MODE_NEG_BIAS	= 2
} hmc5883l_measurment_mode_t;

typedef enum {
	MAG_CONTINUOUS	= 0,
	MAG_SINGLE		= 1,
	MAG_IDLE		= 2,
	MAG_IDLE_2		= 3
} hmc5883l_mode_t;

typedef struct {
	int16_t raw_x;
	int16_t raw_y;
	int16_t raw_z;
} mag_field_raw_t;

typedef struct {
	float x;
	float y;
	float z;
} mag_field_t;



extern hmc5883l_handle_t hmc5883l_create(i2c_port_t port, uint16_t addr); // dev addresse are fixed

extern esp_err_t hmc5883l_delete(hmc5883l_handle_t sensor);

extern esp_err_t hmc5883l_config(hmc5883l_handle_t sensor, const hmc5883l_fs_t scale, const hmc5883l_measurement_mode_t m_mode, const hmc5883l_mode_t mode); // TODO edit

extern esp_err_t hmc5883l_get_raw_mag_field(hmc5883l_handle_t sensor, mag_field_raw_t* mag);
extern esp_err_t hmc5883l_get_mag_field(hmc5883l_handle_t sensor, mag_field_t* mag);

extern esp_err_t hmc5883l_get_gain(hmc5883l_handle_t sensor, uint16_t* gain);

#endif //! HMC5883L_H