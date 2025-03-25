/**
*@author:Yannick Le Cam
*@date:08/03/2022
*@brief:Ce programme excutera une commande sur tout les fichiers d'un repertoire et sous-repertoire donné
*@param:char*chemin(repertoire)
*@param:char*commande et arguments
*@return:int
**/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>


/*============================================================================*/
/*FONCT:EXECUTE_COMMANDE*/
/*============================================================================*/
/**
 * @brief Cette fonction execute une commande sur un fichier "regulier"
 * 
 * @param commande[char**] est la commande au complet avec la position du ficher a l'avant dernier place et la commande en a la premiere place du tableau
 * @return ** int renvoie 0 si la commande c'est bien executé et 127 si un probleme avec.
 */
int execute_commande(char**commande)
{

    if(execvp(commande[0],commande)==-1)
    {
        return 127;
    }

    return 0;
}
/*============================================================================*/
/*FONCT:CLOSE_DIR*/
/*============================================================================*/
/**
*@brief Cette fonction permet de fermer un repertoire avec une securité
*@param fd_dir[DIR*] Le chemin de l'emplacement du fichier avec la commande et arguments
*@return : Int 0 si la fonction c'est terminer normalement et 1 si il y a eu un probleme lors de la fermeture du repertoire
*/
int close_dir(DIR* fd_rep)
{
    if(closedir(fd_rep)==-1)
    {
        perror("EXIT_FAILLURE:CLOSE_DIR");
        exit(1);
    }
    return 0;
}
/*============================================================================*/
/*FONCT:EST_REPERTOIRE*/
/*============================================================================*/
/**
*@brief Cette fonction permet de savoir si un fichier est un repertoire ou non
*@param path_file[char*] Le chemin de l'emplacement du fichier
*@return : Int 0 si c'est pas un repertoire 1 si c'est un repertoire (BOOL)
*/
int est_repertoire(char* path_file)
{
    DIR* fd_dir;
    if((fd_dir=opendir(path_file))==NULL)
    {
        return 0;
    }
    close_dir(fd_dir);
    return 1;
}


/*============================================================================*/
/*FONCT:LANCEMENT_PROG*/
/*============================================================================*/
/**
*@brief Cette fonction permet de gerer la recursivité
*@param argv[char**] Le chemin de l'emplacement du fichier avec la commande et arguments
*@param argc[int] le nombre de d'arguments
*@return : Int 0 si la fonction c'est terminer normalement 127 si c'est un probleme de d'exec
*/

int lancement_prog(char**argv,int argc){
    DIR* fd_rep;
    if((fd_rep=opendir(argv[1]))==NULL)
    {
        perror("EXIT_FAILLURE:PRIMITIVE_DIR_OPEN\n");
        exit(EXIT_FAILURE);
    }
    //Le repertoire est ouvert
   
    //Variables
    struct dirent* file;
    pid_t pid ;
    char path_file[PATH_MAX];
    int error_signal;
    int compteur; //Pour savoir si le dossier est vide 
    //End Var

    char* com[argc];//renvoie la commande avec l'avant dernier element qui est le futur fichier
    char** com_rec=argv; //sera la recursive si un le fichier trouver est un repertoire

    //fabrication du argv de commande
    for (int i=0;i<=argc-2;i++)
    {
        com[i]=argv[i+2];
    }
    com[argc-2]=NULL; //futur emplacement du fichier
    com[argc-1]=NULL; //car la commande execpv doit terminer par NULL
    

    //Boucle du parcours du repertoire.
    while((file=readdir(fd_rep))!=NULL)
    {
        
        if(file->d_name[0]=='.')
        {         
            continue;
        }
        compteur++;

        //lancement du processus fils
        if((pid=fork())==-1)
        {
            perror("EXIT_FAILURE:FORK\n");
            return -1;
        }
        //pour eviter la boucle infinie pour la recursivité
        if(file->d_name[0]=='.')
        {         
            continue;
        } 
        //partie de la boucle pour pour le processus fils
        if(pid==0){

            sprintf(path_file,"%s/%s",argv[1],file->d_name);
            
            //annulation du parcours recursif mais detection si c'est un repertoire pour l'ignorer
            if(est_repertoire(path_file)==1)
            {
                //pour ignorer la recursivité pour le 4 b)
                return 0;
                //partie de la boucle en recursif
                com_rec[1]=path_file;
                lancement_prog(com_rec,argc);
                return 0;
            }

            struct stat st;
            
            lstat(path_file,&st);
		//verification si le fichier est un fichier non traité pour l'ignoré sans erreur.
            if(S_ISREG(st.st_mode)==0)
            {
                exit(1);
            }
            
            com[argc-2]=path_file; //on met le nom du fichier a se
            error_signal=execute_commande(com);
            exit(error_signal); 
            return 0;
        }
        int raison;
        if(wait(&raison)==-1){return 1;}
        if(WEXITSTATUS(raison)==127)
        {
            close_dir(fd_rep);
            perror("EXIT_FAILURE:EXEC\n");
            exit(EXIT_FAILURE);
        }
        if(WEXITSTATUS(raison)==1)
        {
            fprintf(stderr,"entree ingnoree\n");
        }
    }

    close_dir(fd_rep);
    return 0;
}
/*============================================================================*/
/*MAIN*/
/*============================================================================*/

int main(int argc,char** argv)
{
    //verification du nombre d'argument
    if (argc<3)
    {
        perror("EXIT_FAILLURE:ARG\n");
        return 2;
    }

    //si n'est pas un repertoire error
    if(est_repertoire(argv[1])==0)
    {
        perror("EXIT_FAILLURE:NOT_A_DIR");
        return 1;
    }
    int res;
    res=lancement_prog(argv,argc);
    
    return res;
}
