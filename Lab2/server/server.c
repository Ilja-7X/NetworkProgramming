#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig)
{
    pid_t id;
    while ((id = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        printf("wait child success: %d\n", getpid());
    }
}

void func(int signum)
{
    wait(NULL);
}

int main()
{
    int sockfd, sock;
    struct sockaddr_in addr, addr_client;
    int bytes_r;
    int in, out;
    //printf("Perent:%u, I is %u \n", getppid(), getpid());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3429);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        exit(2);
    }

    //printf("Perent:%u, I is %u\n", getppid(), getpid());

    listen(sockfd, 5);

    while (1)
    {

        sock = accept(sockfd, NULL, NULL);
        if (sock < 0)
        {
            perror("accept");
            exit(3);
        }

        switch (fork())
        {
        case -1:
            perror("fork");
            break;

        case 0:
            while (1)
            {

                bytes_r = recv(sock, &in, sizeof(in), 0);
                if (bytes_r <= 0)
                {
                    break;
                }
                printf("IP: %s, port: %d\n", inet_ntoa(addr_client.sin_addr), ntohs(addr.sin_port));
                printf("massage: %d\n", in);

                out = in * in;
                send(sock, &out, sizeof(out), 0);
            }
            close(sock);
            exit(0);

        default:
            signal(SIGCHLD, func);

            //printf("My perent is %u, I is %u\n", getppid(), getpid());
        }

        close(sock);
    }
    close(sockfd);

    return 0;
}