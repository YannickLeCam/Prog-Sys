#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
//PRECOND:
//pas avoir d'espace a la fin du message ! 
int compte_mot(char*mot,int size_max)
{
    if(mot[0]=='\n'||(mot[0]==' '&&mot[1]=='\n'))
    {
        return 0;
    }
    int retour=1;
    for (int i = 0; i <size_max; i++)
    {
        if(mot[i]=='\0')
        {
            return retour;
        }
        if((mot[i]==' ')&&(mot[i+1]!='\n'))
        {
            retour++;
        }
    }
}
/*
ON NE PEUT PAS CAR LOCAL VARIABLE
char ** separe_mot(char*str,int nb_mot,int size_max)
{
    char*list_str[nb_mot];
    char*buff;
    int compteur_mot=0;
    int compteur=0;
    for (int i = 0; i < size_max; i++)
    {
        if(str[i]=='\n')
        {
            return list_str;
        }
        if(str[i]==' ')
        {
            compteur=0;
            compteur_mot++;
        }else
        {
            list_str[compteur_mot][compteur]=str[i];
            compteur++;
        }
    }
    return list_str;
    
}
*/
int main()
{
    int SIZE_MAX=100;
    int nb_mots;
    char buff[SIZE_MAX];
    fgets(buff,SIZE_MAX,stdin);
    int i = 0;
    while(buff[i]!='\0')
    {
        printf("%c",buff[i]);
        i++;
    }
    printf("======================================\n");
    nb_mots=compte_mot(buff,SIZE_MAX);
    if(nb_mots==0)
    {
        printf("EXIT_FAILLURE:MOT_VIDE \n");
        return 1;
    }
    printf("Le nombre de mot est : %d \n",nb_mots);

    //char**argv=separe_mot(buff,nb_mots,SIZE_MAX);
    
    char list_str[nb_mots][SIZE_MAX];
    int compteur_mot=0;
    int compteur=0;
    for (int i = 0; i < SIZE_MAX; i++)
    {
        if(buff[i]=='\0'||buff[i]=='\n')
        {
            i=SIZE_MAX;
        }
        if(buff[i]==' ')
        {
            list_str[compteur_mot][compteur]='\0';
            compteur=0;
            compteur_mot++;
        }else
        {
            list_str[compteur_mot][compteur]=buff[i];
            compteur++;
        }
    }

    for (int i = 0; i < nb_mots; i++)
    {
        printf("le mot %d est : %s \n",i+1,list_str[i]);
    }
    printf("===========================\n");
    
    return 0;


}