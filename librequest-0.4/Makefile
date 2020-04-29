
all : sock 

sock : librequest.so main.o 
	gcc -g -o sock main.o -L. -lrequest

%.o: %.c 
	gcc -fPIC -g -I../../abnf -I./api -c $< 

main.o: main.c
	gcc -fPIC -g -I./api -c $< 

clean : 
	rm -rf *.o 

