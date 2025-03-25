#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

void fonction(char* arg)
{
    int tube[2];
    int tube2[2];
    int pid;
    int pid1;
    int pid2;
    if(pipe(tube)==-1)
    {
        exit(1);
    }
    if(pipe(tube2)==-1)
    {
        exit(1);
    }
    if((pid=fork())==0)
    {
        close(tube[0]);
        int a=dup2(tube[1],1);
        printf("dup : %d \n",a);
        close(tube[1]);

        execlp("ps","ps","eaux",NULL);
        exit(1);
    }
    if((pid1=fork())==0){
        dup2(tube[0],0);
        close(tube[1]);

        dup2(tube2[1],1);
        close(tube2[0]);
        execlp("grep","grep",arg,NULL);
        exit(1); //pcq error
        
    }
    close(tube[0]);
    close(tube[1]);

    if ((pid2=fork())==0){
        dup2(tube2[0],0);
        close(tube2[1]);
        execlp("wc","wc","-l",NULL);
        exit(1);
    }
    close(tube2[1]);
    close(tube2[0]);
    wait(NULL);
}


int main(int argc, char** argv)
{
    fonction(argv[1]);
    return 0;
}