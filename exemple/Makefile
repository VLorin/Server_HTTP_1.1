all: vincent.o
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -o server server.c vincent.o -lparser -lrequest 

vincent.o: vincent.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c vincent.c -Wall -o vincent.o
	
# -g : pour valgrind

val:
	valgrind --tool=memcheck --leak-check=full --leak-resolution=low --show-reachable=yes --track-origins=yes ./server 