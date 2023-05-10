#ifndef HMC5883L_H
#define HMC5883L_H

#include "driver/i2c.h"
#include "driver/gpio.h"

#define HMC5883L_ADDRESS 0x1E

/**
 * @typedef hmc5883l_handle_t
 */
typedef void* hmc5883l_handle_t;

/**
 * @enum hmc5883l_fs_t
 * @brief Used for configure the sensor gain (scale)
 * 
 * @todo rename
 */
typedef enum {
	MAG_FS_0_88GA	= 0,
	MAG_FS_1_3GA	= 1,
	MAG_FS_1_9GA	= 2,
	MAG_FS_2_5GA	= 3,
	MAG_FS_4_0GA	= 4,
	MAG_FS_4_7GA	= 5,
	MAG_FS_5_6GA	= 6,
	MAG_FS_8_1GA	= 7
} hmc5883l_fs_t;

/**
 * @enum hmc5883l_measurement_mode_t
 * @brief Used for configure the sensor measuremenet mode
 * 
 * @todo rename
 */
typedef enum {
	MAG_MEASUREMENT_MODE_NORMAL		= 0,
	MAG_MEASUREMENT_MODE_POS_BIAS	= 1,
	MAG_MEASUREMENT_MODE_NEG_BIAS	= 2
} hmc5883l_measurement_mode_t;

/**
 * @enum hmc5883l_mode_t
 * @brief Used for configure the sensor mode
 * 
 * @todo rename
 */
typedef enum {
	MAG_CONTINUOUS	= 0,
	MAG_SINGLE		= 1,
	MAG_IDLE		= 2,
	MAG_IDLE_2		= 3
} hmc5883l_mode_t;

/**
 * @enum hmc5883l_highspeed_t
 * @brief Used for enabling I2C highspeed (3400kHz)
 */
typedef enum {
	HMC5883L_HIGHSPEED_DISABLED = 0,
	HMC5883L_HIGHSPEED_ENABLED = 1
} hmc5883l_highspeed_t;

/**
 * @enum hmc5883l_output_rate_t
 * @brief Used for configure the sensor output rate
 */
typedef enum {
	HMC5883L_OUTPUT_RATE_0_75_HZ = 0,
	HMC5883L_OUTPUT_RATE_1_5_HZ = 1,
	HMC5883L_OUTPUT_RATE_3_HZ = 2,
	HMC5883L_OUTPUT_RATE_7_5_HZ = 3,
	HMC5883L_OUTPUT_RATE_15_HZ = 4,
	HMC5883L_OUTPUT_RATE_30_HZ = 5,
	HMC5883L_OUTPUT_RATE_75_HZ = 6
} hmc5883l_output_rate_t;

/**
 * @enum hmc5883l_sample_avg_t
 * @brief Used for configure the sensor sample avarage
 */
typedef enum {
	HMC5883L_SAMPLE_AVG_1 = 0,
	HMC5883L_SAMPLE_AVG_2 = 1,
	HMC5883L_SAMPLE_AVG_4 = 2,
	HMC5883L_SAMPLE_AVG_8 = 3
} hmc5883l_sample_avg_t;

/**
 * @struct hmc5883l_config_t
 * @brief Used to full configurate the sensor with an unique structure
 */
typedef struct {
	hmc5883l_sample_avg_t samples_avg;
	hmc5883l_output_rate_t output_rate;
	hmc5883l_measurement_mode_t m_mode;
	hmc5883l_fs_t gain;
	hmc5883l_highspeed_t hs;
	hmc5883l_mode_t mode;
} hmc5883l_config_t;

/**
 * @struct mag_field_raw_t
 * @brief Rappresents the raw values read by the sensor
 */
typedef struct {
	int16_t raw_x;
	int16_t raw_y;
	int16_t raw_z;
} mag_field_raw_t;

/**
 * @struct mag_field_t
 * @brief Rappresents the values read by the sensor adjusted with the configured gain
 */
typedef struct {
	float x;
	float y;
	float z;
} mag_field_t;


/**
 * @brief Create a sensor handler given the I2C port and sensor address
 * 
 * @param port I2C port
 * @param addr I2C sensor address
 * 
 * @return hmc5883l_handle_t Sensor handler
 */
extern hmc5883l_handle_t hmc5883l_create(i2c_port_t port, uint16_t addr); // dev addresse are fixed
/**
 * @brief Delete the given handler
 * 
 * @param sensor Sensor handler
 * 
 * @return esp_err_t
 */
extern esp_err_t hmc5883l_delete(hmc5883l_handle_t sensor);

/**
 * @brief Apply to the given sensor a specific configuration
 * 
 * @param sensor Sensor handler
 * @param cfg Configuration to apply
 * 
 * @return esp_err_t
 */
extern esp_err_t hmc5883l_config(hmc5883l_handle_t sensor, const hmc5883l_config_t cfg);

/**
 * @brief Retrieve the configured gain value
 * 
 * @param sensor Sensor handler
 * @param gain Pointer to store the configured gain value
 * 
 * @return esp_err_t
 */
extern esp_err_t hmc5883l_get_gain(hmc5883l_handle_t sensor, uint16_t* gain);
/**
 * @brief Retrieve the raw readings of the sensor
 * 
 * @param sensor Sensor handler
 * @param mag Pointer to store the raw reading
 * 
 * @return esp_err_t
 */
extern esp_err_t hmc5883l_get_raw_mag_field(hmc5883l_handle_t sensor, mag_field_raw_t* mag);
/**
 * @brief Retrieve the readings of the sensor adjusted according to current gain
 * 
 * @param sensor Sensor handler
 * @param mag Pointer to store the reading
 * 
 * @return esp_err_t
 */
extern esp_err_t hmc5883l_get_mag_field(hmc5883l_handle_t sensor, mag_field_t* mag);

#endif //! HMC5883L_H