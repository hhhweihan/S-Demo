/* ************************************************************************
> File Name:     fnctl.c
> Author:        huangweihan
> 
> Created Time:  Fri 19 Nov 2021 05:20:14 AM PST
> Description:   
 ************************************************************************/
/*
 *      #include<unistd.h>
 *      #include<fcntl.h>
 *      int fcntl(int fd, int cmd, ...);
 *      参数：
 *          fd : 表示需要操作的文件描述符
 *          cmd: 表示对文件描述符进行如何操作
 *              - F_DUPFD :  复制文件描述符, 复制的是第一个参数fd, 得到一个新的文件描述符
 *                  int ret = fcntl(fd, f_DUPFD);
 *              - F_GETFL : 获取指定的文件描述符状态flag
 *                  获取的flag和我们通过open函数传递的flag是一个东西。
 *              - F_SETFL ：设置文件描述符文件状态flag
 *                  必选项：O_RDONLY, O_WRONLY, O_RDWR不可以被修改
 *                  可选项：O_APPEND, O_NONBLOCK
 *                      O_APPEND 表示追加数据
 *                      NONBLOCK 设置成非阻塞
 *          阻塞和非阻塞：描述的是函数调用的行为。add()
 */
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>

int main(){
    // 1. 复制文件描述符
    //int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);
    
    // 2. 修改或者获取文件状态flag
    int fd = open("1.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        return -1;
    }

    // 获取文件描述符状态flag
    int flag = fcntl(fd, F_GETFL);
    if(flag == -1){
        perror("fcntl");
        return -1;
    }
    flag |= O_APPEND;   // flag = flag | O_APPEND

    // 修改文件描述符状态的flag，给flag加入O_APPEND这个标记
    int ret = fcntl(fd, F_SETFL, flag);
    if(ret == -1){
        perror("fcntl");
        return -1;
    }
    char *str = "nihao";
    write(fd, str, strlen(str));

    close(fd);
    return 0;
}
