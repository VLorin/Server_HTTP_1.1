#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// for librequest 
#include "request.h"   

// for parser 

// this will declare internal type used by the parser
#include "httpparser.h" 
#include "api.h" 

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"
#define ERROR_404 "HTTP/1.0 404 NOT FOUND\r\n\r\n"


#include "vincent.h"

int main(int argc, char *argv[])
{
	message *requete; 
	int res; 
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  
		if (res=parseur(requete->buf,requete->len)) {
			_Token *r,*tok,*root; 
			if(sendHTML(requete->clientId,"./index.html") == -1){
				printf("ERROR 404\n");
				writeDirectClient(requete->clientId,ERROR_404,strlen(ERROR_404));
			}
			 // TEST 
			
			
			root=getRootTree(); 
			r=searchTree(root,"GET"); 
			tok=r; 
			/*Lnode *node;
			node=(Lnode *)tok->node; 
			printf("%s\n",node->value);*/
			/*
			while (tok) {
				int l; 
				char *s; 
				// node is no longer opaque 
				Lnode *node;
				node=(Lnode *)tok->node; 
				writeDirectClient(requete->clientId,node->value,node->len);
				tok=tok->next; 
			}
			*/
			
		purgeTree(root); 
		} else {
			writeDirectClient(requete->clientId,ERROR,strlen(ERROR)); 
		}
		endWriteDirectClient(requete->clientId); 
		requestShutdownSocket(requete->clientId); 
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
	freeRequest(requete); 
	}
	return (1);
}
