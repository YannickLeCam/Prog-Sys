#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 100

void shell()
{
    char buffer[BUFFERSIZE];
    printf("%s",fgets(buffer, BUFFERSIZE, stdin));
}


int main(int argc, char *argv[])
{
    shell();
    return 0;
}
