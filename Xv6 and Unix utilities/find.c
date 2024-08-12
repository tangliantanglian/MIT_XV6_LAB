/*查找目录树中具有特定名称的所有文件*/
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

void find(char* path,const char* filename){
    char buf[512],*p;
    struct stat st;
    struct dirnet de;
    int fd;
    if((fd=open(path,0))<0){
        fprintf(2,"find:cannot open%s\n",path);
        return;
    }
    if((fstat(fd,&st))<0){
        fprintf(2,"find:cannot fstat%s\n",path);
        return;
    }
    if(st.type!=T_DIR){
        fprintf(2,"usage: find <DIRECTORY> <filename>\n");
        return;
    }
    if(strlen(path)+1+DIRSIZ+1>sizeof(buf)){
        fprintf(2,"find:path too long\n");
        return;
    }
    strcpy(buf,path);
    p=buf+strlen(buf);
    *p++='/';
    while(read(fd.&de,sizeof(de))==sizeof(de)){
        if(de.inum==0)continue;
        memmove(p,de.name,DIRSIZ);
        p[DTRSIZ]=0;
        if(stat(buf,&st)<0){
            fprintf(2,"find:cannot stat%s\n",buf);
            continue;
        }
        if(st.type==T_DIT&&strcmp(p,'.')!=0&&strcmp(p,"..")!=0){
            find(buf,filename);
        }else if(strcmp(filename,p)==0){
            fprintf(1,"%s\n",buf);
        }
    }
    close(fd);
}
int main(int argc,char const* argv[]){
    if(argc!=3){
        fprintf(2,"usage:find <path> <filename>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    return 0;
}