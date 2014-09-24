#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LOOPS 100

//Structure of thread information.
struct thread_arg
{
	int sock;
	int buffer_size;
};

//TCP Server receiving data.
void *tcpOprtData(void *arg)
{
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);

	int i = 0;
	for (i = 0; i < LOOPS; ++i) 
	{
		recv(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0);
	}

	free(buffer);
    return NULL;
}

//TCP Server function.
void tcpServer(int buffer_size, int num_thr)
{
	int server_sock, client_sock, err_bind;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	if (buffer_size == 65536)
	{
		buffer_size = 65507;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4567);

	//create socket.
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	err_bind = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if(err_bind == -1)
	{
		exit(-1);
	}

	//Begin listening.
	listen(server_sock, 10);
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	pthread_t threads[num_thr];

	//Create many threads to process. 
	int i = 0;
	while(i < num_thr)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sockaddr_in_size);
		if(client_sock == -1)
		{
			exit(-1);
		}
		struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
		thr_arg -> sock = client_sock;
		thr_arg -> buffer_size = buffer_size;
		pthread_create(&threads[i], NULL, tcpOprtData, thr_arg);
		pthread_join(threads[i], NULL);
		i++;
	}

	close(client_sock);
	close(server_sock);	
}

//UDP server receiving data. 
void *udpOprtData(void *arg)
{
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	struct sockaddr_in client_addr;

	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);
	int sockaddr_in_size = sizeof(struct sockaddr_in);

	int i = 0;
	for (i = 0 ; i < LOOPS; ++i) {
		recvfrom(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0, (struct sockaddr *)&client_addr, &sockaddr_in_size);
	}
    return NULL;
}

//Function of UDP server. 
void udpServer(int buffer_size, int num_thr)
{
	int server_sock, err_bind;
	struct sockaddr_in server_addr;
	if (buffer_size == 65536)
	{
		buffer_size = 65507;
	}
	char * buffer = (char *)malloc(sizeof(char) * buffer_size);
	memset(buffer, 0, sizeof(char) * buffer_size);
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4567);

	//Create socket. 
	server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_sock == -1)
	{
		exit(-1);
	}

	//Bind socket. 
	err_bind = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if(err_bind == -1)
	{
		exit(-1);
	}

	pthread_t threads[num_thr];
	int i = 0;
	
	struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
	thr_arg -> sock = server_sock;
	thr_arg -> buffer_size = buffer_size;

	//Create many threads to process. 
	for (i = 0; i < num_thr; ++i) 
	{
		pthread_create(&threads[i], NULL, udpOprtData, thr_arg);
	}
	for (i = 0; i < num_thr; ++i) 
	{
		pthread_join(threads[i], NULL);
	}

	close(server_sock);
}

//Main function, select protocol according to the input. 
int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("You should input four arguments: TCP/UDP, buffer size and number of threads\n");
		return 1;
	}

	int buffer_size = atoi(argv[2]);
	int num_thr = atoi(argv[3]);

	if (strcmp(argv[1], "TCP") == 0)
	{
		tcpServer(buffer_size, num_thr);
	} else {
		udpServer(buffer_size, num_thr);
	}

	return 0;
}
