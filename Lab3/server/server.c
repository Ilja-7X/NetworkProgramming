#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_OF_THREADS 10
#define PORT 4358

void *handler(void *arg);

struct server
{
    pthread_mutex_t st_mutex;
} GLOBAL;

struct data
{
    int sock;
    struct sockaddr_in clientaddr;
};

int real_thread = 0;

int main()
{
    int listener, sock;
    struct sockaddr_in addr, clientaddr;
    socklen_t len;
    struct data *arg[NUM_OF_THREADS];
    pthread_t threads[NUM_OF_THREADS];
    pthread_attr_t attr;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        exit(2);
    }

    listen(listener, 3);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&GLOBAL.st_mutex, 0);

    len = sizeof(clientaddr);

    for (int i = 0; i < 3; i++)
    {
        sock = accept(listener, (struct sockaddr *)&clientaddr, &len);
        if (sock < 0)
        {
            perror("accept");
            exit(3);
        }
        printf("Sock in main: %d\n", sock);

        arg[i] = (struct data *)malloc(sizeof(struct data));
        arg[i]->sock = sock;
        //memcpy((void *)&arg[i]->clientaddr, &clientaddr, sizeof(clientaddr));

        if (pthread_create(&threads[i], &attr, (void *)handler, (void *)arg[i]) < 0)
        {
            perror("pthread");
            return 1;
        }

        real_thread = real_thread + 1;
    }
    printf("Waiting for join\n");
    for (int i = 0; i < real_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //pthread_exit(NULL);

    printf("Memory is beging released\n");

    for (int i = 0; i < real_thread; i++)
    {
        free(arg[i]);
    }

    close(listener);
    printf("Program is completed\n");

    return 0;
}

void *handler(void *arg)
{
    int num;
    int in;

    struct data *info = (struct data *)arg;

    //printf("Hendler is begin\n");
    printf("info->sock = %d\n", info->sock);

    if ((num = recv(info->sock, &in, sizeof(in), 0) == -1))
    {
        perror("Recv");
        exit(0);
    }
    //printf("Sock: %d, message: %d\n", info->sock, in);
    //sleep(10);
    //printf("Hendler is end\n");

    close(info->sock);

    //pthread_exit(NULL);

    return 0;
}