//
//  cpu_benchmark
//
//  Created by Boyang Li on 9/10/14.
//

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

const int LOOPS = 1e8;

void * iops(void *);
void * flops(void *);
int do_Iops(int);
int do_Flops(int);

int main(int argc, char const * argv[])
{
    if(argc != 3)
	{
		printf("usage: %s <operation type> <num of threads>\n", argv[0]);
        return -1;
	}
    
    // argv[0] cpu_benchmark
    // argv[1] flops / iops
    // 1 thread / 2 threads / 4 threads / 8 threads
    int thread_n = atoi(argv[2]);
    
    if (strcmp(argv[1], "flops") == 0) {
        do_Flops(thread_n);
    }
    else if (strcmp(argv[1], "iops") == 0){
        do_Iops(thread_n);
    }
    else{
        printf("Error operation type\n");
        return -1;
    }
    
    return 0;
}

int do_Flops(int thread_n)
{
    int i = 0;
    struct timeval startTime, endTime;
    // total operations
    unsigned long operations = LOOPS * thread_n;
    pthread_t threads[thread_n];
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, iops, NULL);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double flops = ((double)operations / (exec_time / 1000)) / 1e9; //GFLOPS
    printf("With %d threads, the execution time is %10f ms and the GFlOPS is %10f\n", thread_n, exec_time, flops);
    
    return 0;
}

int do_Iops(int thread_n)
{
    int i = 0;
    struct timeval startTime, endTime;
    // total operations
    unsigned long operations = 2ul * LOOPS * thread_n;
    pthread_t threads[thread_n];
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, iops, NULL);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double iops = ((double)operations / (exec_time / 1000)) / 1e9; //IOPS
    printf("With %d threads, the execution time is %10f ms and the GIOPS is %10f\n", thread_n, exec_time, iops);
    
    return 0;
}

void * flops(void * arg)
{
    int i = 0;
    float sum = 0;
    
    for (i = 0; i < LOOPS; i++) {
        // float addition
        sum += (float)i;
    }
    
    return NULL;
}

void * iops(void * arg)
{
    int i;
    int sum = 0;
    
    for (i = 0; i < LOOPS; i++) {
        // integer addition
        sum += i;
    }
    
    return NULL;
}