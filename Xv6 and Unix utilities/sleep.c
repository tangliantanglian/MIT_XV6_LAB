/*实现xv6的UNIX程序sleep：
您的sleep应该暂停到用户指定的计时数。一个滴答(tick)是由xv6内核定义的时间概念，
即来自定时器芯片的两个中断之间的时间。您的解决方案应该在文件user/sleep.c中*/

#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char const* argv[]){
    if(argc!=2){
        fprintf(2,"usage:sleep <time>\n");
    }
    sleep(stoi(argv[1]));
    return 0;
}

