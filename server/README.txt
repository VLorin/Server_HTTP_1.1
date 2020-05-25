**** launch it with: 

export LD_LIBRARY_PATH=. 
./server

**** Info
le fichier "vincent.c" contient toutes les fonctions relatives à la réponse d'une requête


La vérification de l'unicité des headers (unicite_header) ne fonctionne pas trop
il est donc désactivé temporairement (voir conformite.c ligne 175).

Il y a quelques problèmes d'allocs et free avec la fonction requestTarget, mais 
nous n'arrivons pas à regler le problème.

Si le champ "host" n'est pas spécifié, ou en HTTP/1.1 si le champ ne commence pas par
'w' (pour identifier www.toto.com, par simplicité pour acceder au site toto.com via 
l'adresse IP directement), la page retourné est www.toto.com par défaut.

Le chemin pour accèder aux sites doit-être spécifié dans vincent.c à la ligne 27
"PATH_TO_SITES", de même le site par défaut à la ligne 28 "DEFAULT_SITE".