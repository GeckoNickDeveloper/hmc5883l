#include "esp_system.h"
#include "driver/i2c.h"

#include "hmc5883l.h"


#define HMC5883L_CONFIG_A_REG 0x00
#define HMC5883L_CONFIG_B_REG 0x01
#define HMC5883L_MODE_REG 0x02
#define HMC5883L_DATA_X_REG_0 0x03
#define HMC5883L_DATA_X_REG_1 0x04
#define HMC5883L_DATA_Z_REG_0 0x05
#define HMC5883L_DATA_Z_REG_1 0x06
#define HMC5883L_DATA_Y_REG_0 0x07
#define HMC5883L_DATA_Y_REG_1 0x08
#define HMC5883L_STATUS_REG 0x09
#define HMC5883L_IDA_REG 0x0A
#define HMC5883L_IDB_REG 0x0B
#define HMC5883L_IDC_REG 0x0C


/**
 * @struct hmc5883l_dev_t
 * @brief Used to descrive a sensor
 */
typedef struct {
	i2c_port_t bus;
	uint16_t dev_addr;
} hmc5883l_dev_t;


/**
 * @brief Create an handle for the sensor
 * 
 * @param port Specify the I2C port
 * @param addr Specify the sensor address
 * @return hmc5883l_handle_t Handle of the sensor
 */
hmc5883l_handle_t hmc5883l_create(i2c_port_t port, uint16_t addr)
{
	hmc5883l_dev_t* sensor = (hmc5883l_dev_t*) calloc(1, sizeof(hmc5883l_dev_t));

	sensor->bus = port;
	sensor->dev_addr = addr << 1;
	
	return (hmc5883l_handle_t) sensor;
};

/**
 * @brief Delete the sensor handler
 * 
 * @param sensor The handler to delete
 * @return esp_err_t 
 */
esp_err_t hmc5883l_delete(hmc5883l_handle_t sensor)
{
	hmc5883l_dev_t *sens = (hmc5883l_dev_t *) sensor;

    free(sens);
    
	return ESP_OK;
};


/**
 * @brief Read the specified number of bytes starting from the given register
 * 
 * @param sensor Sensor handler to read
 * @param reg_start_addr Register where to start reading
 * @param data_buf Where to put data
 * @param data_len Number of bytes to read
 * @return esp_err_t 
 */
