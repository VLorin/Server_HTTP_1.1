#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// pour librequest 
#include "request.h"   

// pour parser 
#include "httpparser.h" 
#include "api.h" 

// pour gérer les erreurs
#include "erreur.h"

//pour nous
#include "reqtarget.h"
#include "finconnexion.h"
#include "conformite.h"



int main(int argc, char *argv[]) {
	message *requete; 
	int res;
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande\n%.*s\n\n",requete->len,requete->buf);  
		
		if (res=parseur(requete->buf,requete->len)) { //syntaxe valide -> interprétation de la requete
			_Token *root;
			root=getRootTree();
			
			if(conformite(root,requete->clientId) == 1){ //vérification sémantique valide, sinon essage d'erreur envoyé et connexion fermée
				//traitement request-target : Bastien
				char *req = requestTarget(root,requete->clientId);
				if(req != (void *)-1){ //request-target valide, sinon message d'erreur envoyé et connexion fermée

					//traitement de la réponse : Vincent
					
					if(fin_connexion(root) == -1){
						requestShutdownSocket(requete->clientId);
					}
				}
			}
			
			purgeTree(root);
		}
		
		else {	//syntaxe invalide -> Erreur 400 : Bad Request
			writeDirectClient(requete->clientId,ERROR400,strlen(ERROR400)); 
			endWriteDirectClient(requete->clientId); 
			requestShutdownSocket(requete->clientId);
		}

		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requete); 
	}
	return (1);
}
