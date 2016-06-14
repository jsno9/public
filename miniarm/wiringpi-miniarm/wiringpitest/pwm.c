#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void)
{
  int i ;

  

//  wiringPiSetupPhys () ;
 // pinMode (32, PWM_OUTPUT) ;
wiringPiSetupGpio();
  pinMode (239, PWM_OUTPUT) ;

 		 asus_pwm_start(3,0,1024,512);
while(1)
{
	
// 		 asus_pwm_start(3,0,1024,1);
}

  while(1)
  {

	for(i=0;i<1024;i++)
	{
 		 asus_pwm_start(3,0,1024,i);
		delay (1) ;
	}
	for(i=1024;i>0;i--)
	{
 		 asus_pwm_start(3,0,1024,i);
		delay (1) ;
	}
  }
  return 0 ;
}
