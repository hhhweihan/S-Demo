/* ************************************************************************
> File Name:     exit.c
> Author:        huangweihan
> 
> Created Time:  Fri 19 Nov 2021 10:43:45 PM PST
> Description:   
 ************************************************************************/
/*
 *      #include<stdlib.h>
 *      void exit(int status);
 *
 *      #include<unistd.h>
 *      void _exit(int status);
 *
 *      status 参数：是进程退出时的一个状态信息。父进程回收子进程资源得时候可以获取得到
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

    printf("hello\n");
    printf("world");

    //exit(0);
    _exit(0);

    return 0;
}

