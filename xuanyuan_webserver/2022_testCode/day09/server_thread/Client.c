#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
int main() {
    // 1.创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("socket");
        exit(-1);
    }
    // 2.连接服务器端
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(12356);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1) {
        perror("connect");
        exit(-1);
    }
    // 3. 通信
    char recvBuf[1024] = {0};
    int size_recv, total_size = 0;
    // 构造协议族GET包
    char *p_message = "GET / HTTP/1.1\r\n";
    // 调用send 向服务器发送http协议包
    send(fd, p_message,strlen(p_message),0);
    // 设置超时时间
    struct timeval timeout = {3, 0};
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout, sizeof(struct timeval));

    while(1) {
        if((size_recv = recv(fd, recvBuf, 512, 0)) == -1) {
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                printf("recv timeout..\n");
                break;
            }else if(errno == EINTR){
                printf("interrupt bu signal.,.\n");
                continue;
            }else if(errno == ENOENT){
                printf("recv RST segement.,.\n");
                break;
            }else{
                printf("unknown error :%d\n", errno);
                exit(1);
            }
        } else if(size_recv == 0) {
            printf("peer closed...\n");
            break;
        } else{
            total_size += size_recv;
            recvBuf[total_size] = '\0';
            printf("%s", recvBuf);
        }
    }
    // 关闭连接
    close(fd);
    return 0;
}
