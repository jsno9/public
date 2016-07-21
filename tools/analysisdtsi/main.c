#include<stdio.h>
#include<string.h>

int analysis(unsigned char *dstname,unsigned char *filename,unsigned int layer)
{
	FILE* filesrc;
	unsigned int filelen;
	unsigned char filebuf[0x100000];
	unsigned char name[100];
	unsigned char wholename[100];
	unsigned int i,j,locallayer;
	int backflag;
	sprintf(wholename,"%s%s",dstname,filename);	
	filesrc=fopen(wholename,"rb");
	if(filesrc==NULL)
	{
		return 2;
	}
	filelen=fread(filebuf,1,0x100000,filesrc);
	printf("\n");
	i=0;
	locallayer=layer+1;
	while(1)
	{
		if(filebuf[i]=='#')
		{
			if(*(unsigned long long *)(filebuf+i)==0x6564756c636e6923) //#include
			{
				i=i+8;
				while((filebuf[i]==0x20)||(filebuf[i]==0x8))
				{
					i++;
					if(i==filelen)
					{
						break;
					}
				}
				if(i==filelen)
				{
					break;
				}
				if(filebuf[i]=='<'||filebuf[i]=='"')
				{
					j=0;i++;
					while(filebuf[i]!='>'&&filebuf[i]!='"')
					{
						name[j]=filebuf[i];
						i++;j++;
					}
					name[j]='\0';
					switch (locallayer)
					{
						case 5:printf("\t");
						case 4:printf("\t");
						case 3:printf("\t");
						case 2:printf("\t");
						case 1:printf("\t");
					
					}
					printf("%s",name);
					if(name[j-1]!='h')
					{	
						backflag=analysis(dstname,name,locallayer);
						if(backflag==2)
						{
							printf("(can not open this file)\n");
						}
					}
					else
					{
						printf("\n");
					}
				}
			

			}
		}
		i++;
		if(i==filelen)
		{
			return 1;
		}
	}

}


void main(int argc,char** argv)
{
	FILE* src;
	unsigned char name[100];
	unsigned char dstname[100];	
	unsigned int i,j,flag;
	if(argc<2)
	{
		printf("analysis xxx.dts\n");
		return;
	}
	strcpy(dstname,argv[1]);
	i=0;flag=0;
	while(dstname[i]!='\0')
	{
		if(dstname[i]=='/')
		{
			flag++;
		}
		i++;
	}
	i=0;j=0;
	while(flag!=0)
	{
		if(dstname[i]=='/')
		{
			flag--;
		}
		i++;
	}
	name[j]=dstname[i];
	dstname[i]='\0';
	i++;j++;
	while(dstname[i]!='\0')
	{
		name[j]=dstname[i];
		i++;j++;
	}
	name[j]='\0';
	printf("%s",name);
	analysis(dstname,name,0);

}
