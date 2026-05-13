/* ************************************************************************
> File Name:     create.c
> Author:        huangweihan
> 
> Created Time:  Tue 09 Nov 2021 05:05:24 AM PST
> Description:   
 ************************************************************************/
/*
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
 
  int open(const char *pathname, int flags, mode_t mode);
    参数：
        - pathname: 要创建的文件的路径
        - flags: 对文件的操作权限和其它的设置
            - 必选项： O_RDONLY, O_WRONLY, O_RDWR这三个之间是互斥的
            - 可选项： O_CREAT 文件不存在，创建新文件
        - mode: 八进制的数，表示用户对创建出的新的文件的操作权限， 比如：0775 
        最终的权限是：mode & ~umask
        0777

        umask的作用就是抹去某些权限
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    //创建一个新的文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1){
        perror("open");
    }
    //关闭
    close(fd);

    return 0;
}


