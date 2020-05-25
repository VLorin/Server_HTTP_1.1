#ifndef __VINCENT_H__
#define __VINCENT_H__

/* Envoie le fichier filename par bloc(s) de TAILLE octets au clientId,
 * retourne -1 si fichier introuvable, retourne 1 en cas de succés        
 * Cette fonction n'envoie aucune entête !!                               */
int sendFile(int clientId, char *filename);



/* Verifie l'existence du fichier
 * retourne -1 si fichier introuvable, retourne 1 en cas de succés        */
int fileExist(char * filename);


/* Retourne la taille en octet d'un fichier                               */
int contentLength(char * filename);

/* Fonction principale de reponse à un GET                                 */
int sendRequest(int clientId, char * host, char * pathname, int verionHTTP);

/* Envoie l'en-tête "Content-Length : <taille du fichier>"                 */
int sendContentLength(int clientId,char * file);

/* Envoie l'en-tête "Content-type: <type> \r\n "                           */
int sendContentType(int clientId, char * pathname);

/*Retourne le champ host */
char * findHost(_Token* root);

/* Retourne 1 si HTTP/1.1, 0 si HTTP/1.0, -1 sinon */
int isHTTP11(_Token* root);

#endif