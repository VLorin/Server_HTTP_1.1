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
	char * res = malloc((len+1)*sizeof(char));
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
	while(i < n1 && j < n2 && index < longueur && z == 0){
		if(r[i] != s[j]){ z = 1; }
		else {
			i++;
			j++;
			index += 1;
		}
	}
	if(index == longueur){ return longueur; }
	else { return -1; }
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
	while(index > 0){
		longueur = 0;
		while(index > 0 && cwd[index] != '/'){
			longueur += 1;
			index -= 1;
		}
		k2 += 1;
		j = index+1;
		if(k2 == k){ return compare_annexe(r,i,len,cwd,j,longueur); }
		index -= 1;
	}
	return -1;
}


void * dotdotremoval(char *r, int len){
	char * res = malloc((len+1)*sizeof(char));
	int longueur;
	int i = 0;
	int j = 0;
	int k = 0;
	while(i < len){
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
				return -1;
			}
		} else {
			res[j] = r[i];
			j++;
			i++;
		}
	}
	res[i] = '\0';
	if(k == 0) {return res;}
	else {return -1;}
}


void * requestTarget(_Token *r, int client){
	_Token *orig;
	Lnode *noeud;
	int erreur = 0;
	orig = searchTree(r,"origin_form");
	
	if(orig){
		noeud = (Lnode *) orig->node;
		if(noeud->len > TAILLE_URI) erreur = 1;
		else {	char * res = dotdotremoval(percentEncoding(noeud->value,noeud->len),noeud->len);
			if(res != -1) return res;
			else erreur = 2;
		}
	} else { erreur = 3; }
	
	if(erreur == 1) { writeDirectClient(client,ERROR414,strlen(ERROR414)); }
	else if(erreur == 2) { writeDirectClient(client,ERROR400,strlen(ERROR400)); }
	else if(erreur == 3) { writeDirectClient(client,ERROR501,strlen(ERROR501)); }
	endWriteDirectClient(client);
	requestShutdownSocket(client);
	return -1;
}

