#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void raler (const char * msg){
    perror (msg);
    exit (EXIT_FAILURE);
}

int main(){
	
}
