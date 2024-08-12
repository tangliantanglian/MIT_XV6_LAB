/*使用管道编写prime sieve(筛选素数)的并发版本
使用pipe和fork来设置管道。第一个进程将数字2到35输入管道
一个管道从其左邻居读取数据，并通过另一个管道向其右邻居写入数据*/

#include "kernel/types.h"
#include "user/user.h"

int left_pipe_first(int left_pipe[2],int* dst){
    if(read(left_pipe[0],dst,sizeof(int))==sizeof(int)){
        printf("prime %d",*dst);
        return 0;
    }
    return -1;
}

void transmit(int left_pipe[2],int right_pipe[2],int first){
    int data;
    while(read(left_pipe[0],&data,sizeof(int))==sizeof(int)){
        if(data%first){
            write(right_pipe[1],&data,sizeof(int));
        }
    }
    close(left_pipe[0]);
    close(right_pipe[1]);
}
void prime(int left_pipe[2]){
    close(left_pipe[1]);
    int first;
    if(left_pipe_first(left_pipe,&first)==0){
        int p[2];
        pipe(p[2]);
        transmit(left_pipe,p,first);
        if(frok()==0){
            prime(p);
        }else{
            close(p[0]);
            wait(0);
        }
    }
    exit(0);
}
int main(int argc,char const* argv[]){
    int p[2];
    pipe(p[2]);
    for(int i=2;i<=35;i++){
        write(p[1],&i,sizeof(int));
    }
    if(fork()==0){
        prime(p);
    }else{
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    return 0;
}