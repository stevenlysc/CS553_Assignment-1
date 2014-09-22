#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LOOPS 100

//Define the struct of thread. 
struct thread_arg
{
	int sock;
	int buffer_size;
};

//TCP function that can send and receive data. 
void *tcpOprtData(void *arg)
{
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);

	int i = 0;
	for (i = 0; i < LOOPS; ++i) 
	{
		recv(thr_arg -> sock, buffer, sizeof(char) * thr_arg -> buffer_size, 0);
		send(thr_arg -> sock, buffer, sizeof(char) * thr_arg -> buffer_size, 0);
	}

	free(buffer);
}

//TCP server function. 
void tcpServer(int buffer_size, int num_thr)
{
	int server_sock, client_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	//Initializing network related arguments. 
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4567);

	server_sock = socket(AF_INET, SOCK_STREAM, 0);	//Create socket. 
	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));	//Bind a socket and the address. 
	listen(server_sock, 10);	//listen to a connection. 
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	pthread_t threads[num_thr];

	//Aiming at every specific connection, create a thread to operate the request. 
	int i = 0;
	while(1)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sockaddr_in_size);
		struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
		thr_arg -> sock = client_sock;
		thr_arg -> buffer_size = buffer_size;
		pthread_create(&threads[i], NULL, tcpOprtData, thr_arg);
		i++;
	}
}

//UDP function that can send and receive data.
void *udpOprtData(void *arg)
{
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	struct sockaddr_in client_addr;

	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);
	int sockaddr_in_size = sizeof(struct sockaddr_in);

	int i = 0;
	for (i = 0 ; i < LOOPS; ++i) {
		recvfrom(thr_arg -> sock, buffer, sizeof(char) * thr_arg -> buffer_size, 0, (struct sockaddr *)&client_addr, &sockaddr_in_size);
		sendto(thr_arg -> sock, buffer, sizeof(char) * thr_arg -> buffer_size, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
	}
}

//UDP server function.
void udpServer(int buffer_size, int num_thr)
{
	int server_sock;
	struct sockaddr_in server_addr;

	//Initializing related arguments.
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4567);
	server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	pthread_t threads[num_thr];
	int i = 0;
	
	struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
	thr_arg -> sock = server_sock;
	thr_arg -> buffer_size = buffer_size;

	//For every request, create a thread to operate it. 
	for (i = 0; i < num_thr; ++i) 
	{
		pthread_create(&threads[i], NULL, udpOprtData, thr_arg);
	}
	for (i = 0; i < num_thr; ++i) 
	{
		pthread_join(threads[i], NULL);
	}
}

//Main function, invoke corresponding function according to the input. 
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
