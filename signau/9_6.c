#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int i=0;

void compteur(int sig)
{
    i++;
}

int main(int argc, char const *argv[])
{
    struct sigaction s;

    int ajout=0;
    s.sa_flags=0;
    s.sa_handler=compteur;
    sigemptyset(&s.sa_mask);
    sigaction(SIGINT,&s,NULL);
    while(i!=1)
    {
        ajout++;
    }
    printf("%d",ajout);

    return 0;
}
