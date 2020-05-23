all: finconnexion.o reqtarget.o conformite.o
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -o server server.c conformite.o finconnexion.o reqtarget.o -lparser -lrequest

conformite.o: conformite.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c conformite.c -Wall -o conformite.o	
	
finconnexion.o: finconnexion.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c finconnexion.c -Wall -o finconnexion.o
	
reqtarget.o: reqtarget.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c reqtarget.c -Wall -o reqtarget.o

