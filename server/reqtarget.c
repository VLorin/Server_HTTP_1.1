#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request.h"
#include "httpparser.h"
#include "api.h"

#include "reqtarget.h"
#include "erreur.h"


char * percentEncoding(char *r, int len){
	char * res = malloc(len+1);
	int i = 0;
	int j = 0;
	int k;
	while(i < len){
		if(r[i] == '%'){
			i++;
			if(r[i] == '%'){
				res[j] = r[i];
				j++;
				i++;
			} else {
				k = 0;
				k = 10*k + r[i] - '0';
				i++;
				k = 10*k + r[i] - '0';
				i++;
				res[j] = (char) k;
				j++;
			}
		}
		else {
			res[j] = r[i];
			j++;
			i++;
		}
	}
	res[i] = '\0';
	return res;
}


int compare_annexe(char *r, int i, int len, char *s, int j, int longueur){
	int n2 = strlen(s);
	int index = 0;
	int z = 0;
	int n1 = i + len;
	int retour = -1;
	while(i < n1 && j < n2 && index < longueur && z == 0){
		if(r[i] != s[j]){ z = 1; }
		else {
			i++;
			j++;
			index += 1;
		}
	}
	if(index == longueur){ retour = longueur; }
	return retour;
}


int compare(char *r, int len, int i, int k){
/* Fait appel a la fonction annexe pour vérifier la conformité de la requete avec le dot dot removal */
	char cwd[200];
	getcwd(cwd,sizeof(cwd));
	int n2 = strlen(cwd);
	int index = n2-1;
	int j;
	int k2 = 0;
	int longueur;
	int fin_boucle = 1;
	int retour = -1;
	while(index > 0 && fin_boucle == 1){
		longueur = 0;
		while(index > 0 && cwd[index] != '/'){
			longueur += 1;
			index -= 1;
		}
		k2 += 1;
		j = index+1;
		if(k2 == k){ 
			retour = compare_annexe(r,i,len,cwd,j,longueur);
			fin_boucle = 0;
		}
		index -= 1;
	}
	return retour;
}


void * dotdotremoval(char *r, int len){
	char * res = malloc(len+1);
	int longueur;
	int i = 0;
	int j = 0;
	int k = 0;
	int fin_boucle = 1;
	void* retour = (void*) -1;
	while(i < len && fin_boucle == 1){
		if(i < len-2 && r[i] == '.' && r[i+1] == '.' && r[i+2] == '/'){
			k += 1;
			i += 3;
		} else if(i < len-1 && r[i] == '.' && r[i+1] == '/'){
			i += 2;
		} else if(k != 0){
			longueur = compare(r,len,i,k);
			if(longueur > 0){
				i += longueur + 1;
				k -= 1;
			} else {
				fin_boucle = 0;
			}
		} else {
			res[j] = r[i];
			j++;
			i++;
		}
	}
	res[i] = '\0';
	if(k == 0) {retour = (void*) res;}
	//free(res);
	return retour;
}


void * requestTarget(_Token *r, int client){
	_Token *orig;
	Lnode *noeud;
	int erreur = 0;
	orig = searchTree(r,"origin_form");
	void* retour = (void*) -1;
	
	if(orig){
		noeud = (Lnode *) orig->node;
		if(noeud->len > TAILLE_URI) erreur = 1;
		else {	char* res = dotdotremoval(percentEncoding(noeud->value,noeud->len),noeud->len);
			if(res != (void*) -1){
				retour = (void*) res;
				
			} 
			else{
				free(res);
				erreur = 2;
			}
		}
	} else { erreur = 3; }

	if(erreur == 1) { 
		writeDirectClient(client,ERROR414,strlen(ERROR414));
		endWriteDirectClient(client);
		requestShutdownSocket(client);
	}
	else if(erreur == 2) {
		writeDirectClient(client,ERROR400,strlen(ERROR400));
		endWriteDirectClient(client);
		requestShutdownSocket(client);
	}
	else if(erreur == 3) {
		writeDirectClient(client,ERROR501,strlen(ERROR501));
		endWriteDirectClient(client);
		requestShutdownSocket(client);	
	}
	purgeElement(&orig);
	return retour;
}

