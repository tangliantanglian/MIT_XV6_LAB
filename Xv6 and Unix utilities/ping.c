/*编写一个使用UNIX系统调用的程序来在两个进程之间“ping-pong”一个字节，
请使用两个管道，每个方向一个。父进程应该向子进程发送一个字节;
子进程应该打印“<pid>: received ping”，其中<pid>是进程ID，
并在管道中写入字节发送给父进程，然后退出;父级应该从读取从子进程而来的字节，
打印“<pid>: received pong”，然后退出。您的解决方案应该在文件user/pingpong.c中*/
#include "kernel/types.h"
#include "user/user.h"

#define RD 0 //读端
#define WR 1 //写端
int main(int argc,char const* argv[]){
    char buf='p';
    int fd_ptoc[2],fd_ctop[2];
    pipe(fd_ptoc);
    pipe(fd_ctop);
    int pid=frok();
    int exit_status=0;
    if(pid<0){
        fprintf(2,"frok error\n");
        close(fd_ptoc[0]);
        close(fd_ptoc[1]);
        close(fd_ctop[0]);
        close(fd_ctop[1]);
        exit(1);
    }else if(pid==0){
        close(fd_ptoc[1]);
        close(fd_ctop[0]);
        if(read(fd_ptoc[0],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"child read error\n");
            exit_status=1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }
        if(write(fd_ctop[1],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"child write error\n");
            exit_status=1;
        }
        close(fd_ptoc[0]);
        close(fd_ctop[1]);
        exit(exit_status);
    }else{
        close(fd_ptoc[0]);
        close(fd_ctop[1]);
        if(write(fd_ptoc[1],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"parent write error\n");
            exit_status=1;
        }
        if(read(fd_ctop[0],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"parent read error\n");
            exit_status=1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }
        close(fd_ptoc[1]);
        close(fd_ctop[0]);
        exit(exit_status);
    }
    return 0;
}