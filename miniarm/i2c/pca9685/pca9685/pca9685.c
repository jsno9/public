#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>

#include "i2c.h"

#define I2C_FILE_NAME "/dev/i2c-1"
#define PCA9685ADDR 0x40
#define PCA9685_MODE1 0x00

#define LED0_ON_L 0x6
#define LEDALL_ON_L 0xFA

#define PIN_ALL 16
#define PCA9685_PRESCALE 0xfe

void pca9685PWMFreq(int fd, float freq)
{
	unsigned char val ;
	int prescale,sleep,wake,settings,restart;
	// Cap at min and max
	freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));
	
	
	get_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,&val);
	prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

	// Get settings and calc bytes for the different states.
	settings = val & 0x7F;	// Set restart bit to 0
	sleep	= settings | 0x10;									// Set sleep bit to 1
	wake 	= settings & 0xEF;									// Set sleep bit to 0
	restart = wake | 0x80;										// Set restart bit to 1

	// Go to sleep, set prescale and wake up again.
	//printf("%d\n",prescale);
	set_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,sleep);
	set_i2c_register(fd,PCA9685ADDR,PCA9685_PRESCALE,prescale);
	set_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,wake);

	// Now wait a millisecond until oscillator finished stabilizing and restart PWM.
	usleep(10);
	set_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,restart);
	
}

void pca9685_init(int fd,float freq)
{
	unsigned char val;
	get_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,&val);
	//printf("PCA9685_MODE1");
	val &= 0x7f;
	val |= 0x20;
	set_i2c_register(fd,PCA9685ADDR,PCA9685_MODE1,val);
	pca9685PWMFreq(fd,freq);	
}

int baseReg(int pin)
{
	return (pin >= PIN_ALL ? LEDALL_ON_L : LED0_ON_L + 4 * pin);
}

void pca9685PWMWrite(int fd, int pin, int on, int off)
{
	int reg = baseReg(pin);

	unsigned char bufon[2]={0};
	unsigned char bufoff[2]={0};
	unsigned char buf[4];
	bufon[0] = on & 0xff;
	bufon[1]=(on>>8) & 0xff;
	bufoff[0] = off & 0xff;
	bufoff[1]=(off>>8) & 0xff;
	
	buf[0]=bufon[0];
	buf[1]=bufon[1];
	buf[2]=bufoff[0];
	buf[3]=bufoff[1];
	set_i2c_buf(fd,PCA9685ADDR,reg,4,buf);

}

void main()
{
	int i2c_file;
	int a,b;
	if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) {
        	perror("Unable to open i2c control file");
        	exit(1);
    	}

	pca9685_init(i2c_file,320);
	pca9685PWMWrite(i2c_file,0,100,1000);
	
	while(1)
	{
		while(scanf("%d %d",&a,&b)!=0)
		{
			pca9685PWMWrite(i2c_file,0,a,b);
		}
	}

}


