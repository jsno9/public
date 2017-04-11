#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libxl.h"

struct alias_sensor_map
{
	char alias_sensor_name[64];
	char sensor_name[64];
};

struct alias_sensor_map alias_sensor[30];

int main(int argc ,char **argv)
{
	int i,j,len;
	int x,y;
	int pathflag=0;
	FILE *fp;
	int thermalcount=0;
	char tempbuf[50];
	char thermalcountbuf[5];
    BookHandle book = xlCreateBook();
    if(book) 
    {
		printf("book ok\n");
        if(xlBookLoad(book, "thermal1.xls")) 
        {
			printf("xlBookLoad ok\n");
            SheetHandle sheet = xlBookGetSheet(book, 1);
            if(sheet)
            {
				for(i=0;i<40;i++)
				{
					for(j=0;j<5;j++)
					{
						const char* s = xlSheetReadStr(sheet, i, j, 0);
						if(s==NULL)
						{
							continue;
						}
	
						if(!strcmp(s,"Path"))
						{
							printf("row=%d,col=%d\n",i,j);
							x=i;y=j;
							pathflag = 1;
							break;
						}
					}
					if(pathflag==1)
						break;
				}



				for(j=0,i=(x+1);;i++,j++)
				{
					const char* s = xlSheetReadStr(sheet, i, y, 0);
					if(s==NULL)
					{
						printf("break\n");
						break;
					}
					strcpy(alias_sensor[j].alias_sensor_name,s);
					if(alias_sensor[j].alias_sensor_name[11]!='\0')
					{
						if(alias_sensor[j].alias_sensor_name[11]=='T'&&
							alias_sensor[j].alias_sensor_name[12]=='h'&&
							alias_sensor[j].alias_sensor_name[13]=='e'&&
							alias_sensor[j].alias_sensor_name[14]=='r'&&
							alias_sensor[j].alias_sensor_name[15]=='m'&&
							alias_sensor[j].alias_sensor_name[16]=='a'&&
							alias_sensor[j].alias_sensor_name[17]=='l')
						{
							thermalcount++;
						}
					}
								
					printf("%s\n",alias_sensor[j].alias_sensor_name);

					const char* s1 = xlSheetReadStr(sheet, i, y-2, 0);
					//printf("%s\n",s1);	
					if(s1==NULL)
					{
						printf("break1\n");
						break;
					}
					strcpy(alias_sensor[j].sensor_name,s1);
					
					printf("%s\n", alias_sensor[j].sensor_name);
					
				}

            }
        }     
       	len=j;
        xlBookRelease(book);
    }

	if(pathflag==0)
	{
		return 0;
	}
	printf("count=%d\n",thermalcount);

	fp=fopen("thermalconfig.txt","w+");
	if(fp == NULL)
	{
		exit(1);
	}

	for(i=0;i<len;i++)
	{
		fwrite(alias_sensor[i].alias_sensor_name,strlen(alias_sensor[i].alias_sensor_name),1,fp);
		fwrite("\n",1,1,fp);
		fwrite(alias_sensor[i].sensor_name,strlen(alias_sensor[i].sensor_name),1,fp);
		fwrite("\n",1,1,fp);
	}

	strcpy(tempbuf,"/data/data/ThermalCount");
	fwrite(tempbuf,strlen(tempbuf),1,fp);
	fwrite("\n",1,1,fp);

	sprintf(thermalcountbuf, "%d", thermalcount); 
	fwrite(thermalcountbuf,strlen(thermalcountbuf),1,fp);
	fwrite("\n",1,1,fp);

	fwrite("end",3,1,fp);
	fwrite("\n",1,1,fp);


    return 0;
}


