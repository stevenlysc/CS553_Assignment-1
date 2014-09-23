//
//  disk_benchmark
//
//  Created by Boyang Li on 9/10/14.
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const int MAX = 1000000000;

void * random_Read(void *);
void * random_Write(void *);
void * sequential_Read(void *);
void * sequential_Write(void *);
int do_Read_Ran(int, int);
int do_Read_Seq(int, int);
int do_Write_Ran(int, int);
int do_Write_Seq(int, int);

int main(int argc, char const *argv[])
{
    if(argc != 5)
	{
		printf("usage: %s <operation type> <access type> <block size> <num of threads>\n", argv[0]);
		return -1;
	}
    
    // argv[0] disk_benchmark
    // argv[1] Read / Write
    // argv[2] ran / seq
    // argv[3] 1b / 1kb / 1mb
    // argv[4] 1 thread / 2 threads / 4 threads
    int thread_n, block_size;
    thread_n = atoi(argv[4]);
    if (strcmp(argv[3], "1B") == 0) {
        block_size = 1;
    }
    else if (strcmp(argv[3], "1KB") == 0){
        block_size = 1024;
    }
    else if (strcmp(argv[3], "1MB") == 0){
        block_size = 1024 * 1024;
    }
    else{
        printf("The block size is not correct.\n");
        return -1;
    }
    
    if (strcmp(argv[1], "Read") == 0) {
        if (strcmp(argv[2], "seq") == 0) {
            do_Read_Seq(block_size, thread_n);
        }
        else if (strcmp(argv[2], "ran") == 0){
            do_Read_Ran(block_size, thread_n);
        }
        else{
            printf("Invalid access type.\n");
            return -1;
        }
    }
    else if (strcmp(argv[1], "Write") == 0){
        if (strcmp(argv[2], "seq") == 0) {
            do_Write_Seq(block_size, thread_n);
        }
        else if (strcmp(argv[2], "ran") == 0){
            do_Write_Ran(block_size, thread_n);
        }
        else{
            printf("Invalid access type.\n");
            return -1;
        }
    }
    else{
        printf("Invalid operation type.\n");
        return -1;
    }
    
    return 0;
}

int do_Read_Ran(int block_size, int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    struct timeval startTime, endTime;
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, random_Read, (void *)(long)block_size);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    int capacity;  // modify file size
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    double execTime = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double throughput = ((double) thread_n * capacity / (1024.0 * 1024.0)) / (execTime / 1000.0); // MBps
    double latency = execTime / capacity; // s/B
    
    printf("With %d threads, for random reading from disk, the throughput is %10lf MBps and the latency is %10.9lf ms\n", thread_n, throughput, latency);
    
    return 0;
}

int do_Read_Seq(int block_size, int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    struct timeval startTime, endTime;
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, sequential_Read, (void *)(long)block_size);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    int capacity; // modify file size
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    double execTime = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double throughput = ((double) thread_n * capacity / (1024.0 * 1024.0)) / (execTime / 1000.0); // MBps
    double latency = execTime / capacity;
    
    printf("With %d threads, for sequential reading from disk, the throughput is %10lf MBps and the latency is %10.9lf ms\n", thread_n, throughput, latency);
    
    return 0;
}

int do_Write_Ran(int block_size, int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    struct timeval startTime, endTime;
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, random_Write, (void *)(long)block_size);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    int capacity; // modiify file size
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    double execTime = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double throughput = ((double) thread_n * capacity / (1024.0 * 1024.0)) / (execTime / 1000.0); // MBps
    double latency = execTime / capacity;
    
    printf("With %d threads, for random writing to disk, the throughput is %10lf MBps and the latency is %10.9lf ms\n", thread_n, throughput, latency);
    
    return 0;
}

int do_Write_Seq(int block_size, int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    struct timeval startTime, endTime;
    
    gettimeofday(&startTime, NULL);
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, sequential_Write, (void *)(long)block_size);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&endTime, NULL);
    
    int capacity; // modify file size
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    double execTime = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    double throughput = ((double) thread_n * capacity / (1024.0 * 1024.0)) / (execTime / 1000.0); // MBps
    double latency = execTime / capacity;
    
    printf("With %d threads, for sequential writing to disk, the throughput is %10lf MBps and the latency is %10.9lf ms\n", thread_n, throughput, latency);
    
    return 0;
}

void * random_Read(void * arg)
{
    int fpos = 0;
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int capacity;
    
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    srand((int)time(0));
    
    int fin;
    // read-only
    if ((fin = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    int i = 0;
    for (i = 0; i < capacity / block_size; i++) {
        // random position in the file
        fpos = (rand() % (capacity / block_size)) * block_size;
        // locate to the random position
        lseek(fin, fpos, SEEK_SET);
        // read file
        read(fin, buffer, block_size);
    }
    
    close(fin);
    free(buffer);
    
    return NULL;
}

void * sequential_Read(void * arg)
{
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int capacity;
    
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    int fin;
    // read-only
    if ((fin = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    
    int i = 0;
    for (i = 0; i < capacity / block_size; i++) {
        // read the file sequentially
        read(fin, buffer, block_size);
    }
    
    close(fin);
    free(buffer);
    
    return NULL;
}

void * random_Write(void * arg)
{
    int fpos = 0;
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int capacity;
    srand((int)time(0));
    
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    int fin;
    // create file / write file
    if ((fin = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    
    int i = 0;
    for (i = 0; i < capacity / block_size; i++) {
        // random position in the file
        fpos = (rand() % (capacity / block_size)) * block_size;
        // set the content to be written
        memset(buffer, '0', block_size);
        // find the random position in file
        lseek(fin, fpos, SEEK_SET);
        // write from buffer into file
        write(fin, buffer, block_size);
    }
    
    close(fin);
    free(buffer);
    
    return NULL;
}

void * sequential_Write(void * arg)
{
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int capacity;
    
    if (block_size == 1) {
        capacity = MAX / 50;
    }
    else if (block_size == 1024) {
        capacity = MAX / 10;
    }
    else {
        capacity = MAX;
    }
    
    int fin;
    // create file / write file
    if ((fin = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    
    int i = 0;
    for (i = 0; i < capacity / block_size; i++) {
        // set buffer content to be written
        memset(buffer, '0', block_size);
        // write to disk sequentially
        write(fin, buffer, block_size);
    }
    
    close(fin);
    free(buffer);
    
    return NULL;
}