#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <softPwm.h>

#define RANGE 100
#define MOTOR+  29
#define MOTOR-  31
#define MOTOL+  33
#define MOTOL-  35
#define MAXSPEED RANGE

int main(void)
{

  	wiringPiSetupPhys () ;
  	softPwmCreate (MOTOR+, MAXSPEED, RANGE);
	softPwmCreate (MOTOR-, 0, RANGE);
	softPwmCreate (MOTOL+, MAXSPEED, RANGE);
	softPwmCreate (MOTOL-, 0, RANGE);

	softPwmWrite (MOTOR+, 0) ;
	softPwmWrite (MOTOR-, 0) ;
	softPwmWrite (MOTOL+, 0) ;
	softPwmWrite (MOTOL-, 0) ;

}
