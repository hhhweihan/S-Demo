#include <stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
    char*p[100];
    char*str = "hello";
    char*p2;
    p2 = (char*)malloc(sizeof(str));
    strcpy(p2, str);
    p[0]=p2;
    printf("%s\n", p[0]);
    return 0;
}
