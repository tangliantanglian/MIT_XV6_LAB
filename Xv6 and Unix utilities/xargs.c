/*编写一个简化版UNIX的xargs程序：
它从标准输入中按行读取，并且为每一行执行一个命令，将行作为参数提供给命令*/

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXSZ 512
//xarg->代表1个参数
enum state{
  S_WAIT,         // 等待参数输入，此状态为初始状态或当前字符为空格
  S_ARG,          // 参数内
  S_ARG_END,      // 参数结束
  S_ARG_LINE_END, // 左侧有参数的换行，例如"arg\n"
  S_LINE_END,     // 左侧为空格的换行，例如"arg  \n""
  S_END           // 结束，EOF
};
enum char_type{
  C_SPACE,
  C_CHAR,
  C_LINE_END
};
enum char_type get_char_type(char c){
    switch(c){
        case ' ':
            return C_SPACE;
        case '\n':
            return C_LINE_END;
        default:
            return C_CHAR;
    }
}
enum state transfrom_state(enum state cur_state,enum char_type cur_type){
    switch (cur_state)
    {
        case S_WAIT:
            if(cur_type==C_SPACE)return S_WAIT;
            if(cur_type==C_LINE_END)return S_LINE_END;
            if(cur_type==C_CHAR)return S_ARG;
            break;
        case S_ARG:
            if(cur_type==C_SPACE)return S_ARG_END;
            if(cur_type==C_LINE_END)return S_ARG_LINE_END;
            if(cur_type==C_CHAR)return S_ARG;
            break;
        case S_ARG_END:
        case S_ARG_LINE_END:
        case S_LINE_END:
            if(cur_type==C_SPACE)return S_WAIT;
            if(cur_type==C_LINE_END)return S_LINE_END;
            if(cur_type==C_CHAR)return S_ARG;
            break;
        default:
            break;
    }
    return S_END;
}
void clear_arg_param(char* x_argv[MAXSZ],int pos){
    for(int i=pos;i<MAXSZ;i++){
        x_argv[i]=0;
    }
}
int main(int argc,char const* argv[]){
    if(argc-1>=MAXSZ){
        fprintf(2,"xargs:too many param\n");
    }
    char lines[MAXSZ];
    char* ptr=lines;
    char* x_argv[MAXSZ]={0};
    for(int i=1;i<argc;i++){
        x_argv[i-1]=argv[i];
    }
    int arg_beg=0,arg_end=0;
    int arg_cur=argc-1;
    enum state st=S_WAIT;
    while(st!=S_END){
        if(read(0,ptr,sizeof(ptr))!=sizeof(char)){
            st=S_END;
        }else{
            st=transfrom_state(x_argv,get_char_type(*ptr));
        }
        if(++arg_end>=MAXSZ){
            fprintf(2,"xargs: arguments too long\n");
            exit(1);
        }
        switch (st)
        {
            case S_WAIT:
                ++arg_beg;
                break;
            case S_ARG_END:
                x_argv[arg_cur++]=&lines[arg_beg];
            case S_LINE_END:
                arg_beg=arg_end;
                *ptr='\0';
                if(fork()==0){
                    exec(argv[1],x_argv);
                }
                arg_cur=argc-1;
                clear_arg_param(x_argv,arg_cur);
                wait(0);
                break;
            default:
                break;
        }
        ++ptr; // 下一个字符的存储位置后移
    }
    exit(0);
}

