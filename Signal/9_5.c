#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void raler (const char * msg){
    perror (msg);
    exit (EXIT_FAILURE);
}

int msg=0;

void traite(){
	msg++;
	printf("msg: %d\n", msg);
}

int main(){
	msg=0;
	pid_t pid;
	if(signal(SIGALRM,traite)==-1){raler("signal");}
	switch(pid=fork()){
		case -1:
			raler("fork");
			break;
		case 0:
			while(1){
				alarm(1);
				pause();
			}
		default:
			alarm(60);//attendre 60 sec
			pause();
			kill(pid, SIGUSR1);
			wait(NULL);
			printf("fini\n");
			break;
	}
	return 0;
}
