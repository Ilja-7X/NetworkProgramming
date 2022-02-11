#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sockfd;
    struct sockaddr_in addr;
    int in, out;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket cteation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3429);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Not connection");
        exit(2);
    }
    printf("I is %u, perent: %u, ", getpid(), getppid());

    for (int i = 0; i < 4; i++)
    {
        sleep(i);
        out = i;
        send(sockfd, &out, sizeof(out), 0);

        recv(sockfd, &in, sizeof(in), 0);

        printf("%d\n", in);
    }

    close(sockfd);
    return 0;
}