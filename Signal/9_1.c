#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
/*
version v7: signal()
version POSIX: sigaction()
*/
void raler (const char * msg){
    perror (msg);
    exit (EXIT_FAILURE);
}

int c=0;
void compteur(){
	c++;
	printf("c: %d\n", c);
}


////////////tester le retour de la primitive signal et sigaction
int main(){
	
	//version v7
	c=0;
	
	if(signal(SIGINT,compteur)==-1){raler("signal");}
	while(c!=5){
		pause();
	}
	return 0;
	/*
	//version POSIX
	c=0;
	struct sigaction s;
	s.sa_handler=compteur;
	s.sa_flags=0;
	sigemptyset(&s.sa_mask);
	if(sigaction(SIGINT, &s, NULL)==-1){raler("sigaction");}
	while(c!=5){
		printf("pause\n");
		pause();
	}
	return 0;
	*/
}		
		
		
