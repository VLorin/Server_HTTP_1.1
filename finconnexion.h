#ifndef FINCONNEXION_H
#define FINCONNEXION_H

/* Renvoie -1 si s[0..len[ vaut "close" et 0 sinon
- s : chaine de caractères à comparer
- len : longueur de s */
int connexion_close(char *s, int len);


/* Renvoie -1 si s[0..len[ vaut "HTTP/1.0" et 0 sinon
- s : chaine de caractères à comparer
- len : longueur de s */
int httpversion(char *s, int len);


/* Renvoie -1 s'il faut mettre fin à la connexion et 0 sinon
- root : pointeur vers la racine de l'arbre représentant la requete http */
int fin_connexion(_Token *root);

#endif
