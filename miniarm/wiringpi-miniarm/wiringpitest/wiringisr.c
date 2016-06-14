#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>


void myInterrupt0 (void) 
{  
	static int a=0;
	printf("irq a = %d\n",a++);
}

int main (void)
{
  int gotOne, pin ;


 

  wiringPiSetupGpio () ;
  pinMode (8, OUTPUT) ;
  wiringPiISR (9, INT_EDGE_FALLING, &myInterrupt0) ;


  for (;;)
  {
    digitalWrite (8, HIGH) ;	// On
    delay (5000) ;		// mS
    digitalWrite (8, LOW) ;	// Off
    delay (5000) ;
  }

  return 0 ;
}


