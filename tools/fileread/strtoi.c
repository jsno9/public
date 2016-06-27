#include <stdio.h>

int strtoi(char *str)
{
	int i=0;
	int flag=0;
	int sum=0;
	while((str[i]!='-')&&((str[i]>'9')||(str[i]<'0')))
	{
		if((str[i]=='\0')||(str[i]=='\n'))
		{
			return 0;
		}
		i++;
	}
	if(str[i]=='-')
	{
		flag=1;
		i++;
	}
	while((str[i]<='9')&&(str[i]>='0'))
	{
		sum=sum*10+(str[i]-'0');
		i++;
	}
	if(flag==1)
	{
		sum=0-sum;
	}
	return sum;	

}

int strhextoi(char *str)
{
	int i=0;
	int flag=0;
	int sum=0;
	while((str[i]!='-')&&((str[i]>'9')||(str[i]<'0'))&&((str[i]>'z')||(str[i]<'a')))
	{
		if((str[i]=='\0')||(str[i]=='\n'))
		{
			return 0;
		}
		i++;
	}
	if(str[i]=='-')
	{
		flag=1;
		i++;
	}
	if((str[i]=='0')&&(str[i+1]=='x'))
	{
		//flag=1;
		i++;
		i++;
	}
	if((str[i]=='0')&&(str[i+1]=='X'))
	{
		//flag=1;
		i++;
		i++;
	}
	while(((str[i]<='9')&&(str[i]>='0'))||((str[i]<='f')&&(str[i]>='a'))||((str[i]<='F')&&(str[i]>='A')))
	{
		if((str[i]<='9')&&(str[i]>='0'))
		{
			sum=sum*16+(str[i]-'0');
		}
		if((str[i]<='f')&&(str[i]>='a'))
		{
			sum=sum*16+(str[i]-'a')+10;
		}
		if((str[i]<='F')&&(str[i]>='A'))
		{
			sum=sum*16+(str[i]-'A')+10;
		}
		i++;
	}
	if(flag==1)
	{
		sum=0-sum;
	}
	return sum;	

}

/*
void main(int argc,char* argv[])
{
	char a[200]="asd-1672135afertsd";
	int b;
	b = strtoi(argv[1]);
	printf("b = %d\n",b);
}
*/
