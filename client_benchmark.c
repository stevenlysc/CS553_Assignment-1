#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

const int DATA_SIZE = 500000

typedef struct Data {
    int packet_size;
    int thread_id;
} data;