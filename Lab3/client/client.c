#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 4358

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in addr;
    int in, out;
    int deley = atoi(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket cteation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Not connection");
        exit(2);
    }
    printf("I is %u, perent: %u\n", getpid(), getppid());

    sleep(deley);

    out = deley;
    send(sockfd, &out, sizeof(out), 0);

    close(sockfd);
    return 0;
}