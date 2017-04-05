#include <stdio.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct alias_sensor_map
{
	char alias_sensor_name[64];
	char sensor_name[64];
};

struct alias_sensor_map alias_sensor[30];

int readtemp(int num)
{
	int i;
	int tempfp;
	char nodebuf[100];
	char typebuf[100];
	char tempbuf[100];
	char buf[100];
	int ret;
	int nodetemp;
	for(i=0;;i++)
	{
		snprintf(nodebuf,sizeof(nodebuf),"/sys/class/thermal/thermal_zone%d/type",i);
		printf("%s\n",nodebuf);
		tempfp=open(nodebuf,O_RDONLY);
		if(tempfp<0)
		{
			//printf("i=%d\n",i);
			return 0;
		}
		ret=read(tempfp,typebuf,sizeof(typebuf));

		//printf("%s",typebuf);
		strcpy(buf,"acpitz\n");	
		//printf("%s",buf);
		close(tempfp);	

		if(!strcmp(buf,typebuf))
		{
			snprintf(nodebuf,sizeof(nodebuf),"/sys/class/thermal/thermal_zone%d/temp",i);
			tempfp=open(nodebuf,O_RDONLY);
			ret=read(tempfp,tempbuf,sizeof(tempbuf));
			//printf("%s\n",tempbuf);
			nodetemp=atoi(tempbuf);
			//printf("int=%d\n",nodetemp);
			close(tempfp);
			//fflush(stdin);	
			return nodetemp;
		}
		close(tempfp);	
		
	}
	return 1;
	
}

int readline(int fd,void* buff)  
{  
        int rl=-1;  
        char c;  
        //long maxlength=128;  
        long count=0;  
		long wcount=0;
		char (*buf)[30]=buff;
        if(fd<0)  
        {  
            printf("open file error\n");  
            exit(0);  
        }  
		//printf("readline\n");
        while((rl=read(fd,&c,1))>0)  
        {  
            buf[count][wcount++]=c;
            if(c=='\n')  
            {  
				buf[count][wcount]='\0';
				count++;
				wcount=0;
				continue;
                  
            }  
  
        }  
         
        return count;  
} 

int main(int argc ,char **argv)
{
	int i;
	int fp;
	int count;
	char buff[100][30];
	char rbuf[100];
	int temp;
	fp=open("thermalconfig.txt",O_RDONLY);
	if(fp<0)
	{
		exit(1);
	}
	
	count=readline(fp,buff);
	//printf("%d\n",count);
	for(i=0;i<(count-1);i++)
	{
		if(i%2==0)
		{
			strcpy(alias_sensor[i/2].alias_sensor_name,buff[i]);
		}
		else
		{
			strcpy(alias_sensor[i/2].sensor_name,buff[i]);
		}
	}
	
	//for(i=0;i<((count-1)/2);i++)
	//{
	//	printf("%s",alias_sensor[i].alias_sensor_name);
	//	printf("%s",alias_sensor[i].sensor_name);
	//}	

	strcpy(rbuf,argv[1]);
	strcat(rbuf,"\n");
	for(i=0;i<((count-1)/2);i++)
	{
		if(!strcmp(rbuf,alias_sensor[i].alias_sensor_name))
		{
			temp=readtemp(i);
			return temp;
		}
	}

	//if(!strcmp("/data/data/Thermal0\n",alias_sensor[0].alias_sensor_name))
	//{
		//printf("haha\n");
	//}


	return 0;
}

