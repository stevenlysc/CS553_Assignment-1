#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define LOOPS 100

//Define the struct of thread. 
struct thread_arg
{
	int buf_size;
	struct sockaddr_in server_addr;
};

//Define the function of TCP client.
void *tcpClient(void *arg) 
{
	//Initializing related arguments and connect to the server. 
	int err_connect;
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char *buffer = (char *)malloc(sizeof(char) * (thr_arg -> buf_size));
	memset(buffer, 'a', sizeof(char) * (thr_arg -> buf_size));
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	err_connect = connect(client_sock, (struct sockaddr *)&(thr_arg -> server_addr), sizeof(struct sockaddr));
	if(err_connect == -1)
	{
		printf("Error: TCP connect\n");
		exit(-1);
	}
	//Send and receive data. 
	int i = 0;
	for (i = 0; i < LOOPS; ++i)
	{
		send(client_sock, buffer, thr_arg -> buf_size, 0);
	}

	close(client_sock);
}

//Define the function of UDP client.
void *udpClient(void *arg) 
{
	//Initializing related arguments and connect to the server. 
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char *buffer = (char *)malloc(sizeof(char) * (thr_arg -> buf_size));
	memset(buffer, 1, sizeof(char) * (thr_arg -> buf_size));
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_sock == -1)
	{
		printf("Error: UDP client socket\n");
		exit(-1);
	}

	//Send and receive data. 
	int i = 0;
	for (i = 0; i < LOOPS; ++i)
	{
		sendto(client_sock, buffer, thr_arg -> buf_size, 0, (struct sockaddr *)&(thr_arg -> server_addr), sizeof(struct sockaddr));
	}
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		printf("You should input five arguments: TCP/UDP, server ip address, buffer size and number of threads.\n");
		return(1);
	}
	//Get parameters from the input.
	char *server_ip = argv[2];
	int buffer_size = atoi(argv[3]);
	int num_thr = atoi(argv[4]);
	if (buffer_size == 65536)
	{
		buffer_size = 65507;
	}
	//Define and initialize the socket related parameters.
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	server_addr.sin_port = htons(4567);

	//Initializing threads and assign related parameters.
	struct thread_arg *thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
	thr_arg -> server_addr = server_addr;
	thr_arg -> buf_size = buffer_size;
	pthread_t threads[num_thr];

	//Define the start time and end time. 
	int i = 0;
	struct timeval start_time, end_time;

	//Start executing communication using multi-threads, get the start time and end time. 
	gettimeofday(&start_time, NULL);
	if (strcmp(argv[1], "TCP") == 0)
	{
		for (i = 0; i < num_thr; ++i)
		{
			pthread_create(&threads[i], NULL, tcpClient, thr_arg);
		}
		for (i = 0; i < num_thr; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	} else {
		for (i = 0; i < num_thr; ++i)
		{
			pthread_create(&threads[i], NULL, udpClient, thr_arg);
		}
		for (i = 0; i < num_thr; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	}
	gettimeofday(&end_time, NULL);

	//Calculate the execte time and throughput. 
	double execute_time = (1000.0 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000.0);
	double throughput = (num_thr * LOOPS * buffer_size / (1024.0 * 1024.0)) / (execute_time / 1000.0);
	double latency = execute_time / (num_thr * LOOPS * buffer_size);
	printf("%d threads: the latency is %10.9f ms and the throughput is %10f MB/S\n", num_thr, latency,  throughput);

	return 0;
}
