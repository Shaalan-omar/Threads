#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t m1;

unsigned long long int totalSum = 0;

struct Numbers{
    unsigned long long start;
    unsigned long long end;
};


void *print_message_function(void *ptr)
{
    //printf("th here\n");
    unsigned long long int tempSum = 0;

    struct Numbers *ptr_to_struc = (struct Numbers* ) ptr;
    //printf("\n\n\n\n %d\n%d",ptr_to_struc->start,ptr_to_struc->end);
    for(int i = ptr_to_struc->start; i<ptr_to_struc->end;i++)
    {
        tempSum+= i;
    }
    //printf("tempoo:%llu\n",tempSum);
    pthread_mutex_lock(&m1);
    totalSum+= tempSum;
    pthread_mutex_unlock(&m1);
}

unsigned long long int sum(unsigned long long N)
{
    unsigned long long int sumx=0;
    for (int i = 1; i <= N; i++)
    {
        sumx+=i;
    }
    
    return sumx;
}

unsigned long long parallel_sum_threads(int const n_thread, int N)
{
    int n_proc = n_thread/2;
    //int mod= N % n_proc;
    //long long int sumx=0;
 //   unsigned long long partial_sum=0 ;

    unsigned long long int total_sum_local = 0;

    struct Numbers *arr;

    //int k= log2 (n_proc);

    unsigned long long int n_psums=0;

    arr = (struct Numbers*)malloc(n_thread* sizeof(struct Numbers));

    int pid = getpid();

    unsigned long long  int offset = N/n_thread;

    int fd[n_proc][2];    

    pid_t pids[n_proc];

    //struct Numbers arr2[n][2];
    arr[0].start =1;

    arr[0].end = arr[0].start + offset;
    //printf("endoo%llu\n\n\n",arr[0].end);
    

    for(int i = 1; i<n_thread;i++)
    {
        arr[i].start = arr[i-1].end;

        arr[i].end = arr[i].start + offset;
        if(i==n_thread-1)
        arr[i].end = N;
        // arr2[i][0].start = arr[i].start;
        //     arr2[i][0].end = (arr[i].end + arr[i].start)/2;

        //     arr2[i][1].start =  (arr[i].end + arr[i].start)/2;
        //     arr2[i][1].end =  arr[i].end;

    }

    for ( int i = 0; i<n_proc; i++) 
    {
        if (pipe(fd[i]))
            exit(0);
    }

    for(int i = 0; i<n_proc;i++)
    {
        pids[i] = fork();

        if(pids[i] == 0) // I am child
        {
            //printf("hello\n");

            //partial_sum=0;    
            
            //for (unsigned long long  j = 0; j<2; j++) 
            //{
            totalSum = 0;
            //printf("start\n");
                pthread_t thread1, thread2;
                pthread_create(&thread1,NULL,print_message_function, (void*) (arr+(i*2)));
                pthread_create(&thread2,NULL,print_message_function, (void*) (arr+(i*2)+1));


                        // if(iter1 != 0)
                        //     exit(11);
                        // else if(iter2 != 0)
                        //     exit(12);
                pthread_join(thread1,NULL);
                pthread_join(thread2, NULL);
                //printf("done\n");
            //}
//                //printf("partial sum sent %d\n",partial_sum);
            close(fd[i][0]);
            write(fd[i][1], &totalSum, sizeof(totalSum));
//                //printf("process i:(%d) partial sum:(%d)\n",i,partial_sum);
            //free(arr);
            exit(0);        
        }


    }

    //wait(NULL);     
    while(wait(NULL) >= 0){}

//printf("hereeee\n");

    for (int i = 0; i<n_proc; i++) 
    {
        close(fd[i][1]);
        read(fd[i][0], &n_psums, sizeof(n_psums));
        //printf("noo\n");
        total_sum_local+=n_psums;           
    }

    //printf("total sum:%llu\n",total_sum_local);

//            printf("Total sum:%d\n",total_sum_local);
    for (int k = 0; k<n_proc; k++) 
    {
        close(fd[k][1]);
        close(fd[k][0]);
    }
    free(arr);
    return total_sum_local+N;

}


void test1()
{

unsigned long long int Parallel_sumthreads;
clock_t start2,end2;
double cpu_time_used2;
unsigned int N;

printf("parallel_sum_threads\n" );
for(unsigned int i=1;i<=1000000;i=i+10000)
{
	start2 = clock();
	Parallel_sumthreads=parallel_sum_threads(7,i);
	end2 = clock();
	cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
	printf("%d,%lld,%f\n",i,Parallel_sumthreads,cpu_time_used2);
	
}
}

void test2()
{
    const unsigned int N=1000000;

    unsigned long long int Parallel_sumthreads;
    clock_t start2,end2;
    double cpu_time_used2;
    
    printf("parallel_sum_threads\n" );
    for(int i = 1; i < 200; i+=2)
    {
	start2 = clock();
       Parallel_sumthreads=parallel_sum_threads(i,N);
	end2 = clock();
	cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
	printf("%d,%lld,%f\n",i,Parallel_sumthreads,cpu_time_used2);
	
    }
    
}


int main ()
{


        //printf("%llu",parallel_sum(6, 5));
    //test2();
    test1();  

 return 0;
}
