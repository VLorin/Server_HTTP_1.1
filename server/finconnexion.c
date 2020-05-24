#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "httpparser.h"
#include "api.h"
#include "finconnexion.h"


int connexion_close(char *s, int len){
	char connexion[5] = "close";
	int i = 0;
	int fin_boucle = 1;
	int retour = -1;
	while(i < len && fin_boucle == 1){
		if(s[i] != connexion[i]) fin_boucle = 0;
		else i++;
	}
	if(i == len) retour = 0;
	return retour;
}


int httpversion(char *s, int len){
	char version[8] = "HTTP/1.0";
	int i = 0;
	int fin_boucle = 1;
	int retour = -1;
	while(i < len && fin_boucle == 1){
		if(s[i] != version[i]) fin_boucle = 0;
		else i++;
	}
	if(i == len) retour = 0;
	return retour;
}


int fin_connexion(_Token *root){
	_Token *connect = searchTree(root,"Connection");
	_Token *version = searchTree(root,"HTTP_version");
	int retour;
	if(connect) {
		Lnode* noeud = (Lnode*) connect->node;
		retour = connexion_close(noeud->value,noeud->len);
	} else {
		Lnode* noeud = (Lnode*) version->node;
		retour = httpversion(noeud->value,noeud->len);
	}
	purgeElement(&connect);
	purgeElement(&version);
	return retour;
}
