all: finconnexion.o reqtarget.o conformite.o vincent.o
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -o server server.c conformite.o finconnexion.o reqtarget.o vincent.o -lparser -lrequest

vincent.o: vincent.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c vincent.c -Wall -o vincent.o
	
conformite.o: conformite.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c conformite.c -Wall -o conformite.o	
	
finconnexion.o: finconnexion.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c finconnexion.c -Wall -o finconnexion.o
	
reqtarget.o: reqtarget.c
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -g -c reqtarget.c -Wall -o reqtarget.o


	
val:
	valgrind --tool=memcheck --leak-check=full --leak-resolution=low --show-reachable=yes --track-origins=yes ./server 	
	
clean:
	rm *.o