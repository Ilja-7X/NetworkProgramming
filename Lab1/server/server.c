#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int sockfd;
    struct sockaddr_in addr, addr_client;
    int bytes_r;
    char buf[256];
    int in, out;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1239);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    int size = sizeof(addr);
    getsockname(sockfd, (struct sockaddr *)&addr, &size);
    printf("Port - %d\n", ntohs(addr.sin_port));

    for (int i = 0; i < 8; i++)
    {
        int len = sizeof(addr_client);
        bytes_r = recvfrom(sockfd, &in, sizeof(in), 0, (struct sockaddr *)&addr_client, &len);

        printf("%d\n", i);
        printf("%s\n", inet_ntoa(addr_client.sin_addr));

        out = in * in;
        sendto(sockfd, &out, sizeof(out), 0, (struct sockaddr *)&addr_client, sizeof(addr_client));
        printf("n - %d\n", i);
    }
    return 0;
}