#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void raler (const char * msg){
    perror (msg);
    exit (EXIT_FAILURE);
}

int tab[10];
int i=0;
void b(){
	while(1){
	}
}

void a(){
	b();
}

void sigquit(){
	printf("exit ta mere !\n");
}

//ulimit -c unlimited
//gdb a.out core

//variation=lancer un programme qui ne fini jamais(pour avoir le tps de faire ctrl altgr 8)
//puis gdb après
int main(){
	i=0;
	a();
	return 0;
}
