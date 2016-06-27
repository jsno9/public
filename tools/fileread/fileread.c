#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "strtoi.h"

void fileread(int fd,int seek,unsigned char *buf,int len)
{
	int ret,i;
	lseek(fd,seek,SEEK_SET);
	ret=read(fd,buf,len);
	if(ret<0)
	{
		printf("read failed\n");
		exit(-1);	
	}
	else if(ret!=len)
	{
		printf("readback=%d,len=%d\n",ret,len);
		exit(-1);
	}
	else
	{
		for(i=0;i<len;i++)
		{	
			if(i%16==0)
			{
				printf("\n");
				printf("%08x   ",seek+i);
			}			
			if(i%4==0)
				printf(" ");
			printf("%02x ",buf[i]);
			
		}
		printf("\n");
	}
}

void useage()
{
	fprintf(stderr,
		"  -f file name\n"
		"  -s lseek (0~4096)\n"
		"  -l length (0~4096)\n"
		"  -h help\n"
	);
}

int main(int argc,char *argv[])
{
	int fd;
	int seek=0;
	unsigned char buf[4096];
	int len=1;
	int opt=0;
	char *name=NULL;
	opterr=0;
	if(argc==1)
	{
		useage();
        	return 2;
	} 

	while((opt=getopt(argc,argv,"f:s:l:h"))!=-1)
	{
		switch(opt)
		{
			case 'f':
				name=optarg;
				printf("file=%s\n",name);
				break;
			case 's':
				if((optarg[1]=='x')||(optarg[1]=='X'))
				{
					seek=strhextoi(optarg);
				}
				else
				{
					seek=strtoi(optarg);
				}
				printf("seek=%d\n",seek);
				if(seek>4096)
					return 2;
				break;
			case 'l':if((optarg[1]=='x')||(optarg[1]=='X'))
				{
					len=strhextoi(optarg);
				}
				else
				{
					len=strtoi(optarg);
				}
				printf("len=%d\n",len);
				if(len>4096)
				return 2;
				break;
			case 'h':
				useage();
				return 2;
				break;
		}
	
	}

	printf("ok\n");
	fd=open(name,O_RDONLY);
	if(fd<0)
	{
		printf("open failed\n");
		exit(-1);
	}
	fileread(fd,seek,buf,len);

}
