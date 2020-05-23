#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "httpparser.h"
#include "api.h"
#include "finconnexion.h"


int connexion_close(char *s, int len){
	char connexion[5] = "close";
	int i = 0;
	while(i < len){
		if(s[i] != connexion[i]) return 0;
		else i++;
	}
	return -1;
}


int httpversion(char *s, int len){
	char version[8] = "HTTP/1.0";
	int i = 0;
	while(i < len){
		if(s[i] != version[i]) return 0;
		else i++;
	}
	return -1;
}


int fin_connexion(_Token *root){
	_Token *connect = searchTree(root,"Connection");
	_Token *version = searchTree(root,"HTTP_version");
	if(connect) {
		Lnode* noeud = (Lnode*) connect->node;
		return connexion_close(noeud->value,noeud->len);
	} else {
		Lnode* noeud = (Lnode*) version->node;
		return httpversion(noeud->value,noeud->len);
	}
}
