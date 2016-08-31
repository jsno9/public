#include <sys/timerfd.h>
#include <stdio.h>
#include <stdint.h> 
#include <unistd.h> 
#include <stdlib.h> 
void main()
{
	int fd,ret;
	struct timespec now;
	struct timespec curr;
	int secs,nsecs;
	unsigned int exp; 
	struct itimerspec new_value;
	
	clock_gettime(CLOCK_REALTIME, &now);
	new_value.it_value.tv_sec = now.tv_sec+5;
	new_value.it_value.tv_nsec=now.tv_nsec;
	new_value.it_interval.tv_sec = 2;  
    new_value.it_interval.tv_nsec = 0;  
	printf("start=%ld.%ld\n",now.tv_sec,now.tv_nsec);
	fd=timerfd_create(CLOCK_REALTIME,0);
	if (fd == -1)  
    {
		printf("timerfd_create error\n");
		exit(-1);
	} 
	if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) 
	{
		printf("timerfd_settime error\n");
		exit(-1);
	}
	while(1)
	{
		ssize_t s = read(fd, &exp, sizeof(uint64_t));  
      	if (s != sizeof(uint64_t))  
        {
			printf("read error\n");
			exit(-1);
		}
		clock_gettime(CLOCK_REALTIME, &curr);
		printf("%ld.%ld ",curr.tv_sec,curr.tv_nsec);
		

		printf("read=%u\n",exp);

	}	


}
