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
#include <pthread.h>
#include <sys/select.h>

#define BUF_LEN             1024
#define SERVER_PORT         6666

/**
 * @brief socket_send
 * @param arg
 * @return
 */
void *socket_recv(void *arg)
{
    int sockfd = *(int *)arg;
    char buff[BUF_LEN];
    int n;
    fd_set readfds;
    struct timeval time_out = {0,0};

    for(;;){

        FD_ZERO(&readfds);
        FD_SET(sockfd,&readfds);

        n = select(sockfd + 1, &readfds, NULL, NULL, &time_out);//no block

        if(n == 0){
            //do nothing;
        }
        else if(n < 0){
            //error
            break;
        }
        else{
            if(FD_ISSET(sockfd ,&readfds)){
                n = recv(sockfd, buff, BUF_LEN, 0);
                if(n > 0){
                    buff[n] = '\0';
                    printf("Server says:%s\n", buff);
                }
                else break;         //socket closed.
            }
        }
    }

    pthread_exit(NULL);
}

/**
 * @brief socket_send
 * @param arg
 * @return
 */
void *socket_send(void *arg)
{
    int sockfd = *(int *)arg;
    char buff[BUF_LEN];

    for(;;){
        fgets(buff, BUF_LEN, stdin);
        if(send(sockfd, buff, strlen(buff), 0) < 0){
            printf("Send msg error:%s(errorno:%d)\n",strerror(errno),errno);
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockfd;
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

    pthread_t tid;
    int ret;
    if((ret = pthread_create(&tid, NULL, &socket_recv,(void *)&sockfd)) != 0){
        printf("Connect error:%s(errorno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    pthread_detach(tid);

    while(1)
    {
        //send msg
        socket_send((void *)&sockfd);
        sleep(1);
    }

    close(sockfd);

    return 0;
}
