/*
 * 9-21-2018 by kk
 * 最简单的socket聊天小程序（客户端）
 * 一个作为服务器，另外一个作为客户端
 * 由于当前使用的是局域网，因此需要在路由器中设置虚拟服务器，
 * 把内网IP和端口与路由器端口映射
 * note:需要把局域网中的电脑IP设为手动设置模式
 * 输入exit()退出聊天
 * */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

#define MAX_MSG_SIZE 256
#define SERVER_PORT 8000
#define CLIENT_PORT 8001
#define SERVER_ADDR "210.43.57.175" //外网IP，可在路由器中查看，此IP一般也是动态的

int GetServerAddr(char* addrname){
    printf("please input your addr:");
    scanf("%s", addrname);
    return 1;
}
int main() {
    int cli_socketfd;
    int addr_len;
    char seraddr[14];
    struct sockaddr_in ser_addr, cli_addr;
    char msg[MAX_MSG_SIZE];
    cli_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cli_socketfd < 0){
        fprintf(stderr, "socket error:%s\n", strerror(errno));
        exit(1);
    }
    addr_len = sizeof(struct sockaddr_in);
    bzero(&cli_addr, addr_len);
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //cli_addr.sin_addr.s_addr = inet_addr("192.168.1.100");//内网IP，需在路由器中进行映射后
    cli_addr.sin_port = htons(CLIENT_PORT);
    if(bind(cli_socketfd, (struct sockaddr*)&cli_addr, addr_len)==-1){
        fprintf(stderr, "bind error:%s\n", strerror(errno));
        exit(1);
    }
    bzero(&ser_addr, addr_len);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    ser_addr.sin_port = htons(SERVER_PORT);
    if(connect(cli_socketfd, (struct sockaddr*)&ser_addr, addr_len)==-1){
        fprintf(stderr, "connect error:%s\n", strerror(errno));
        close(cli_socketfd);
        exit(1);
    }
    while(1)
    {
        printf("client:");
        fgets(msg, MAX_MSG_SIZE, stdin);
        if(strcmp(msg, "exit()\n")==0) break;
        send(cli_socketfd, msg, sizeof(msg), 0);
        recv(cli_socketfd, msg, MAX_MSG_SIZE, 0);
        printf("server:%s", msg);
    }

    close(cli_socketfd);

    return 0;
}