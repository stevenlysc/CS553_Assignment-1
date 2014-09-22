#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

const int DATA_SIZE = 50000000

void * udpServer(void *);
void * tcpServer(void *);
int setTCPServer(int);
int setUDPServer(int);

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("usage: %s <connect type> <num of thread>\n", argv[0]);
        return -1;
    }
    
    int thread_n = atoi(argv[2]);
    
    if (strcmp(argv[1], "TCP") == 0) {
        setTCPServer(thread_n);
    }
    else if (strcmp(argv[1], "UDP") == 0) {
        setUDPServer(thread_n);
    }
    else {
        printf("Error connect type.\n");
        return -1;
    }
    
    return 0;
}

int setUDPServer(int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, udpServer, (void *)(long)i);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}

int setTCPServer(int thread_n)
{
    int i = 0;
    pthread_t threads[thread_n];
    
    for (i = 0; i < thread_n; i++) {
        pthread_create(&threads[i], NULL, tcpServer, (void *)(long)i);
    }
    for (i = 0; i < thread_n; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}

void * udpServer(void * tid)
{
    int sockfd, err, size;
    char * buffer;
    struct sockaddr_in addrServer addrClient;
    socklen_t addrlen;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Socket error.\n");
        exit(-1);
    }
    
    memset(&addrServer, 0, sizeof(addrServer));
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //addrServer.sin_port = hons(PORT + (int)(long)tid);
    
    if ((err = bind(sockfd, (struct sockaddr *)&addrServer, sizeof(addrServer))) < 0) {
        printf("Error!\n");
        exit(-1);
    }
    if ((size = recvfrom(sockfd, buffer, DATA_SIZE, 0, (struct *)&addrClient, &addrlen)) < 0) {
        printf("Error!\n");
        exit(-1);
    }
    
    close(sockfd);
    
    return NULL;
}

void * tcpServer(void * tid)
{
    int sockfdServer, sockfdClient, err, size;
    char * buffer;
    struct sockaddr_in addrServer;
    socklen_t addrlen;
    
    if ((sockfdServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket error.\n");
        exit(-1);
    }
    
    memset(&addrServer, 0, sizeof(addrServer));
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr = htonl(INADDR_ANY);
    
    //addrServer.sin_port = htons(PORT + (int)(long)tid);
    
    if ((err = bind(sockfdServer, (struct sockaddr *)&addrServer,sizeof(addrServer))) < 0) {
        printf("Error!\n");
        exit(-1);
    }
    if ((err = listen(sockfdServer, 3)) < 0) {
        printf("Error!\n");
        exit(-1);
    }
    
    addrlen = sizeof(struct sockaddr);
    
    if ((sockfdClient = accept(sockfdServer, (struct sockaddr *)&sockfdClient, &addrlen)) < 0) {
        printf("Error!");
        exit(-1);
    }
    
    buffer = (char *)malloc(DATA_SIZE);
    size = DATA_SIZE;
    
    while (size > 0){
        size = recv(sockfdClient, buffer, DATA_SIZE, 0);
    }
    
    close(sockfdClient);
    close(sockfdServer);
}