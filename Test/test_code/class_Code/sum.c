/* ************************************************************************
> File Name:     sum.c
> Author:        huangweihan
> 
> Created Time:  Fri 29 Oct 2021 07:41:54 PM PDT
> Description:   
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>

long plus(long x, long y);
void sumstore(long x, long y, long *dest){
    long t = plus(x, y);
    *dest = t;
}
long plus(long x, long y){
    return x + y;
}
int main(int argc, char const* argv[]){
    long x = atoi(argv[1]);
    long y = atoi(argv[2]);
    long z;
    sumstore(x, y, &z);
    printf("%ld + %ld -> %ld \n", x, y, z);
    return 0;
}
