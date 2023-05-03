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
	MAG_MEASUREMENT_MODE_NORMAL		= 0,
	MAG_MEASUREMENT_MODE_POS_BIAS	= 1,
	MAG_MEASUREMENT_MODE_NEG_BIAS	= 2
} hmc5883l_measurement_mode_t;

typedef enum {
	MAG_CONTINUOUS	= 0,
	MAG_SINGLE		= 1,
	MAG_IDLE		= 2,
	MAG_IDLE_2		= 3
} hmc5883l_mode_t;

typedef enum {
	HMC5883L_HIGHSPEED_DISABLED = 0,
	HMC5883L_HIGHSPEED_ENABLED = 1
} hmc5883l_highspeed_t;

typedef enum {
	HMC5883L_OUTPUT_RATE_0_75_HZ = 0,
	HMC5883L_OUTPUT_RATE_1_5_HZ = 1,
	HMC5883L_OUTPUT_RATE_3_HZ = 2,
	HMC5883L_OUTPUT_RATE_7_5_HZ = 3,
	HMC5883L_OUTPUT_RATE_15_HZ = 4,
	HMC5883L_OUTPUT_RATE_30_HZ = 5,
	HMC5883L_OUTPUT_RATE_75_HZ = 6
} hmc5883l_output_rate_t;

typedef enum {
	HMC5883L_SAMPLE_AVG_1 = 0,
	HMC5883L_SAMPLE_AVG_2 = 1,
	HMC5883L_SAMPLE_AVG_4 = 2,
	HMC5883L_SAMPLE_AVG_8 = 3
} hmc5883l_sample_avg_t;

typedef struct {
	hmc5883l_sample_avg_t samples_avg;
	hmc5883l_output_rate_t output_rate;
	hmc5883l_measurement_mode_t m_mode;
	hmc5883l_fs_t gain;
	hmc5883l_highspeed_t hs;
	hmc5883l_mode_t mode;
} hmc5883l_config_t;

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

extern esp_err_t hmc5883l_config(hmc5883l_handle_t sensor, const hmc5883l_config_t cfg);

extern esp_err_t hmc5883l_get_gain(hmc5883l_handle_t sensor, uint16_t* gain);
extern esp_err_t hmc5883l_get_raw_mag_field(hmc5883l_handle_t sensor, mag_field_raw_t* mag);
extern esp_err_t hmc5883l_get_mag_field(hmc5883l_handle_t sensor, mag_field_t* mag);

#endif //! HMC5883L_H