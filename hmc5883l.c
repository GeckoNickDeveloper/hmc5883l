#include "esp_system.h"
#include "driver/i2c.h"

#include "hmc5883l.h"

typedef struct {
	i2c_port_t bus;
	uint16_t dev_addr;
} hmc5883l_dev_t;

