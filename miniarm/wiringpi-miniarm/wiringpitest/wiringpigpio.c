/*
 * blink.c:
 *	Standard "blink" program in wiringPi. Blinks an LED connected
 *	to the first GPIO pin.
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include "wiringPi.h"

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LED	188
#define INGPIO  33
int main111 (void)
{
	int a,i;
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetupPhys () ;
for(i=1;i<=40;i++)
{
	pinMode(i,OUTPUT);
}
//  pinMode (LED, OUTPUT) ;
 // pinMode (9, INPUT) ;
  for (;;)
  {
for(i=1;i<=40;i++)
{
    digitalWrite (i, HIGH) ;	// On 
  //  a=digitalRead(9);
    printf("a1=%d\n",a);	
    delay (50) ;	// mS
    digitalWrite (i, LOW) ;	// Off
//    a=digitalRead(9);
  //  printf("a0=%d\n",a);
    delay (50) ;
 }
 }
  return 0 ;
}

int mai111n(void)
{
	int a;
	wiringPiSetupPhys () ;
	pinMode (LED, OUTPUT) ;
	pinMode (INGPIO,INPUT);
	for(;;)
	{
		digitalWrite (LED, HIGH) ;
		delay(500);
   		a=digitalRead(INGPIO);
   		printf("a0=%d\n",a);
		digitalWrite (LED, LOW) ;
    		a=digitalRead(INGPIO);
    		printf("a0=%d\n",a);
		delay(500);
	
	}
}


int main(void)
{
	wiringPiSetupGpio();
	pinMode(LED,OUTPUT);
	for(;;)
	{
		
		digitalWrite (LED, HIGH) ;
		delay(500);
		digitalWrite (LED, LOW) ;
		delay(500);
	}

}









