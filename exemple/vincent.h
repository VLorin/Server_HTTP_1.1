#ifndef __VINCENT_H__
#define __VINCENT_H__

/* Envoie le fichier filename par bloc(s) de TAILLE octets au clientId,
 * retourne -1 si fichier introuvable, retourne 1 en cas de succés        
 * Cette fonction n'envoie aucune entête !!                               */
int sendFile(int clientId, char *filename);

/* Envoie l'entête HTML + sendFile
 * retourne -1 si fichier introuvable, retourne 1 en cas de succés        */
int sendHTML(int clientId, char * filename);

/* Verifie l'existence du fichier
 * retourne -1 si fichier introuvable, retourne 1 en cas de succés        */
int fileExist(char * filename);


/* Retourne la taille en octet d'un fichier                               */
int contentLength(char * filename);

/* */
int sendRequest(char * host, char * pathname);


#endif