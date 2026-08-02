#include "cachelab.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char *optarg;
extern int optind, opterr, optopt;

long htoi(char *s);  //十六进制字符转成十进制整数

struct ROW{
   int validBit;     //有效位
   int tagBit;       //标记位
};

struct LRU_ROW{
   struct line *head;   //链表头
   struct ROW *E_ROW;   //组
};

struct line{
   struct line *prior;  //指向直接前趋
   struct line *next;   //指向直接后继
   int tagBit;          //标记
};


int
main(int argc, char *argv[]){
   int hit_count=0,miss_count=0,eviction_count=0;  //命中，未命中，驱逐
   int opt;
   int s=0;      //高速缓存组S=2^s
   int E=0;      //高速缓存行
   int b=0;      //数据块B=2^b
   int tag_v=0;  //是否查看过程信息
   char *file_name;
   FILE *file;
   //提示(帮助)的信息
   char help[]="Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "-h          Print this help message.\n"
            "-v          Optional verbose flag.\n"
            "-s <num>    Number of set index bits.\n"
            "-E <num>    Number of lines per set.\n"
            "-b <num>    Number of block offset bits.\n"
            "-t <file>   Trace file.\n\n"
            "Examples:\n"
            "linux>     %s  -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "linux>     %s  -v -s 8 -E 2 -b 4 -t traces/yi.trace\n";

   //"sEbt:hv"：-s、-E、-b、-t 可接收字符，-h和-v不接收
   while ((opt = getopt(argc, argv, "sEbt:hv")) != -1) {
      switch (opt) {
         case 'h':  
            fprintf(stderr, help,argv[0],argv[0],argv[0]);  //argv[0]是命令行参数的第一个值，即运行的可执行文件名
            break;
         case 'v':
            tag_v = 1;
            break;
         case 's':                        
            s=atoi(argv[optind]);   //atoi字符转十进制
            break;
         case 'E':                        
            E=atoi(argv[optind]);
            break;
         case 'b':                       
            b=atoi(argv[optind]);
            break;
         case 't':
            file_name=argv[optind-1];     //获取文件名
            file = fopen(file_name,"r");  //打开文件
            if(file == NULL){
               perror(file_name);
               exit(EXIT_FAILURE);
            }
            break;
         default:
            fprintf(stderr, help,argv[0],argv[0],argv[0]);
            exit(EXIT_FAILURE);
      }
   }

   /**
    * 设置模拟缓存。初始化
    * S=2^s组，即1<<s。
    * B=2^b字节，即1<<b。
    **/

   int S=1<<s;
   int block_size=1<<b;    //数据块的大小

   struct LRU_ROW cacheLine[S];
   for(int i = 0;i < S;i++){
      cacheLine[i].head=(struct line*)malloc(sizeof(struct line));
      cacheLine[i].head->tagBit=-1;
      cacheLine[i].E_ROW=malloc(sizeof(struct ROW)*E);
      for(int j = 0;j < E;j++){
         cacheLine[i].E_ROW[j].validBit=0;    
         cacheLine[i].E_ROW[j].tagBit=-1;
      }
   }

   int mask_b = (int)pow(2,b)-1;           //创建块偏移b位的掩码
   int mask_s = ((int)pow(2,s)-1)<<b;      //创建组索引s位的掩码

   char ch[16];                     //刚好可以容纳一行数据的大小

   const char Separator[3]=" ,";    //分隔符
   char *token;                     //子串
   
   while(fgets(ch,16,file)!=NULL){
      token = strtok(ch,Separator);
      char *instruction = token;       //内存访问的类型
      if (*instruction == 'I')         //I类型不用处理
      {
         continue;
      }
      token = strtok(NULL,Separator);
      char *addres_char = token;       //地址的字符表示
      long addres = htoi(addres_char); //地址的十进制表示

      token = strtok(NULL,Separator);
      int size = atoi(token);  

      int cacheLine_index = (addres & mask_s)>>b;  //组索引
      int tag = addres>>s>>b;                     //标记位  

      if(tag_v) printf("%s %s,%d ",instruction,addres_char,size);

      int tag_M = *instruction == 'M' ? 2:1;          //'M' 循环两遍,'S''I'一遍
      do{
         struct LRU_ROW *cache_lru_row = &cacheLine[cacheLine_index];   //获取该组
         struct line *body = cache_lru_row->head;     //获取该组的双链表头
         for(int i = 0;i < E;i++){                    //遍历一组中的所有行
            struct ROW *one_row = &cache_lru_row->E_ROW[i]; //获取一行
            if(one_row->validBit && one_row->tagBit == tag){     //有效且标记相同则命中

                  //找到双链表中命中的缓存，准备放到链表头
                  while(body!=NULL){
                     if(body->tagBit == tag){
                        break;
                     }
                     body=body->next;
                  }

                  //放到链表头，要判断，是不是本身就是头，是头就跳过
                  if (body!=cache_lru_row->head){
                     body->prior->next = body->next;

                     //判断是不是链表尾，是尾就跳过
                     if(body->next != NULL){
                        body->next->prior = body->prior;
                     }

                     body->prior == NULL;
                     body->next = cache_lru_row->head;
                     cache_lru_row->head->prior=body;
                     cache_lru_row->head = body;
                  }
                  hit_count++;                        //命中
                  if(tag_v) printf("hit ");
                  goto end;
            }
         }

         for(int i = 0;i < E;i++){      //无效：未命中，要写入无效的缓存区
            struct ROW *one_row = &cache_lru_row->E_ROW[i]; //获取一行
            if(!(one_row->validBit)){ 
               //准备放入链表的新结点
               struct line * temp=(struct line*)malloc(sizeof(struct line));
               temp->prior=NULL;
               temp->next=NULL;
               temp->tagBit = tag;  

               //判断是不是链表中的第一个结点，是就跳过
               if(body->tagBit!=-1){
                  temp->next = body;
                  body->prior = temp;
               }
               //设置尾头结点
               cache_lru_row->head = temp;

               miss_count++;     
               one_row->validBit = 1;
               one_row->tagBit = tag;

               if(tag_v)printf("miss ");
               goto end;
            }
         }
         /**
          * 又没命中，又没有多余的缓存区，则驱逐
          * 
          * 驱逐的是双链表的最后一个
          * 驱逐就是把双链表最后一个移到开头并重新设置tag
          **/
         while(body->next!=NULL){
            body = body->next;
         }
         //判断链表中是不是只有一个结点，是就跳过
         if (body!=cache_lru_row->head){
            body->prior->next=NULL;
            body->next=cache_lru_row->head;
            cache_lru_row->head->prior=body;
         }
         cache_lru_row->head=body;

         //找到在缓存中对应与双链表最后一个的缓存，移除，写入新的缓存。
         struct ROW *one_row;
         for(int j = 0;j < E;j++){
            one_row=&cacheLine[cacheLine_index].E_ROW[j];
            if(body->tagBit == (one_row->tagBit)){
               break;
            }
         }
         body->tagBit=tag;

         one_row->validBit=1;
         one_row->tagBit=tag;

         if(tag_v)printf("miss eviction");
         miss_count++;        //未命中
         eviction_count++;    //驱逐
      end:
         if (tag_v && !(tag_M-1)){printf("\n");}            //不是'M'就要换行
         tag_M--;
      }while(tag_M);  
   }
   //释放分配的空间
   for (int i = 0; i < S; i++ ){
      free(cacheLine[i].head);
      free(cacheLine[i].E_ROW);
   }
   printSummary(hit_count,miss_count,eviction_count);
   return 0;
}

/**
 * 十六进制字符转成十进制整数
 **/
long htoi(char *s){
   long n = 0;
   for(int i = 0;(s[i] >= '0' && s[i] <= '9') || (s[i]>= 'a' && s[i] <= 'f');i++)   {
      if(s[i] > '9'){
         n = 16 * n +(10 + s[i] - 'a');
      }else{
         n = 16 * n + (s[i] - '0');
      }
   }
   return n;
}