static esp_err_t hmc5883l_read(hmc5883l_handle_t sensor, const uint8_t reg_start_addr, uint8_t *const data_buf, const uint8_t data_len)
{
	hmc5883l_dev_t *sens = (hmc5883l_dev_t *) sensor;
    esp_err_t  ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ret = i2c_master_start(cmd);
    assert(ESP_OK == ret);
    ret = i2c_master_write_byte(cmd, sens->dev_addr | I2C_MASTER_WRITE, true);
    assert(ESP_OK == ret);
    ret = i2c_master_write_byte(cmd, reg_start_addr, true);
    assert(ESP_OK == ret);
    ret = i2c_master_start(cmd);
    assert(ESP_OK == ret);
    ret = i2c_master_write_byte(cmd, sens->dev_addr | I2C_MASTER_READ, true);
    assert(ESP_OK == ret);
    ret = i2c_master_read(cmd, data_buf, data_len, I2C_MASTER_LAST_NACK);
    assert(ESP_OK == ret);
    ret = i2c_master_stop(cmd);
    assert(ESP_OK == ret);
    ret = i2c_master_cmd_begin(sens->bus, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
};

/**
 * @brief Write the specified number of bytes starting from the given register
 * 
 * @param sensor Sensor handler to write
 * @param reg_start_addr Register where to start writing
 * @param data_buf Data to write
 * @param data_len Number of bytes to write
 * @return esp_err_t 
 */
static esp_err_t hmc5883l_write(hmc5883l_handle_t sensor, const uint8_t reg_start_addr, const uint8_t *const data_buf, const uint8_t data_len)
{
	hmc5883l_dev_t *sens = (hmc5883l_dev_t *) sensor;
    esp_err_t  ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ret = i2c_master_start(cmd);
    assert(ESP_OK == ret);
    ret = i2c_master_write_byte(cmd, sens->dev_addr | I2C_MASTER_WRITE, true);
    assert(ESP_OK == ret);
    ret = i2c_master_write_byte(cmd, reg_start_addr, true);
    assert(ESP_OK == ret);
    ret = i2c_master_write(cmd, data_buf, data_len, true);
    assert(ESP_OK == ret);
    ret = i2c_master_stop(cmd);
    assert(ESP_OK == ret);
    ret = i2c_master_cmd_begin(sens->bus, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
};


/**
 * @brief Configure the sensor according the cfg parameter
 * 
 * @param sensor Sensore to configure
 * @param cfg Configuration to use
 * @return esp_err_t 
 */
esp_err_t hmc5883l_config(hmc5883l_handle_t sensor, const hmc5883l_config_t cfg)
{
	int8_t config[3];
	config[0] = (cfg.samples_avg << 5) | (cfg.output_rate << 2) | cfg.m_mode;
	config[1] = (cfg.gain << 5);
	config[2] = (cfg.hs << 7) | cfg.mode;

	return hmc5883l_write(sensor, HMC5883L_CONFIG_A_REG, config, sizeof(config));
};


/**
 * @brief Retrieves gain from configuration
 * 
 * @param sensor Sensor
 * @param gain Pointer where to store the gain
 * @return esp_err_t 
 */
esp_err_t hmc5883l_get_gain(hmc5883l_handle_t sensor, uint16_t* gain)
{
	esp_err_t ret;

	uint8_t fs;
	*gain = (uint16_t) 0x0000;

	ret = hmc5883l_read(sensor, HMC5883L_CONFIG_B_REG, &fs, 1);

	if (ret |= ESP_OK)
		return ret;

	fs >>= 5;

	switch (fs) {
	case MAG_FS_0_88GA:
		*gain = (uint16_t) 1370;
		break;
	case MAG_FS_1_3GA:
		*gain = (uint16_t) 1090;
		break;
	case MAG_FS_1_9GA:
		*gain = (uint16_t) 820;
		break;
	case MAG_FS_2_5GA:
		*gain = (uint16_t) 660;
		break;
	case MAG_FS_4_0GA:
		*gain = (uint16_t) 440;
		break;
	case MAG_FS_4_7GA:
		*gain = (uint16_t) 390;
		break;
	case MAG_FS_5_6GA:
		*gain = (uint16_t) 330;
		break;
	case MAG_FS_8_1GA:
		*gain = (uint16_t) 230;
		break;
	
	default:
		*gain = (uint16_t) 1370;
		break;
	}

	return ret;
};

/**
 * @brief Retrieves raw readings
 * 
 * @param sensor Sensor
 * @param mag Pointer where to store the raw reading
 * @return esp_err_t 
 */
esp_err_t hmc5883l_get_raw_mag_field(hmc5883l_handle_t sensor, mag_field_raw_t* mag)
{
	esp_err_t ret;

	uint8_t data[6];

	ret = hmc5883l_read(sensor, HMC5883L_DATA_X_REG_0, data, 6);

	if (ret != ESP_OK)
		return ret;

	mag->raw_x = (int16_t) ((data[0] << 8) | data[1]);
	mag->raw_z = (int16_t) ((data[2] << 8) | data[3]);
	mag->raw_y = (int16_t) ((data[4] << 8) | data[5]);

	return ret;
};

/**
 * @brief Retrieves the readings of the sensor
 * 
 * @param sensor Sensor
 * @param mag Pointer where to store the readings
 * @return esp_err_t 
 */
esp_err_t hmc5883l_get_mag_field(hmc5883l_handle_t sensor, mag_field_t* mag)
{
	esp_err_t ret;
	uint16_t gain;
	mag_field_raw_t raw;

	ret = hmc5883l_get_raw_mag_field(sensor, &raw);
	if (ret != ESP_OK)
		return ret;
	
	ret = hmc5883l_get_gain(sensor, &gain);
	if (ret != ESP_OK)
		return ret;
	
	mag->x = (float) raw.raw_x / (float) gain;
	mag->y = (float) raw.raw_y / (float) gain;
	mag->z = (float) raw.raw_z / (float) gain;

	return ret;
};
