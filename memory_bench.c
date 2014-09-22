//
//  memory_bench.c
//  CS553
//
//  Created by 胡行坦 Xingtan Hu on 14-9-18.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

/********** memory sequential access **********/

void *sequential_access(void *n){
    int *m = (int*)n; // m is a point variable
    unsigned long size = 1024ul * 1024ul * 2100ul;
    int loop = 0; // initiate loop
    if (*m > 1024)
        loop = 2000;
    else
        loop = 1000000;
    char *des = (char *)malloc(size * sizeof(char)); // give size byte(char) to destination
    char *sor = (char *)malloc(loop * (*m) * sizeof(char)); // give size byte(char) to source
    int mem = 0, i = 0;
    for (i = 0; i < loop; i++) {
        memcpy(des + mem, sor + mem, *m);//copy (*m)byte from sor+mem to des+mem
        mem += (*m);
    }
    free(des); //free memory
    free(sor);
    return NULL;
}

/********** memory random access **********/

void *random_access(void *n){
    int *m = (int*)n;// m is a point variable
    unsigned long size = 1024ul * 1024ul * 1024ul;
    int loop = 0;// initiate loop
    if (*m > 1024)
        loop = 2000;
    else
        loop = 1000000;
    char *des = (char *)malloc(size * sizeof(char));// give size byte(char) to destination
    char *sor = (char *)malloc(loop * (*m) * sizeof(char));// give size byte(char) to source
    int i = 0;
    for (i = 0; i < loop; i++) {
        int index_1 = rand()%(size - *m - 1); //random index_1
        int index_2 = rand()%(loop * (*m) - *m - 1); //random index_2
        memcpy(des + index_1, sor + index_2, *m);//copy (*m)byte from sor+index_2 to des+index_1
    }
    free(des);//free memory
    free(sor);
    return NULL;
}

/********** memory benchmark **********/

int main(){
    srand((int)time(0)); // initiate a counter
    int i = 0, j = 0;
    for (i = 1; i < 3; i++) {// using 1 and 2 thread(s)
        for (j = 0; j < 3; j++) { // 1 byte, 1024 bytes and 1048576 bytes
            int byte = pow(1024.0, (double)j);
            int k = 0, loop = 0; // initiate loop
            if (byte > 1024)
                loop = 2000;
            else
                loop = 1000000;
            
            struct timeval start_seq, end_seq; // timeval structure
            pthread_t pthread_seq[i]; // define a thread for sequential access
            gettimeofday(&start_seq, NULL);// get strating time
            for (k = 0; k < i; k++)
                pthread_create(&pthread_seq[k], NULL, sequential_access, &byte); //sequential access
            for (k = 0; k < i; k++)
                pthread_join(pthread_seq[k], NULL);// waiting thread end
            gettimeofday(&end_seq, NULL);// get ending time
            double latency = (1000.0 * (end_seq.tv_sec - start_seq.tv_sec) + (end_seq.tv_usec - start_seq.tv_usec) / 1000.0) / loop / byte;
            double throughput = (i * byte / (1024.0 * 1024.0)) / (latency * byte / 1000.0);
            printf("Using %d thread(s), sequential access %10d bytes, the throughput is %10f MB/sec, the latency is %10f ms\n", i, byte, throughput, latency);
            
            
                           
        }
        for (j = 0; j < 3; j++) {
            int byte = pow(1024.0, (double)j);
            int k = 0, loop = 0;
            if (byte > 1024)
                loop = 2000;
            else
                loop = 1000000;
            struct timeval start_ran, end_ran;
            pthread_t pthread_ran[i];// define a thread for random access
            gettimeofday(&start_ran, NULL);
            for (k = 0; k < i; k++)
                pthread_create(&pthread_ran[k], NULL, random_access, &byte);//random access
            for (k = 0; k < i; k++)
                pthread_join(pthread_ran[k], NULL);
            gettimeofday(&end_ran, NULL);
            double latency = (1000.0 * (end_ran.tv_sec - start_ran.tv_sec) + (end_ran.tv_usec - start_ran.tv_usec) / 1000.0) / loop / byte;
            double throughput = (i * byte / (1024.0 * 1024.0)) / (latency * byte / 1000.0);
            printf("Using %d thread(s), random     access %10d bytes, the throughput is %10f MB/sec, the latency is %10f ms\n", i, byte, throughput, latency);
        }
    }
}











