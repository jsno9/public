#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>


#define I2C_FILE_NAME "/dev/i2c-1"
#define MPU6050ADDR	0x68
#define PWR_MGMT_1	0x6b
#define SMPLRT_DIV	0x19
#define CONFIG		0x1a
#define ACCEL_CONFIG	0x1c
#define GYRO_CONFIG	0x1b

#define ACCEL_XOUT_H	0x3b
#define ACCEL_XOUT_L	0x3c
#define ACCEL_YOUT_H	0x3d
#define ACCEL_YOUT_L	0x3e
#define ACCEL_ZOUT_H	0x3f
#define	ACCEL_ZOUT_L	0x40

#define GYRO_XOUT_H	0x43
#define GYRO_XOUT_L	0x44
#define GYRO_YOUT_H	0x45
#define GYRO_YOUT_L	0x46
#define GYRO_ZOUT_H	0x47
#define GYRO_ZOUT_L	0x48


#define Kp 2.0f
#define Ki 0.002f
#define halfT 0.001f
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float exInt = 0, eyInt = 0, ezInt = 0;
float Pitch;
float Roll;
float Yaw;

//input
	float kalmangain[6];
	float H;
	float R;

//output
	float predictdata[6];
	float predictp[6];
	float A;
	float Q;

static int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value) {

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    /* The first byte indicates which register we'll write */
    outbuf[0] = reg;

    /* 
     * The second byte indicates the value to write.  Note that for many
     * devices, we can write multiple, sequential registers at once by
     * simply making outbuf bigger.
     */
    outbuf[1] = value;

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}


static int get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    *val = inbuf;

    return 0;
}

static int get_i2c_buf(int file,
                            unsigned char addr,
                            unsigned char reg,
			    unsigned char len,
                            unsigned char *buf) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = len;
    messages[1].buf   = buf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    

    return 0;
}


void mpu6050_init(int fd)
{
	sleep(1);
	set_i2c_register(fd,MPU6050ADDR,PWR_MGMT_1,0x00);
	//msleep(10);
	set_i2c_register(fd,MPU6050ADDR,SMPLRT_DIV,0x07);
	//set_i2c_register(fd,MPU6050ADDR,SMPLRT_DIV,0x04);
	//msleep(10);
	set_i2c_register(fd,MPU6050ADDR,CONFIG,0x06);
	//set_i2c_register(fd,MPU6050ADDR,CONFIG,0x0b);
	//msleep(10);
	set_i2c_register(fd,MPU6050ADDR,ACCEL_CONFIG,0x00);
	//msleep(10);
	set_i2c_register(fd,MPU6050ADDR,GYRO_CONFIG,0x18);
	//msleep(10);
	
}

