/* ************************************************************************
> File Name:     fork_test.c
> Author:        huangweihan
> 
> Created Time:  Wed 27 Oct 2021 07:39:30 AM PDT
> Description:   
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char const *argv[]){
    char p = 'p';
    int number = 11;
    if(fork() == 0){   //子进程
        p = 'c';    //子进程对数据的修改
        printf("p = %c , number = %d \n ", p, number);
        exit(0);
    }
    // 父进程
    number = 14;    // 父进程对数据修改
    printf("p = %c , number = %d \n ", p, number);
    exit(0);
}
