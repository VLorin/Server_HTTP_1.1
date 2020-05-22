all: server.o finconnexion.o reqtarget.o conformite.o
	gcc -I ../libparser -I ../librequest-0.4/api  -L ../libparser -L ../librequest-0.4 -o server conformite.o server.o finconnexion.o reqtarget.o -lparser -lrequest

server.o : server.c
	gcc -c server.c -o server.o
	
conformite.o : conformite.c
	gcc -c conformite.c -o conformite.o	
	
finconnexion.o : finconnexion.c finconnexion.h
	gcc -c finconnexion.c -o finconnexion.o
	
reqtarget.o : reqtarget.c reqtarget.h
	gcc -c reqtarget.c -o reqtarget.o

