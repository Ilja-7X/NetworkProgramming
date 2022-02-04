#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int sockfd;
    struct sockaddr_in addr;
    int bytes_r;
    char massage[] = "Hello server!!!\n";
    int in, out;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket cteation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1239);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    printf("Port - %d\n", ntohs(addr.sin_port));

    for (int i = 0; i < 4; i++)
    {
        sleep(i);
        out = i;
        sendto(sockfd, &out, sizeof(out), 0, (struct sockaddr *)&addr, sizeof(addr));
        int len = sizeof(addr);
        recvfrom(sockfd, &in, sizeof(in), 0, (struct sockaddr *)&addr, &len);

        printf("%d\n", in);
    }
    close(sockfd);

    return 0;
}