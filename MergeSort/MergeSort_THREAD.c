#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int arr[100001],n;

typedef struct node {
    int left;
    int right;
} NODE;

void swap(int *xp, int *yp)
{
	int temp;
    temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void merge(int s, int mid, int e)
{
	int ar[100001],i;
	int p=s,q=mid+1,r=0;
	while(p<=mid || q<=e)
	{
		if(p>mid)
		{
			ar[r] = arr[q];
			q+=1;
		}
		else if(q>e)
		{
			ar[r] = arr[p];
			p+=1;
		}
		else if ( arr[p]<arr[q])
		{
			ar[r] = arr[p];
			p+=1;
		}
		else if ( arr[p]>=arr[q])
		{
			ar[r] = arr[q];
			q+=1;
		}
		r+=1;
	}
	for(i=s;i<=e;i++)
		arr[i]=ar[i-s];
	return;
}

void * mergeSort(void *arg)
{
    NODE *p = (NODE *)arg;
    int l=p->left;
    int r=p->right;
	
	if(l>=r)
	{
		return NULL;
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
        return NULL;
    }
    else if(l<r)
    {
        pthread_t tid1, tid2;
        int ret;

	    NODE n1;
        n1.left = l;
        n1.right = mid;

        NODE n2;
        n2.left = mid+1;
        n2.right = r;

        ret = pthread_create(&tid1, NULL, mergeSort, &n1);
        if (ret) 
        {
                printf("Error in creation of thread\n");    
                exit(1);
        }
        ret = pthread_create(&tid2, NULL, mergeSort, &n2);
        if (ret) 
        {
                printf("Error in creation of thread\n");
                exit(1);
        }

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        merge(l,mid,r);
        pthread_exit(NULL);
    }
}


int main()
{
        int i;
        scanf("%d",&n);
        for(i=0;i<n;i++)
        {
			scanf("%d",&arr[i]);
		}

        NODE m;
        m.left = 0;
        m.right = n-1;

        pthread_t tid;
        int ret; 
        ret=pthread_create(&tid, NULL, mergeSort, &m);
        if (ret) 
        {
                printf("Error in thread creation\n");
                exit(1);
        }
        pthread_join(tid, NULL);
        for(i=0;i<n;i++)
        {
            printf ("%d ",arr[i]);
        }
        printf ("\n");
        return 0;
}
