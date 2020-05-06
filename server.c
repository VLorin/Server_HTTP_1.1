#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// pour librequest
#include "request.h"

// pour parser (this will declare internal type used by the parser)
#include "httpparser.h"
#include "api.h"

// pour nous
#include "erreur.h"


int main(int argc, char *argv[]) {
	message *requete;
	int res;
	while ( 1 ) {
		// on attend la reception d'une requête HTTP de type requete qui pointera vers une ressource allouée par librequest
		if ((requete=getRequest(8080)) == NULL) return -1;

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  
		
		// Analyse syntaxique correcte
		if (res=parseur(requete->buf,requete->len)) {
			_Token *r,*tok,*root;
			root=getRootTree();
			r=searchTree(root,"HTTP_message");
			// fonction  void reponse(_Token *r)   se charge de répondre au client
			reponse(r);
			purgeElement(&r); 
			purgeTree(root); 
			
		// Analyse syntaxique invalide  ->  erreur 400 : Bad Request
		} else {
			writeDirectClient(requete->clientId,ERROR400,strlen(ERROR400));
		}
		
		// Fin de connexion avec le client + libération mémoire
		endWriteDirectClient(requete->clientId); 
		requestShutdownSocket(requete->clientId); 
		freeRequest(requete); 
	}
	return (1);
}
