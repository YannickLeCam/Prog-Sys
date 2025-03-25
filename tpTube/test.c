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
    int fd_fichier;
    fd_fichier=open("test.txt",O_CREAT|O_WRONLY|O_TRUNC,0666);
    pid_t pid;
    pid=fork();
    if(pid==0)
    {
        printf("slt\n");
        printf("Nom fichier : %d \n",fd_fichier);
        dup2(fd_fichier,1);
        execlp("ls","ls","-l",NULL);
        //printf("%d",a);
        exit(0);
    }
    wait(NULL);
    close(fd_fichier);
    return 0;
}