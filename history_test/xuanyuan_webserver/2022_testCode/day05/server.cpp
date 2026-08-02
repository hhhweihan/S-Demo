#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int sock_fd, new_fd;

    struct sockaddr_in my_addr;
    struct sockaddr_in other_addr;

    // create socket
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    // connect
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(15235);
    inet_pton(AF_INET, "0.0.0.0", &my_addr.sin_addr.s_addr);
    bzero(&(my_addr.sin_zero), 8);
 
    // bind
    if(bind(sock_fd, reinterpret_cast<struct sockaddr*>(&my_addr), sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }
    // listen
    if(listen(sock_fd, 10) == -1){
        perror("listen");
        exit(1);
    }
    // 通信
    unsigned int sin_size;
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sock_fd, reinterpret_cast<struct sockaddr*>(&other_addr), &sin_size)) == -1){
            perror("accept");
            continue;
        }
        
        char recv_buf[65535];
        char send_buf[65535];
        char html_buf[1024] = "<html><body><h1>这是我的第一个网页！</h1></body></html>";
        sprintf(send_buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\nContent-Length: %lu\r\n\r\n%s", strlen(html_buf), html_buf);

        if(!fork()){
            recv(new_fd, recv_buf, 65535, 0);
            std::cout << recv_buf << std::endl;
            if(send(new_fd, send_buf, strlen(send_buf), 0) == -1){
                perror("send");
                close(new_fd);
                exit(0);
            }
            close(new_fd);
        }
    }
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
