#ifndef CONFORMITE_H
#define CONFORMITE_H

/* Renvoie 0 en cas d'erreur et 1 si la méthode de la requete est conforme, i.e.
- HEAD/GET --> sans message_body
- POST --> avec message_body et (Content_Length indiquant la longueur du message_body  ou  Transfer_Encoding)
*/
int conformite_methode(_Token *root, int Id);


/* Renvoie 1 si la version HTTP donné est conforme avec le format de la requête et 0 sinon 
--> Champ Host obligatoire avec HTTP/1.1 */
int conformite_version(_Token *root, int Id);


/* Renvoie 1 si les headers présents dans la requête sont uniques et 0 sinon */
int unicite_header(_Token* root);


/* Renvoie 0 si les chaines s1 de longueur l1 et s2 de longueur l2 sont identiques, -1 sinon */
int compare_chaine(char *s1, int l1, char *s2, int l2);


/* Renvoie 1 si les trois fonctions précédentes renvoient 1 et -1 sinon */
int conformite(_Token *root, int Id);

#endif
