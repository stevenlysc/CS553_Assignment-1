//
//  Cuda.cpp
//  
//
//  Created by 胡行坦 on 14-9-20.
//
//

#include <stdio.h>
#include <sys/time.h>

#define SIZE  1024


__global__ void VectorAdd(int *c, int *a, int *b, int n){
    int i = threadIdx.x;
    
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

__global__ void VectorAdd_f(float *c, float *a, float *b, float n){
    int i = threadIdx.x;
    
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}
/********************* calculate read and write bandwidth****************************/
int bandwidth(){
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    for (int i=1 ; i <= 1024*1024; i = i * 1024) {
        double size_0 = i;
        
        // Allocate memory block(in CPU), returns a pointer
        a = (int *)malloc(size_0*sizeof(int));
        b = (int *)malloc(size_0*sizeof(int));
        c = (int *)malloc(size_0*sizeof(int));

        // Allocate memory block(in GPU), returns a pointer
        cudaMalloc( &d_a, size_0*sizeof(int));
        cudaMalloc( &d_b, size_0*sizeof(int));
        cudaMalloc( &d_c, size_0*sizeof(int));
        

        // structure for calculate the interval
        struct timeval start_read, end_read;
        struct timeval start_write, end_write;
        for (int j = 0; j < size_0; ++j) {
            a[j] = j;
            b[j] = j;
            c[j] = 0;
        }
        
        gettimeofday(&start_read, NULL);
	for (int i = 0; i < 1000000; i++) {
            cudaMemcpy( d_a, a, size_0*sizeof(int), cudaMemcpyHostToDevice ); // copy memory from CPU to GPU
            cudaMemcpy( d_b, b, size_0*sizeof(int), cudaMemcpyHostToDevice );
            cudaMemcpy( d_c, c, size_0*sizeof(int), cudaMemcpyHostToDevice );
	}
        gettimeofday(&end_read, NULL);

        // read_bandwidth = the size of memory copied from CPU to GPU /  interval
        float read_bandwidth = ((float)size_0*sizeof(int)*3*1000000)/(1000.0*(end_read.tv_sec-start_read.tv_sec)+(end_read.tv_usec-start_read.tv_usec)/1000.0);
	// (((size_0/(1024*1024))*3)*(1e6))/ (1000*1000*(end_read.tv_sec - start_read.tv_sec) + (end_read.tv_usec - start_read.tv_usec));
        printf ("with the size of %lf bytes, the read_bandwidth is %lf MBps\n ", size_0, read_bandwidth/1000.0);
    
        VectorAdd<<< 1, size_0 >>>(a, b, c, size_0);
        
        gettimeofday(&start_write, NULL);
	for (int i = 0; i < 1000000; i++) {
            cudaMemcpy( c, d_c, size_0*sizeof(int), cudaMemcpyDeviceToHost ); // copy memory from GPU to CPU
        }
	gettimeofday(&end_write, NULL);
         
         // read_bandwidth = the size of memory copied from GPU to CPU /  interval
        float write_bandwidth = ((float)size_0*sizeof(int)*1000000)/ ((float)(end_write.tv_sec - start_write.tv_sec)*1000.0*1000.0 +(float)(end_write.tv_usec - start_write.tv_usec));
        printf ("with the size of %lf bytes, the write_bandwidth is %lf MBps\n ", size_0, write_bandwidth);
    }

    // free memory

    free(a);
    free(b);
    free(c);
    
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    
    return 0;
}

/******************* claculate FLOPS and IOPS **********************/
int Operations(){
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    float *a_f, *b_f, *c_f;
    float *d_a_f, *d_b_f, *d_c_f;
    
    // declare interger and float variable(pointer) for calculation

    a = (int *)malloc(SIZE*sizeof(int));
    b = (int *)malloc(SIZE*sizeof(int));
    c = (int *)malloc(SIZE*sizeof(int));
    a_f = (float *)malloc(SIZE*sizeof(float));
    b_f = (float *)malloc(SIZE*sizeof(float));
    c_f = (float *)malloc(SIZE*sizeof(float));
    
    cudaMalloc( &d_a, SIZE*sizeof(int));
    cudaMalloc( &d_b, SIZE*sizeof(int));
    cudaMalloc( &d_c, SIZE*sizeof(int));
    cudaMalloc( &d_a_f, SIZE*sizeof(float));
    cudaMalloc( &d_b_f, SIZE*sizeof(float));
    cudaMalloc( &d_c_f, SIZE*sizeof(float));
    
    struct timeval start_seq, end_seq;
    struct timeval start_f, end_f;
    
    int i = 0;
    for (i = 0; i < SIZE; ++i) {
        a[i] = i;
        b[i] = i;
        c[i] = 0;
    }
    
    for (i = 0; i < SIZE; ++i) {
        a_f[i] = i * 1.0;
        b_f[i] = i * 1.0;
        c_f[i] = 0.0;
    }
    
    
    cudaMemcpy( d_a, a, SIZE*sizeof(int), cudaMemcpyHostToDevice );
    cudaMemcpy( d_b, b, SIZE*sizeof(int), cudaMemcpyHostToDevice );
    cudaMemcpy( d_c, c, SIZE*sizeof(int), cudaMemcpyHostToDevice );
    cudaMemcpy( d_a_f, a_f, SIZE*sizeof(float), cudaMemcpyHostToDevice );
    cudaMemcpy( d_b_f, b_f, SIZE*sizeof(float), cudaMemcpyHostToDevice );
    cudaMemcpy( d_c_f, c_f, SIZE*sizeof(float), cudaMemcpyHostToDevice );
    
    gettimeofday(&start_seq, NULL);
   

    int n = 0; 
    // loop here is used to extend the operating time of CPU
    for (n = 0; n < 1000; n++) {
        VectorAdd<<< 1, SIZE >>>(a, b, c, SIZE); // calculate  a[ ] + b[ ] (interger)
    }
    
    gettimeofday(&end_seq, NULL);
    
    gettimeofday(&start_f, NULL);
    
    for (n = 0; n < 1000; n++) {
        VectorAdd_f<<< 1, SIZE >>>(a_f, b_f, c_f, SIZE); // calculate  a[ ] + b[ ] (float)
    }
    
    gettimeofday(&end_f, NULL);
    
    // IOPS = size * loop / interval
    float IOPS = ((SIZE*1000)/ ((1000.0 * (end_seq.tv_sec - start_seq.tv_sec) + (end_seq.tv_usec - start_seq.tv_usec) / 1000.0)/1000)/1e9);
    
    printf("the IOPS is %30f GIOPS\n", IOPS);
    
    // FLOPS = size * loop / interval
    float FLOPS = ((SIZE*1000)/ ((1000.0 * (end_f.tv_sec - start_f.tv_sec) + (end_f.tv_usec - start_f.tv_usec) / 1000.0)/1000)/1e9);
    
    printf("the FLOPS is %30f GFLOPS\n", FLOPS);
    
    //cudaMemcpy( c, d_c, SIZE*sizeof(int), cudaMemcpyDeviceToHost );
    
    //for (int i = 0; i < 10; ++i) {
        //printf("c[%d] = %d\n", i, c[i]);
    //}
    

    // free memory

    free(a);
    free(b);
    free(c);
    free(a_f);
    free(b_f);
    free(c_f);
    
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    cudaFree(d_a_f);
    cudaFree(d_b_f);
    cudaFree(d_c_f);

    return 0;    
}

int main(void){
    bandwidth();
    Operations();
    return 0;
}
