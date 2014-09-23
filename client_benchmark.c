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
	int bufferSize;
	struct sockaddr_in addrServer;
};

//Define the function of TCP client.
void *tcpClient(void *arg) 
{
	//Initializing related arguments and connect to the server. 
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char *buffer = (char *)malloc(sizeof(char) * (thr_arg -> bufferSize));
	memset(buffer, 1, sizeof(char) * (thr_arg -> bufferSize));
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(client_sock, (struct sockaddr *)&(thr_arg -> addrServer), sizeof(struct sockaddr));

	//Send and receive data. 
	int i = 0;
	for (i = 0; i < LOOPS; ++i)
	{
		send(client_sock, buffer, thr_arg -> bufferSize, 0);
	}
}

//Define the function of UDP client.
void *udpClient(void *arg) 
{
	//Initializing related arguments and connect to the server. 
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char *buffer = (char *)malloc(sizeof(char) * (thr_arg -> bufferSize));
	memset(buffer, 1, sizeof(char) * (thr_arg -> bufferSize));
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	int client_sock = socket(AF_INET, SOCK_DGRAM, 0);

	//Send and receive data. 
	int i = 0;
	for (i = 0; i < LOOPS; ++i)
	{
		sendto(client_sock, buffer, thr_arg -> bufferSize, 0, (struct sockaddr *)&(thr_arg -> addrServer), sizeof(struct sockaddr));
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
	struct sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr(server_ip);
	addrServer.sin_port = htons(4567);

	//Initializing threads and assign related parameters.
	struct thread_arg *thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
	thr_arg -> addrServer = addrServer;
	thr_arg -> bufferSize = buffer_size;
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
