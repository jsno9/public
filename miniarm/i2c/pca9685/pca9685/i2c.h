#ifndef I2C_H
#define I2C_H

int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value);

int get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val);

int get_i2c_buf(int file,
                            unsigned char addr,
                            unsigned char reg,
			    unsigned char len,
                            unsigned char *buf);

int set_i2c_buf(int file,
                            unsigned char addr,
                            unsigned char reg,
			    unsigned char len,
                            unsigned char *buf);
#endif
