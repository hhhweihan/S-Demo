/* ************************************************************************
> File Name:     fork_test2.c
> Author:        huangweihan
> 
> Created Time:  Wed 27 Oct 2021 07:52:36 AM PDT
> Description:   
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>

int main(int argc, char const* argv[]){
    int fd;
    char c[3];
    char *s = "TestFs";
    fd = open("foobar.txt", O_RDWR,0);
    if(fork() == 0){
        fd = 1; //子进程
        write(fd, s, 7);
        exit(0);
    }
    //父进程
    read(fd, c, 2);
    c[2] = '\0';
    printf("c = %s\n", c);
    exit(0);
}
