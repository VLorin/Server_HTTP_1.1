/*
Crée par LORIN Vincent P2022
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// for librequest 
#include "request.h"   

// for parser 

// this will declare internal type used by the parser
#include "httpparser.h" 
#include "api.h" 
#include "vincent.h"

#define REPONSE_HTML "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n"
#define TAILLE 1500 // Taille max des buffers d'envoies

int sendHTML(int clientId, char * filename){
    int toReturn = -1;
    if(fileExist(filename) != -1){ //verification de l'existence du fichier
        writeDirectClient(clientId,REPONSE_HTML,strlen(REPONSE_HTML)); // Envoie entête pour reponse HTML
        toReturn = sendFile(clientId, filename); //envoie de la page html
    }
    return toReturn;
}

int sendFile(int clientId, char *filename){ // renvoie 1 si pas d'erreur, -1 sinon
    int toReturn;
    size_t readSize; // nombre de bloc lu par fread
    FILE * file; //pointeur vers filename
    file = fopen(filename,"r");
    char * buffer = (char*) malloc(sizeof(char)*TAILLE); //buffer d'envoie, TAILLE (1500) octets
    strcpy(buffer,"0"); //initialisation
    if(file == NULL){
        perror("SendHTML2 - File not found");
        toReturn = -1; // Erreur 404 ...
    } else {
        toReturn = 1;
        readSize = fread(buffer,TAILLE,1,file);
        do{ //envoie, tant qu'il reste des données
            //printf("%ld\n",readSize);
            writeDirectClient(clientId,buffer,strlen(buffer)); //envoie d'un bloc de donnée
            readSize = fread(buffer,TAILLE,1,file); //fread retourne le nombre de block lue, ici 1 ou 0, 0 = rien ou moins d'un bloc lu
        }while(readSize != 0);
        fclose(file);
    }
    free(buffer);
    return toReturn;
}

int fileExist(char * filename){
    int toReturn = 1;
    FILE * file;
    file = fopen(filename,"r");
    if(file == NULL){
        toReturn = -1;
        perror("SendHTML1 - File not found");
    } else {
        fclose(file);
    }
    return toReturn;
}

int sendRequest(char * hostname, char * pathname){
    int toReturn;
    
    
    
    return toReturn;
}

int contentLength(char * filename){
    int toReturn = 0;
    FILE * fp = fopen(filename,"r");
    fseek(fp, 0L, SEEK_END);
    toReturn = ftell(fp);
    fclose(fp);
    return toReturn;
}