void getmotion6(int i2c_file, short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
{
	static unsigned char buf[14]={0};	
	
	get_i2c_buf(i2c_file,0x68,0x3b,14,buf);
	
	*ax = buf[0];
	*ax = (*ax<<8)+buf[1];	
	*ay = buf[2];
	*ay = (*ay<<8)+buf[3];
	*az = buf[4];
	*az = (*az<<8)+buf[5];

	*gx = buf[8];
	*gx = (*gx<<8)+buf[9];
	*gy = buf[10];
	*gy = (*gy<<8)+buf[11];
	*gz = buf[12];
	*gz = (*gz<<8)+buf[13];

}

int initkalman()
{
	predictdata[0] = predictdata[1] = predictdata[2] = predictdata[3] = predictdata[4] =predictdata[5] = 42;
	predictp[0] = predictp[1] = predictp[2] = predictp[3] = predictp[4] = predictp[5] = 1;
	A = 1;			//testonly=1
	H = 1;
	Q = 1;		//10e-3;	/* predict noise convariance */
	R = 50;	//10e-2;	/* measure error convariance */
}

void kalman(float gx, float gy, float gz, float ax, float ay, float az)
{
	predictdata[0] = A * predictdata[0];
	predictdata[1] = A * predictdata[1];
	predictdata[2] = A * predictdata[2];
	predictdata[3] = A * predictdata[3];
	predictdata[4] = A * predictdata[4];
	predictdata[5] = A * predictdata[5];

	predictp[0] = A*A*predictp[0] + Q;
	predictp[1] = A*A*predictp[1] + Q;
	predictp[2] = A*A*predictp[2] + Q;
	predictp[3] = A*A*predictp[3] + Q;
	predictp[4] = A*A*predictp[4] + Q;
	predictp[5] = A*A*predictp[5] + Q;




	/* Measurement */
	kalmangain[0] = H*predictp[0] / (H*H*predictp[0] + R);
	kalmangain[1] = H*predictp[1] / (H*H*predictp[1] + R);
	kalmangain[2] = H*predictp[2] / (H*H*predictp[2] + R);
	kalmangain[3] = H*predictp[3] / (H*H*predictp[3] + R);
	kalmangain[4] = H*predictp[4] / (H*H*predictp[4] + R);
	kalmangain[5] = H*predictp[5] / (H*H*predictp[5] + R);

	predictdata[0] = predictdata[0]
			+ kalmangain[0] * ((float)ax - H * predictdata[0]);
	predictdata[1] = predictdata[1]
			+ kalmangain[1] * ((float)ay - H * predictdata[1]);
	predictdata[2] = predictdata[2]
			+ kalmangain[2] * ((float)az - H * predictdata[2]);
	predictdata[3] = predictdata[3]
			+ kalmangain[3] * ((float)gx - H * predictdata[3]);
	predictdata[4] = predictdata[4]
			+ kalmangain[4] * ((float)gy - H * predictdata[4]);
	predictdata[5] = predictdata[5]
			+ kalmangain[5] * ((float)gz - H * predictdata[5]);

	predictp[0] = (1 - H*kalmangain[0]) * predictp[0];
	predictp[1] = (1 - H*kalmangain[1]) * predictp[1];
	predictp[2] = (1 - H*kalmangain[2]) * predictp[2];
	predictp[3] = (1 - H*kalmangain[3]) * predictp[3];
	predictp[4] = (1 - H*kalmangain[4]) * predictp[4];
	predictp[5] = (1 - H*kalmangain[5]) * predictp[5];

}


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	//static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;

	ax = ax*9.8/16384;
	ay = ay*9.8/16384;
	az = az*9.8/16384;
	gx = gx*3.14159/16384;
	gy = gy*3.14159/16384;
	gz = gz*3.14159/16384;
	norm = sqrt(ax*ax+ay*ay+az*az);
	ax = ax/norm;
	ay = ay/norm;
	az = az/norm;
	
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	
	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);

	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;	

	Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)*57.3; // pitch
 //       Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv
	Yaw = atan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3))*57.3;
	Roll = atan2(2*(q0*q1+q2*q3),1-2*(q1*q1+q2*q2))*57.3;

}

void main()
{
	int i2c_file;
	int i;
	unsigned char val;
	short accel[3]={0};
	short gyro[3]={0};
	if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) {
        	perror("Unable to open i2c control file");
        	exit(1);
    	}

	mpu6050_init(i2c_file);
	initkalman();
	while(1)
	{
		getmotion6(i2c_file,&accel[0],&accel[1],&accel[2],&gyro[0],&gyro[1],&gyro[2]);
		kalman((float)gyro[0],(float)gyro[1],(float)gyro[2],(float)accel[0],(float)accel[1],(float)accel[2]);		
		IMUupdate((float)predictdata[3],(float)predictdata[4],(float)predictdata[5],(float)predictdata[0],(float)predictdata[1],(float)predictdata[2]);
		//printf("accel = %d ,%d ,%d ,gyro = %d ,%d ,%d\n",accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2]);
		//printf("q0 = %f,q1 = %f,q2 = %f,q3 = %f\n",q0,q1,q2,q3);
		printf("Pitch = %f,Roll = %f,Yaw = %f\n",Pitch,Roll,Yaw);	
	}
	for(i=0;i<=0x75;i++)
	{
		get_i2c_register(i2c_file,0x68,i,&val);
		printf("addr %x= %x\n",i,val);
		
	}
	
	
	
}



