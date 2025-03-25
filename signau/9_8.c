#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void commande (int sig)
{
    pid_t pid;

    switch (pid=fork())
    {
    case 0:
        execlp("ls","ls","-l",NULL);
        break;
    
    default:
        wait(NULL);
        break;
    }
}


int main(int argc, char const *argv[])
{
    struct sigaction s;

    s.sa_flags=0;
    s.sa_handler=commande;
    sigemptyset(&s.sa_mask);

    sigaction(SIGALRM,&s,NULL);
    int i=0;
    while(i<10)
    {
        alarm(1);
        pause();
        i++;
    }
    
    return 0;
}
