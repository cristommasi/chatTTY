#include "../../inc/chatty.h"

void*   tcp_receive(void* arg) {

    t_tcp               *tcp = (t_tcp *)arg;
    int                 newsockfd;
    struct sockaddr_in  cliaddr;
    socklen_t           clilen = sizeof(cliaddr);
    char                buffer[BUF_SIZE];
    ssize_t             n;

    while (1) {

        if ((newsockfd = accept(tcp->sockfd, (struct sockaddr*)&cliaddr, &clilen) ) < 0)
            continue;

        if ((n = read(newsockfd, buffer, BUF_SIZE-1)) <= 0) {
            close(newsockfd);
            continue;
        }
        buffer[n] = '\0'; 
        write(STDOUT_FILENO, "\r\33[K", 4);
        printf("%s\n", buffer);
        write(newsockfd, "OK\n", 3);
        close(newsockfd);

    }
    return (NULL);
}