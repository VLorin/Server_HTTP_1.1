#ifndef ERREUR_H
#define ERREUR_H


/* Erreur 400 : Bad Request 
--> utiliser pour erreur syntaxique et requete invalide */
#define ERROR400 "HTTP/1.0 400 Bad Request\r\n\r\n"


/* Erreur 404 : Not Found
--> utiliser lorsque la ressource demandée n'existe pas */
#define ERROR404 "HTTP/1.0 404 Not Found\r\n\r\n"


/* Erreur 413 : Payload Too Large
--> utiliser lorsque le "message_body" de la requete est trop grande pour notre serveur */
#define ERROR413 "HTTP/1.0 413 Payload Too Large\r\n\r\n"


/* Erreur 414 : URI Too Long
--> utiliser lorsque le "request_target" de la requete est trop grande pour notre serveur */
#define ERROR414 "HTTP/1.0 414 URI Too Long\r\n\r\n"


/* Erreur 501 : Not implemented
--> utiliser pour une request-target qui n'est pas de type origin_form */
#define ERROR501 "HTTP/1.0 501 Not Implemented\r\n\r\n"

#endif
