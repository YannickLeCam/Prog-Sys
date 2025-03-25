//22013477 Joffrey GUILLAUME
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

//fct erreurs
void pfff(char* msg, int err){
	if(err==1){
		//erreur systeme
		perror(msg);
		exit(err);
	}
	else {
		//autre erreur
		fprintf(stderr, "%s\n", msg);
		exit(err);
	}
}

//taille de la commande
int cmdlen(char* cmd[]){
	int i=0;	
	while(cmd[i]!=NULL){
		i++;
	}
	return i;
}

//fonction récursive
void fct(char* path, char* cmd[]){

//ouverture du repertoire
	DIR *dp=opendir(path);
	if(dp==NULL){pfff("opendir",1);}
	
//lecture du repertoire
	struct dirent *d;
	errno=0; //utile pour detecter une erreur après la boucle while ci-dessous
	int raison;
	pid_t pid;
	while ((d=readdir( dp )) != NULL){ //cas où le repertoir est NULL readdir ferme la boucle sans changer errno
		
	//on ne veut pas les fichier "." et ".."
		if( (strcmp(d->d_name,".")!=0)&&(strcmp(d->d_name,"..")!=0) ){
		
		//chemin fichier actuel
			char chemin[PATH_MAX];
			int ret=snprintf(chemin, PATH_MAX, "%s/%s", path, d->d_name);
			if((ret<0)||(ret>=PATH_MAX)){pfff("chemin",2);}
			
		//regarde dans quel cas on est
			struct stat stbuf;
			if(lstat(chemin, &stbuf)==-1){pfff("stat", 1);}
			
		//fichier reg
			if(S_ISREG(stbuf.st_mode)){
			
			//test si c'est un link ou pas
				if(!S_ISLNK(stbuf.st_mode)){
				
				//argument pour execv
					int size=cmdlen(cmd)+2, i=0;
					char *tab[size];
					while(cmd[i]!=NULL){
						tab[i]=cmd[i];
						i++;
					}
					tab[i]=chemin;
					tab[++i]=NULL;
					
				//fork
					switch(pid=fork()){
						case -1:
							pfff("fork",1);
							break;
						case 0:  //fils avec exit 127 en cas d'erreur
							if(execvp(tab[0], tab)==-1){exit(127);}
							exit(0);
						default: //père
							if(wait(&raison)==-1){pfff("wait",1);}
							if(WIFEXITED(raison)&&WEXITSTATUS(raison)==127){
								pfff("execvp",1);
							}
							break;
					}
				}
			}
			
		//sous-repertoire
			else if(S_ISDIR(stbuf.st_mode)){fct(chemin, cmd);}
			
		//est ni un fichier ni un repertoire
			else {fprintf(stderr, "entree ignoree\n");}
		}
	}
	
//test si le readdir c'est bien passer 
	if(errno!=0){pfff("readdir", 1);}
		
//fermeture du repertoire
	if(closedir(dp)==-1){pfff("close",1);}
}

//main
int main(int argc, char* argv[]){

//test le bon nombre d'arguments repertoire + cmd [opt]
	if(argc<3){
		fprintf(stderr, "Usage: %s \n", argv[0]);
		exit(2);
	}
	
//regarder dans quel cas on est
	char *path=argv[1];
	struct stat stbuf;
	if(stat(path, &stbuf)==-1){pfff("stat",1);}
	
//cas où argv viable: un repertoire ou un fichier
	if(S_ISDIR (stbuf.st_mode)||S_ISREG(stbuf.st_mode)){fct(path,argv+2);}
	
//cas où argv non viable: ni un fichier ni un répertoire
	else{pfff("notreg||notdir", 1);}
	
	return 0;
}
