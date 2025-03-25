//FONCTIONNE COMME LE VEUX LE SUJET
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

int main()
{
    int fd;
    fd=open("/dev/null",O_WRONLY);
    int tube[2];
    if(pipe(tube)==-1)
    {
        printf("EXIT_FAILLURE:PID_ERROR");
        exit(1);
    }
    int pid;
    pid=fork();
    if(pid==0){
        if((dup2(tube[1],1))==-1)
        {
            printf("EXIT_FAILLURE:DUP2_ERROR");
            exit(2);
        }
        close(tube[0]);
        close(tube[1]);
        execlp("ps","ps","eaux",NULL);

        exit(1);
    }
    int pid2=fork();
    if(pid2==0){
        dup2(tube[0],0);
        dup2(fd,1);
        close(tube[1]);
        close(tube[0]);
        execlp("grep","grep","$1",NULL);
        printf("fin exec \n");
    }
    close(tube[1]);
    close(tube[0]);
    wait(NULL);
    
    write(1,"$1 est connecté \n",19);

    return 0;
}