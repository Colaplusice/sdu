//
//  main.c
//  P_demo
//
//  Created by 樊佳亮 on 2017/11/17.
//  Copyright © 2017年 樊佳亮. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include  <errno.h>
#include  <unistd.h>
pthread_barrier_t b;
int **max;
int *M2;
int *M3;
int S;
int m[20];
int count;
int printrow;
    int sd[26][26] = {{ 4,-2, 0,-2,-1,-2, 0,-2,-1, 0,-1,-1,-1,-2, 0,-1,-1,-1, 1, 0, 0, 0,-3, 0,-2,-1 },
        /* B */{ -2, 4,-3, 4, 1,-3,-1, 0,-3, 0, 0,-4,-3, 3, 0,-2, 0,-1, 0,-1, 0,-3,-4,-1,-3, 1 },
        /* C */{ 0,-3, 9,-3,-4,-2,-3,-3,-1, 0,-3,-1,-1,-3, 0,-3,-3,-3,-1,-1, 0,-1,-2,-2,-2,-3 },
        /* D */{ -2, 4,-3, 6, 2,-3,-1,-1,-3, 0,-1,-4,-3, 1, 0,-1, 0,-2, 0,-1, 0,-3,-4,-1,-3, 1 },
        /* E */{ -1, 1,-4, 2, 5,-3,-2, 0,-3, 0, 1,-3,-2, 0, 0,-1, 2, 0, 0,-1, 0,-2,-3,-1,-2, 4 },
        /* F */{ -2,-3,-2,-3,-3, 6,-3,-1, 0, 0,-3, 0, 0,-3, 0,-4,-3,-3,-2,-2, 0,-1, 1,-1, 3,-3 },
        /* G */{ 0,-1,-3,-1,-2,-3, 6,-2,-4, 0,-2,-4,-3, 0, 0,-2,-2,-2, 0,-2, 0,-3,-2,-1,-3,-2 },
        /* H */{ -2, 0,-3,-1, 0,-1,-2, 8,-3, 0,-1,-3,-2, 1, 0,-2, 0, 0,-1,-2, 0,-3,-2,-1, 2, 0 },
        /* I */{ -1,-3,-1,-3,-3, 0,-4,-3, 4, 0,-3, 2, 1,-3, 0,-3,-3,-3,-2,-1, 0, 3,-3,-1,-1,-3 },
        /* J */{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* K */{ -1, 0,-3,-1, 1,-3,-2,-1,-3, 0, 5,-2,-1, 0, 0,-1, 1, 2, 0,-1, 0,-2,-3,-1,-2, 1 },
        /* L */{ -1,-4,-1,-4,-3, 0,-4,-3, 2, 0,-2, 4, 2,-3, 0,-3,-2,-2,-2,-1, 0, 1,-2,-1,-1,-3 },
        /* M */{ -1,-3,-1,-3,-2, 0,-3,-2, 1, 0,-1, 2, 5,-2, 0,-2, 0,-1,-1,-1, 0, 1,-1,-1,-1,-1 },
        /* N */{ -2, 3,-3, 1, 0,-3, 0, 1,-3, 0, 0,-3,-2, 6, 0,-2, 0, 0, 1, 0, 0,-3,-4,-1,-2, 0 },
        /* O */{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* P */{ -1,-2,-3,-1,-1,-4,-2,-2,-3, 0,-1,-3,-2,-2, 0, 7,-1,-2,-1,-1, 0,-2,-4,-2,-3,-1 },
        /* Q */{ -1, 0,-3, 0, 2,-3,-2, 0,-3, 0, 1,-2, 0, 0, 0,-1, 5, 1, 0,-1, 0,-2,-2,-1,-1, 3 },
        /* R */{ -1,-1,-3,-2, 0,-3,-2, 0,-3, 0, 2,-2,-1, 0, 0,-2, 1, 5,-1,-1, 0,-3,-3,-1,-2, 0 },
        /* S */{ 1, 0,-1, 0, 0,-2, 0,-1,-2, 0, 0,-2,-1, 1, 0,-1, 0,-1, 4, 1, 0,-2,-3, 0,-2, 0 },
        /* T */{ 0,-1,-1,-1,-1,-2,-2,-2,-1, 0,-1,-1,-1, 0, 0,-1,-1,-1, 1, 5, 0, 0,-2, 0,-2,-1 },
        /* U */{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* V */{ 0,-3,-1,-3,-2,-1,-3,-3, 3, 0,-2, 1, 1,-3, 0,-2,-2,-3,-2, 0, 0, 4,-3,-1,-1,-2 },
        /* W */{ -3,-4,-2,-4,-3, 1,-2,-2,-3, 0,-3,-2,-1,-4, 0,-4,-2,-3,-3,-2, 0,-3,11,-2, 2,-3 },
        /* X */{ 0,-1,-2,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1, 0,-2,-1,-1, 0, 0, 0,-1,-2,-1,-1,-1 },
        /* Y */{ -2,-3,-2,-3,-2, 3,-3, 2,-1, 0,-2,-1,-1,-2, 0,-3,-1,-2,-2,-2, 0,-1, 2,-1, 7,-2 },
        /* Z */{ -1, 1,-3, 1, 4,-3,-2, 0,-3, 0, 1,-3,-1, 0, 0,-1, 3, 0, 0,-1, 0,-2,-3,-1,-2, 4 } };
int Max(int i, int j) {
    if (i ==0) {
        max[i][j] = j*-8;
        if (max[i][j]>M2[i])
        {
            M2[i]=max[i][j];
        }
        if (max[i][j]>M3[j])
        {
            M3[j]=max[i][j];
        }
        return 0;
    }
    if (j == 0) {
        max[i][j] = i*-8;
        if (max[i][j]>M2[i])
        {
            M2[i]=max[i][j];
        }
        if (max[i][j]>M3[j])
        {
            M3[j]=max[i][j];
        }
        return 0;
    }
    int ma1;
    int ma2 =M2[i]-8;
    int ma3 = M3[j]-8;
    ma1 = max[i-1][j-1] + sd[(i-1)%26][(j-1)%26];
    max[i][j]= (ma1>ma2 ? (ma1 >ma3 ? ma1 : ma3) : (ma3 >ma2 ? ma3 : ma2));
    if (max[i][j]>M2[i])
    {
        M2[i]=max[i][j];
    }
    if (max[i][j]>M3[j])
    {
	       M3[j]=max[i][j];
    }
    return 0;
}
void out(int i,int j) {
    printf("***********************************\n");
        for (int j2 =0; j2<j; ++j2)
        {
            printf("%d\t",max[i][j2] );
        }
}
void * hello(void *rank)
{
    int my_rank=(int)rank;
    int k=0;
    while(k<S){
   for(int i=my_rank;i<=k;i+=count)
    {  
       Max(i, k-i);
      // printf("i:%d j:%d\t",i,k-i );
      // printf("%di:\n",i);
    }
       pthread_barrier_wait(&b);
     //out(k,k);
     k++;    
    }
    while(k<S*2){
    	k++;
    	for (int i =my_rank+1; i <=S; i+=count)
    	{
    		if (i>=S||(k-i-1)>=S)
    		{
    		continue;
    		}
    	  Max(i,k-i-1);
    	
    	}
    	pthread_barrier_wait(&b);
    }

    return NULL;
}
int main(int argc,char *argv[])
{
    struct timeval start,end;
 gettimeofday(&start,NULL);
    

	count=atoi(argv[1]);
	int first=atoi(argv[2]);
	int second=atoi(argv[3]);
    S =first;
    printrow=second;
    max=(int**) malloc(S*sizeof(int));
    
    M2=malloc(S*sizeof(int));
    
    M3=(int*)malloc(S*sizeof(int));
    
    for (int i = 0; i <S; i++)
    {
        max[i]=(int*)malloc(S*sizeof(int));
    }

    pthread_t *all;
        all=malloc(count*sizeof(pthread_t));
         pthread_barrier_init(&b, 0, count);
    for (int i=0; i<count; i++) {
        pthread_create(&all[i], NULL,hello,(void *)i);
    }
       for (int i=0; i<count; i++) {
        pthread_join(all[i],NULL);
    }
            out(printrow,S);
       pthread_barrier_destroy(&b);
gettimeofday(&end,NULL);
 int timeuse=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
 printf("time:%d\n",timeuse);
return 0;
}

