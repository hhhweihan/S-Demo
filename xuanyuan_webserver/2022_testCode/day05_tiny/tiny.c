#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char **argv) {
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    listenfd = Open_listenfd(port); //打开监听套接字
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // 连接请求
        doit(connfd);  // 具体处理请求                                           
        Close(connfd);                                            
    }
}

void doit(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE];
    rio_t rio;  // Robust IO
    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);  // 读取一行，在这里是request line   
    // 解析出请求行中的HTTP 方法，uri，版本信息              
    sscanf(buf, "%s %s %s", method, uri, version);       
    if (strcasecmp(method, "GET")) {     // 为了简单现在只是支持GET方法，非GET方法，就告知客户端我们无能为力               
        clienterror(fd, method, "501", "Not Implemented", "Static_Server does not implement this method");
        return;
    }                                                      
    read_requesthdrs(&rio);    // 消耗请求头部                          
    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename);      
    if (stat(filename, &sbuf) < 0) {   // 对应的资源不存在                  
        clienterror(fd, filename, "404", "Not found", "Static_Server couldn't find this file");
        return;
    }                                                    

    if (is_static) { // 请求静态资源          
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { 
            clienterror(fd, filename, "403", "Forbidden", "Static_Server couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);        
    }
}

void read_requesthdrs(rio_t *rp) {
    char buf[MAXLINE];
    Rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) {   // 读取所有行，直到遇到空行，包括空行也会读到       
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}
// 处理URI GET request
int parse_uri(char *uri, char *filename) {             
    if (uri[strlen(uri)-1] == '/')                   
        strcat(filename, "index.html");  
    return 1;
}
// 先返回了响应头，然后采用内存映射读取文件，发送给client
void serve_static(int fd, char *filename, int filesize) {
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);       
    sprintf(buf, "HTTP/1.0 200 OK\r\n");   
    sprintf(buf, "%sServer: Static_Server Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s;charset=utf-8\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));       
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);    
    srcp = (char *)Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd,(off_t) 0);
    Close(srcfd);                           
    Rio_writen(fd, srcp, filesize);       
    Munmap(srcp, filesize);  
}

void get_filetype(char *filename, char *filetype) {
    if (strstr(filename, ".html"))  strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))  strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))  strcpy(filetype, "image/jpeg");
    else    strcpy(filetype, "text/plain");
}  

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];
    /* Build the HTTP response body */
    sprintf(body, "<html><title>Static_Server Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Static_Server Web server</em>\r\n", body);
    /* 构建响应行，响应头部 */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html;charset=utf-8\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));  //空行结束response headers
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
