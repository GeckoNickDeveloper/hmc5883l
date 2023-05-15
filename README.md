# HMC5883L Driver Component

This is a driver component for HMC5883L magnetometer sensor.

The sensor's datasheet can be found [here](https://www.digikey.it/it/htmldatasheets/production/786600/0/0/1/asd2613-r.html).

## Installation

At the moment, this is not an official component. To use it in your ESP-IDF project just copy the source files in a local component (sorry for the inconvenience but I still working on that)

## Getting Started

Include the header file

```
#include <hmc5883l.h>
```

Initialize the I2C protocol and create an handler:
```
hmc5883l_handle_t hmc5883l = hmc5883l_create(i2c_master_port, HMC5883L_ADDRESS);
```

After initializing I2C, create a configuration variable:
```
hmc5883l_config_t cfg = {
	.gain = MAG_FS_8_1GA,
	.output_rate = HMC5883L_OUTPUT_RATE_15_HZ,
	.samples_avg = HMC5883L_SAMPLE_AVG_1,
	.m_mode = MAG_MEASUREMENT_MODE_NORMAL,
	.mode = MAG_CONTINUOUS,
	.hs = HMC5883L_HIGHSPEED_DISABLED
};
```
Then apply it:
```
hmc5883l_config(hmc5883l, cfg);
```

In order to read data, you just have to write:
```
mag_field_t mag;
hmc5883l_get_mag_field(hmc5883l, &mag);
```

Now the values read by the sensor are stored inside the `mag` variable.

## Next versions updates & fixes

- Single mode
- Status Register usage