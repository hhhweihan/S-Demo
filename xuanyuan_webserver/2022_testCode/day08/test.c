#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char *p;
    FILE *fp;
    char *str;
    char *name = "./index.html";
    char buff[4096];
    int filesize;
    if((fp = fopen(name, "r")) == NULL){
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    str=(char *)malloc(filesize);
    str[0] = 0;
    rewind(fp);
    while((fgets(buff, 4096, fp))!= NULL){
        strcat(str, buff);
    }
    fclose(fp);
    p = str;
    if(p!=NULL) puts(p);
    return 0;
}
