/* ************************************************************************
> File Name:     rename.c
> Author:        huangweihan
> 
> Created Time:  Sat 13 Nov 2021 06:33:08 AM PST
> Description:   
 ************************************************************************/
/*
 *  #include<stdio.h>
 *  int rename(const char *oldpath, const char *newpath);
 */
#include<stdio.h>


int main(){

    int ret = rename("aaa", "bbb");

    if(ret == -1){
        perror("rename");
        return -1;
    }


    return 0;
}
