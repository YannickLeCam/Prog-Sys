#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

int incr=0;
void fonct(int sig)
{
	if(incr>4){
    printf("%d\n",incr);
	}
}

int main()
{
    signal(SIGINT,fonct);
    while (incr<6)
    {
        pause();
        incr++;
    }
    return 0;
    
        

}
