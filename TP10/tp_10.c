//NE FONCTIONNE PAS !


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#define SIZE_MAX 1000
int main()
{
    char buffer[SIZE_MAX];
    

    fgets(buffer,SIZE_MAX,stdin);

    int i=0;
    char*text=malloc(sizeof(char)*SIZE_MAX);
    char**list_buff=malloc(sizeof(text));
    int compteur_mot;
    int compteur_lettre;
    while(buffer[i]!='\0')
    {
        if(buffer[i]=='\n')
        {
            compteur_mot++;
            compteur_lettre=0;
        }
        printf("halo \n");
        list_buff[compteur_lettre]=buffer[i];
        printf("halo2 \n");
        compteur_lettre++;
        i++;
    }
    for (int i = 0;i<compteur_mot; i++)
    {
        printf("%s",list_buff[i]);
    }
    return 0;
    

}