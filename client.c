/***
 * Client
 *
 * ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE             4096
#define SERVER_PORT         6666

int main(int argc, char *argv[])
{
    int sockfd, n;
    char recvline[MAXLINE],sendline[MAXLINE];
    struct sockaddr_in  servaddr;

    if(argc != 2){
        printf("Usage:./client <ipaddress>\n");
        exit(0);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Create socket error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",argv[1]);
        exit(0);
    }

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("Connect error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    printf("Send msg to server:\n");

    fgets(sendline, MAXLINE, stdin);

    if(send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("Send msg error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    close(sockfd);

    return 0;
}
