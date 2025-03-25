#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

void fonction()
{
    int tube[2];
    int pid;
    int pid2;

    if (pipe(tube)==-1)
    {
        exit(1);
    }

    pid=fork();
    if(pid==0)
    {
        dup2(tube[1],1);
        close(tube[0]);
        close(tube[1]);
        execlp("ls","ls","-l",NULL);
        exit(1);
    }
    pid2=fork();
    if(pid2==0)
    {
        dup2(tube[0],0);
        close(tube[0]);
        close(tube[1]);
        execlp("wc","wc","-l",NULL);
        exit(1);
    }
    wait(NULL);
    close(tube[1]);
    close(tube[0]);
}

int main()
{
    fonction();
    return 0;
}