#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "httpparser.h"
#include "api.h"
#include "erreur.h"
#include "request.h"
#include "conformite.h"
#include "reqtarget.h"


int conformite_methode(_Token* root, int Id){
	_Token *r, *r1, *r2, *r3, *r4;
	int toReturn;
	r = searchTree(root,"method");
	Lnode* noeud = (Lnode*) r->node;
	if((compare_chaine(noeud->value,noeud->len,"GET",3)==0) || (compare_chaine(noeud->value,noeud->len,"HEAD",4)==0)){
		r1 = searchTree(root,"message_body");
		Lnode* n = (Lnode*) r1->node;
		char* val = n->value;
		purgeElement(&r1);
		if(val[0] != '\0'){
			writeDirectClient(Id,ERROR400,strlen(ERROR400));
			endWriteDirectClient(Id);
			requestShutdownSocket(Id);
			toReturn = 0;
		}else{
			toReturn = 1;
		}
	} else if(compare_chaine(noeud->value,noeud->len,"POST",4)==0){
		r2 = searchTree(root,"message_body");
		Lnode* n2 = (Lnode*) r2->node;
		char* val2 = n2->value;
		if(val2[0] == '\0'){
			writeDirectClient(Id,ERROR400,strlen(ERROR400));
			endWriteDirectClient(Id);
			requestShutdownSocket(Id);
			toReturn = 0;
		}else{
			r3 = searchTree(root,"Content_Length");
			if(r3 == NULL){
				r4 = searchTree(root,"Transfer_Encoding");
				if(r4 == NULL){
					writeDirectClient(Id,ERROR400,strlen(ERROR400));
					endWriteDirectClient(Id);
					requestShutdownSocket(Id);
					toReturn = 0;
				}else{
					purgeElement(&r4);
					toReturn = 1;
				}
			} else {
				Lnode* longueur_mb = (Lnode*) r2->node;
				Lnode* longueur_CL = (Lnode*) r3->node;
				int len = longueur_mb->len;
				char* value = longueur_CL->value;
				purgeElement(&r2);
				purgeElement(&r3);
				if(atoi(value) >= TAILLE_MB){
					writeDirectClient(Id,ERROR413,strlen(ERROR413));
					endWriteDirectClient(Id);
					requestShutdownSocket(Id);
				}
				else if(len == atoi(value)){
					toReturn = 1;
				}else{
					writeDirectClient(Id,ERROR400,strlen(ERROR400));
					endWriteDirectClient(Id);
					requestShutdownSocket(Id);
					toReturn = 0;
				}
			}
		}
	} else if((compare_chaine(noeud->value,noeud->len,"PUT",3)!=0) && (compare_chaine(noeud->value,noeud->len,"DELETE",6)!=0) && (compare_chaine(noeud->value,noeud->len,"OPTIONS",7)!=0) && (compare_chaine(noeud->value,noeud->len,"TRACE",5)!=0) && (compare_chaine(noeud->value,noeud->len,"CONNECT",7)!=0) ){
		writeDirectClient(Id,ERROR400,strlen(ERROR400));
		endWriteDirectClient(Id);
		requestShutdownSocket(Id);
		toReturn = 0;
	}
	purgeElement(&r);
	return toReturn;
}


int compare_chaine(char *s1, int l1, char *s2, int l2){
	int i = 0;
	int toReturn;
	int fin_boucle = 1;
	while(i<l1 && i<l2 && fin_boucle == 1){
		if(s1[i] != s2[i]){
			toReturn = -1;
			fin_boucle = 0;
		} else i++;
	}
	if(i == l2) {toReturn = 0;}
	else {toReturn = -1;}
	return toReturn;
}


int conformite_version(_Token* root, int Id){
	_Token *r, *r1;
	int toReturn;
	r = searchTree(root,"HTTP_version");
	Lnode* noeud = (Lnode*) r->node;
	if(compare_chaine(noeud->value,noeud->len,"HTTP/1.0",8)==0){
		toReturn = 1;
	}
	else if(compare_chaine(noeud->value,noeud->len,"HTTP/1.1",8)==0){
		r1 = searchTree(root,"Host");
		if(r1){
			toReturn = 1;
		} else {
			writeDirectClient(Id,ERROR400,strlen(ERROR400));
			endWriteDirectClient(Id);
			requestShutdownSocket(Id);
			toReturn = 0;
		}
	} else {
		writeDirectClient(Id,ERROR400,strlen(ERROR400));
		endWriteDirectClient(Id);
		requestShutdownSocket(Id);
		toReturn = 0;
	}
	purgeElement(&r);
	purgeElement(&r1);
	return toReturn;
}



int unicite_header(_Token* root, int Id){
	_Token *r,*r1;
	r = searchTree(root,"header_field");
	int fin_boucle = 1;
	int retour = 1;
	while (r && fin_boucle == 1) {	
		int l;
		char *s = getElementValue(r->node,&l);
		char *str = (char *) malloc(l);
		int i=0 ;
		while(i<l && s[i] != ':'){
			if(s[i] == '-') { str[i] = '_'; } 
			else { str[i] = s[i]; }
			i++;
		}
		str[i] = '\0';
		r1 = searchTree(root,str);
		free(str);
		if(!r1){
			r=r->next;
			purgeElement(&r1);
		}
		else if(r1->next){
			writeDirectClient(Id,ERROR400,strlen(ERROR400));
			endWriteDirectClient(Id);
			requestShutdownSocket(Id);
			retour = 0;
			fin_boucle = 0;
			purgeElement(&r1);
		} else { 
			r=r->next;
			purgeElement(&r1);
		}
	}
	purgeElement(&r);
	return retour;
}



int conformite(_Token *root, int Id){
	int retour = -1;
	/*
	if(unicite_header(root,Id) == 1 && conformite_version(root,Id) == 1 && conformite_methode(root,Id) == 1) {
		retour = 1;
		
	}*/
	//unicite_header ne fonctionne pas comme il faut...
	if(conformite_version(root,Id) == 1 && conformite_methode(root,Id) == 1) {
		retour = 1;
		
	}
	return retour;
}
