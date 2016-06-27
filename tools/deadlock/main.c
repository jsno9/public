#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  

void funthread1(char *msg);
void funthread2(char *msg);
int g_value = 1; 
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

int main(int argc,char *argv[])
{
	pthread_t thread1;
	pthread_t thread2;

	if(pthread_mutex_init(&mutex,NULL)!=0)
	{
		printf("Init metux failed\n");
		exit(1);
	}	

	if(pthread_mutex_init(&mutex2,NULL)!=0)
	{
		printf("Init metux2 failed\n");
		exit(1);
	}
	
	if(pthread_create(&thread1,NULL,(void *)funthread1,NULL)!=0)
	{
		printf("Init thread1 failed\n");
		exit(1);
	}

	if(pthread_create(&thread2,NULL,(void *)funthread2,NULL)!=0)
	{
		printf("Init thread2 failed\n");
		exit(1);
	}

	sleep(5);

	pthread_join(thread1,(void**)0);
        pthread_join(thread2,(void**)0);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	printf("This is main\n");

}

void funthread1(char *msg)
{
	int val;

	printf("funthread1\n");
	val=pthread_mutex_lock(&mutex);
	if(val!=0)
	{
		printf("lock1 mutex error\n");
	}
	sleep(1);
	val=pthread_mutex_lock(&mutex2);
	if(val!=0)
	{
		printf("lock1 mutex2 error\n");
	}
	g_value=0;
	printf("thread 1 lock,g_value=%d\n",g_value);
	g_value=1;
	printf("thread 1 lock,g_value=%d\n",g_value);
	sleep(2);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex2);
	printf("thread 1 unlock\n");
  
}

void funthread2(char *msg)
{
	int val;

	printf("funthread2\n");
	val=pthread_mutex_lock(&mutex2);
	if(val!=0)
	{
		printf("lock2 mutex2 error\n");
	}
	sleep(1);
	val=pthread_mutex_lock(&mutex);
	if(val!=0)
	{
		printf("lock2 mutex error\n");
	}

	g_value=2;
	printf("thread 2 lock,g_value=%d\n",g_value);
	g_value=3;
	printf("thread 2 lock,g_value=%d\n",g_value);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex2);
	printf("thread 2 unlock\n");

 

}



