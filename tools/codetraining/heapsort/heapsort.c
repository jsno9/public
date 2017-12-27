#include <stdio.h>
#include <stdlib.h>
#define LENGTH(array) ( (sizeof(array)) / (sizeof(array[0])) )
void heapadjust(int *a,int start,int end)
{
    int c=start;
    int temp;
    int lchild=2*c+1;
    int rchild=2*c+2;
    
    while(lchild<=end)
    {
        
        if(rchild<=end)
        {
            if((a[lchild]>a[c]) || (a[rchild]>a[c]))
            {
                if(a[lchild]>a[rchild])
                {
                    temp=a[c];
                    a[c]=a[lchild];
                    a[lchild]=temp;
                }
                else
                {
                    temp=a[c];
                    a[c]=a[rchild];
                    a[rchild]=temp;
                }
            }
        }
        else
        {
            if(a[lchild]>a[c])
            {
                temp=a[c];
                a[c]=a[lchild];
                a[lchild]=temp;
            }
        }
        c++;
        rchild=2*c+2;
        lchild=2*c+1;
        
    }
}

void heap_sort(int *a,int length)
{
    int i,temp;
    int n=length;
    for(i=n/2-1;i>=0;i--)
    {
        heapadjust(a,i,n-1);
    }
    
    for(i=n-1;i>0;i--)
    {
        temp=a[0];
        a[0]=a[i];
        a[i]=temp;
        heapadjust(a,0,i-1);
    }
}

int main()
{
    int i;
     int a[] = {20,30,90,40,70,110,60,10,100,50,80};
    int ilen = LENGTH(a);
 
    printf("before sort:");
     for (i=0; i<ilen; i++)
         printf("%d ", a[i]);
    printf("\n");

    //heap_sort_asc(a, ilen);            // 升序排列
    heap_sort(a, ilen);        // 降序排列

    printf("after  sort:");
    for (i=0; i<ilen; i++)
        printf("%d ", a[i]);
     printf("\n");
    return 0;
}
