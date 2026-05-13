#include<iostream>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>

int main(){
    int sock_fd, connfd;
    struct sockaddr_in my_addr;
    char buf[4096];
    FILE *fp;
    int n;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        perror("socket");
        exit(-1);
    }
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    my_addr.sin_port = htons(15333);
    
    // 设置端口可重用
    int contain;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));

    if(bind(sock_fd, reinterpret_cast<struct sockaddr*>(&my_addr),sizeof(struct sockaddr)) == -1);{
        perror("bind");
        exit(1);
    }
    if(listen(sock_fd, 10) == -1){
        perror("listen");
        exit(1);
    }
    if((fp = fopen("index.html", "ab")) == NULL){
        printf("fail.\n");
        close(sock_fd);
        exit(1);
    }
    while(1){
        struct sockaddr_in client_addr;
        socklen_t size = sizeof(client_addr);
        if((connfd = accept(sock_fd,(struct sockaddr*)&client_addr, &size)) == -1){
            printf("accept socket erro: %s(errno: %d)", strerror(errno), errno);
            continue;
        }
        while(1){
            n = read(connfd, buf, 4096);
            if(n == 0) break;
            fwrite(buf, 1, n, fp);
        }
        buf[n] = '\0';
        printf("recv msg from client: %s\n", buf);
        close(connfd);
        fclose(fp);
    }
    close(sock_fd);
    return 0;
}
