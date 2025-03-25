#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int msg=0;
void compteur(int sig)
{
    msg++;
    printf("%d\n",msg);
}


int main()
{
    msg=0;
    struct sigaction s;

    s.sa_flags=0;
    s.sa_handler=compteur;
    sigemptyset(&s.sa_mask);
    if(sigaction(SIGALRM,&s,NULL)==-1){perror("SIG ERROR");}
    pid_t pid;

    switch (pid=fork())
    {
        case 0:
            while (1)
            {
                alarm(1);
                pause();
            }
            break;
    
    default:
        alarm(60);
        pause();
        kill(pid,SIGUSR1);
        wait(NULL);
        break;
    }
    
    




    return 0;
}