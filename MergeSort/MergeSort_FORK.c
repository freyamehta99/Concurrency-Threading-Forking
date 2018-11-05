#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int * arr= NULL;
key_t key = IPC_PRIVATE;
int temp[1000001];
void swap(int *xp, int *yp)
{
	int temp;
    temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void merge(int s, int mid, int e)
{
	int i;
	int p=s,q=mid+1,r=0;
	while(p<=mid || q<=e)
	{
		if(p>mid)
		{
			temp[r] = arr[q];
			q+=1;
		}
		else if(q>e)
		{
			temp[r] = arr[p];
			p+=1;
		}
		else if ( arr[p]<arr[q])
		{
			temp[r] = arr[p];
			p+=1;
		}
		else if ( arr[p]>=arr[q])
		{
			temp[r] = arr[q];
			q+=1;
		}
		r+=1;
	}
	for(i=s;i<=e;i++)
		arr[i]=temp[i-s];
	return;
}
 
void mergeSort(int l, int r)
{
	if(l>=r)
	{
		return;
	}
    int mid = (l+r)>>1;
    if(l-r<=5)
    {
        int i, j, min_idx;
 
        for (i = l; i < r; i++)
        {
            min_idx = i;
            for (j = i+1; j <= r; j++)
              if (arr[j] < arr[min_idx])
                min_idx = j;
    
            swap(&arr[min_idx], &arr[i]);
        }
        return;
    }
    else if (l < r)
    {
        pid_t fork1,fork2;
        fork1 = fork();
        if(fork1 < 0)
        {
            printf("Couldn't fork");
            return;
        }
        else if(fork1==0)
        {
            mergeSort(l, mid);
            exit(0);    
        }
        else
        {
            fork2 = fork();
            if(fork2 < 0)
            {
                printf("Couldn't fork");
                return; 
            }
            else if(fork2 == 0)
            {
                mergeSort(mid+1, r);
                exit(0);
            }
        }
        int s;
        waitpid(fork1, &s, 0);
        waitpid(fork2, &s, 0);
        merge(l, mid, r);
    }
}
 
int main()
{
   int shmid;
   int i,n;
    scanf("%d",&n);
	shmid = shmget(key, 4*n+4, IPC_CREAT| 0666);
	if(shmid == -1) {
		perror("Shmget failed");
		exit(1);
	}
   arr = shmat(shmid, 0, 0);
   for(i=0;i<n;i++)
    {
        scanf("%d",&arr[i]); 
    }

    mergeSort(0, n-1);
    
    for (i=0; i < n; i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}
