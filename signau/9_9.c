#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int i=0;
void coupure(int sig)
{
    printf("Coupure du traitement normale\n");
    i++;
}

void normal(int sig)
{
    printf("traitement normal\n");
}

int main(int argc, char const *argv[])
{
    struct sigaction s;
    struct sigaction s1;

    s.sa_flags=0;
    s.sa_handler=normal;
    sigemptyset(&s.sa_mask);

    s1.sa_flags=0;
    s1.sa_handler=coupure;
    sigemptyset(&s1.sa_mask);

    sigaction(SIGALRM,&s,NULL);
    sigaction(SIGINT,&s1,NULL);
    while (i==0)
    {
        alarm(1);
        pause();
    }
    
    return 0;
}
