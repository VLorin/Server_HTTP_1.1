#ifndef REQTARGET_H
#define REQTARGET_H

#define TAILLE_URI 150
#define TAILLE_MB 1000

/* Fonction qui retourne la request-target en transformant le percent-encoding
- r : requete considérée
- len : longueur de la request-target */
char * percentEncoding(char *r, int len);


/* Fonction qui retourne un entier : -1 en cas d'erreur et longueur sinon 
- r : request-target
- i : indice de début de lecture de request-target
- len : longueur de la request-target
- s : chaine correspondant à l'emplacement du fichier
- j : indice de début de lecture de s
- longueur : nombre de caractères d'un sous-répertoire dans s   */
int compare_annexe(char *r, int i, int len, char *s, int j, int longueur);


/* Fonction qui retourne un entier : -1 en cas d'erreur et la taille du sous-répertoire en question sinon 
- r : request-target
- len : longueur de la request-target
- i : indice de début de lecture de request-target
- k : entier indiquant quel sous-chaine comparée avec la request-target (correspondant à un sous-répertoire contenu dans la chaine correspondant à l'emplacement du fichier  */
int compare(char *r, int len, int i, int k);


/* Fonction qui retourne la request-target en simplifiant le dot dot removal  et -1 sinon
- r : requete considérée
- len : longueur de la request-target  */
void * dotdotremoval(char *r, int len);


/* Fonction qui renvoie la request-target si elle est valide  et ferme la connexion en cas d'erreur
- r : pointeur vers la racine de l'arbre créé par le parseur
- client : identificateur du client pour lui envoyer le message d'erreur  */
void * requestTarget(_Token *r, int client);

#endif

