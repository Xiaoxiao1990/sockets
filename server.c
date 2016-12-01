/**************
 *
 * Server
 *
 * **********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE             4096
#define SERVER_PORT         6666
#define MAX_LINKS           10

void ip_ultos(uint32_t uipadrr ,char *s)
{
    int i = 0,j = 0,n;
    char byte[4];
    for(i = 0;i < 4;i++){
        byte[i] = uipadrr&0xff;
        uipadrr >>= 8;

        n = byte[i]/100;
        if(n > 0)
        {
            s[j++] = n + '0';
        }
        else
        {
            s[j++] = ' ';
        }
        printf("%c",s[j-1]);
        n = byte[i]%100/10;
        if(n > 0)
        {
            s[j++] = n + '0';
        }
        else
        {
            s[j++] = ' ';
        }
printf("%c",s[j-1]);
        s[j++] = byte[i]%10 + '0';
        printf("%c",s[j-1]);
        s[j++] = '.';
        printf("%c",s[j-1]);
    }
    s[--j] = '\0';
}

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    int n;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("Create socket error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    if(bind(listenfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        printf("Bind socket error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    if(listen(listenfd, MAX_LINKS) == -1){
        printf("Listen socket error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    printf("==================== Waiting For Client's Request ====================\n");

    while(1){
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        //if((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1){
        if((connfd = accept(listenfd, (struct sockaddr *)&client, &len)) == -1){
            printf("Accept socket error:%s(errorno:%d)\n",strerror(errno),errno);
            continue;
        }

        char *from = inet_ntoa(client.sin_addr);
        printf("[%s:%d] Connected !\n", from, client.sin_port);
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';

        printf("Recv msg from client[%s:%d]:%s\n", from, client.sin_port, buff);
        close(connfd);
    }

    close(listenfd);

    return 0;
}
