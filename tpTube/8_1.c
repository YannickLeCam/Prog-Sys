#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

/*./a.out </bin/ls >toto*/
int main(int argc, char** argv)
{
    int tube[2];
    if (pipe(tube)==-1)
    {
        exit(1);
    }
    int pid;
    
    if((pid=fork())==-1)
    {
        exit(EXIT_FAILURE);
    }
    int SIZE=1;
    int verif;
    char buf[SIZE];
    switch (pid)
    {
    case 0:
        close(tube[1]);
        while ((verif=read(tube[0],buf,SIZE))!=0)
        {
            write(1,&buf,verif);
        }
        close(tube[0]);
        exit(0);
    
    default:
        close(tube[0]);
        while((verif=read(0,&buf,SIZE))!=0)
        {
            write(tube[1],&buf,verif);
        }
        close(tube[1]);
        wait(NULL);
    }
    return 0;
}