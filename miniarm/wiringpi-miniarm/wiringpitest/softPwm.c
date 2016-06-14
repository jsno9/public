/*
 * softPwm.c:
 *	Test of the software PWM driver. Needs 8 LEDs connected
 *	to the Pi - e.g. Ladder board.
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
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <softPwm.h>

#define RANGE		100
#define	NUM_LEDS	  29
#define motor2	31
#define motor3  33
#define motor4  35



int values [8] = { 0, 25, 50, 75, 100, 75, 50, 25 } ;

int main ()
{
  int i, j ;
  char buf [80] ;

 wiringPiSetupPhys () ;
//wiringPiSetupGpio();

     softPwmCreate (NUM_LEDS, 50, RANGE) ;
     softPwmCreate (motor2,0 , RANGE) ;
     softPwmCreate (motor3,50 , RANGE) ;
     softPwmCreate (motor4,0 , RANGE) ;
while(1);




// Bring all up one by one:
/*
  for (;;)
{
    for (j = 0 ; j <= 100 ; ++j)
    {
      softPwmWrite (NUM_LEDS, j) ;
      delay (10) ;
    }



// All Down

  for (i = 100 ; i > 0 ; --i)
  {
    
      softPwmWrite (NUM_LEDS, i) ;
	delay (10) ;
  
  }
}
*/
}
