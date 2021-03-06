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
#define OK_HTML_10 "HTTP/1.0 200 OK\r\n"
#define OK_HTML_11 "HTTP/1.1 200 OK\r\n"
#define TAILLE 1500 // Taille max des buffers d'envoies
#define PATHSIZE 200
#define PATH_TO_SITES "../sites/" 
#define DEFAULT_SITE "www.toto.com"

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
    // send content-length
    if(file == NULL){
        perror("SendHTML2 - File not found");
        toReturn = -1; // Erreur 404 ...
    } else {
        sendContentLength(clientId, filename);
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

int fileExist(char * filename){ //Valide
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

int sendContentLength(int clientId,char * file){
    int toReturn = -1 ;
    char * buffer = (char *)malloc(PATHSIZE);
    int taille = contentLength(file);
    strcpy(buffer, "Content-Length:");
    sprintf(&buffer[strlen(buffer)],"%d",taille);
    strcpy(&buffer[strlen(buffer)],"\r\n");
    printf("%s\n",buffer);
    writeDirectClient(clientId,buffer,strlen(buffer));
    free(buffer);
    return toReturn;
}

int sendRequest(int clientId, char * hostname, char * pathname ){ //Valide
    int toReturn = -1;
    char * path = (char *)malloc(PATHSIZE);
    
    strcpy(path,PATH_TO_SITES);
    if(strlen(hostname) < 1){ //champ host indisponible, donne un site par défaut
        strcpy(&path[strlen(path)],DEFAULT_SITE);
    }else{
        strcpy(&path[strlen(path)],hostname);
    }
    
    path[strlen(path)]='/'; // ../sites/www.sites.com/
    strcpy(&path[strlen(path)],pathname); // ../sites/www.sites.com/image.jpg
    //printf("complete path = %s\n",path);
    if(fileExist(path)){
        sendContentType(clientId,pathname);
        toReturn = sendFile(clientId,path);
    }
    
    
    
    
    
    
    
    free(path);
    return toReturn;
}

int contentLength(char * filename){ //Valide
    int toReturn = 0;
    FILE * fp = fopen(filename,"r");
    fseek(fp, 0L, SEEK_END);
    toReturn = ftell(fp);
    fclose(fp);
    return toReturn;
}

int sendContentType(int clientId, char * pathname){
    int toReturn = -1;
    char * buffer1 = (char *)malloc(PATHSIZE);
    char * buffer2 = (char *)malloc(PATHSIZE);
    char * reponse = (char *)malloc(TAILLE);
    sscanf(pathname,"%[^.].%s",buffer1,buffer2);
    //printf("mime = %s\n",buffer2);
    strcpy(reponse,"Content-Type: ");
    if(strcmp(buffer2,"html") == 0 || strcmp(buffer2,"css") == 0 || strcmp(buffer2,"csv") == 0 || strcmp(buffer2,"html") == 0){ //type text
        strcpy(&reponse[strlen(reponse)],"text\\");
        strcpy(&reponse[strlen(reponse)],buffer2);
    } else if( strcmp(buffer2,"js") == 0){
        strcpy(&reponse[strlen(reponse)],"application/javascript");
    } else if( strcmp(buffer2,"htm") == 0){
        strcpy(&reponse[strlen(reponse)],"text/html");
    } else if( strcmp(buffer2,"abw") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-abiword");
    } else if( strcmp(buffer2,"arc") == 0 || strcmp(buffer2,"bin") == 0){
        strcpy(&reponse[strlen(reponse)],"application/octet-stream");
    } else if( strcmp(buffer2,"avi") == 0){
        strcpy(&reponse[strlen(reponse)],"video/x-msvideo");
    } else if( strcmp(buffer2,"bz") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-bzip");
    } else if( strcmp(buffer2,"bz2") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-bzip2");
    } else if( strcmp(buffer2,"csh") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-csh");
    } else if( strcmp(buffer2,"doc") == 0){
        strcpy(&reponse[strlen(reponse)],"application/msword");
    } else if( strcmp(buffer2,"docx") == 0){
        strcpy(&reponse[strlen(reponse)],"application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    } else if( strcmp(buffer2,"eot") == 0){
        strcpy(&reponse[strlen(reponse)],"application/vnd.ms-fontobject");
    } else if( strcmp(buffer2,"epub") == 0){
        strcpy(&reponse[strlen(reponse)],"application/epub+zip");
    } else if( strcmp(buffer2,"gif") == 0){
        strcpy(&reponse[strlen(reponse)],"image/gif");
    } else if( strcmp(buffer2,"ico") == 0){
        strcpy(&reponse[strlen(reponse)],"image/x-icon");
    } else if( strcmp(buffer2,"ics") == 0){
        strcpy(&reponse[strlen(reponse)],"text/calendar");
    } else if( strcmp(buffer2,"jar") == 0){
        strcpy(&reponse[strlen(reponse)],"application/java-archive");
    } else if( strcmp(buffer2,"jpg") == 0 || strcmp(buffer2,"jpeg") == 0){
        strcpy(&reponse[strlen(reponse)],"image/jpeg");
    } else if( strcmp(buffer2,"json") == 0){
        strcpy(&reponse[strlen(reponse)],"application/json");
    } else if( strcmp(buffer2,"mpeg") == 0){
        strcpy(&reponse[strlen(reponse)],"video/mpeg");
    } else if( strcmp(buffer2,"png") == 0){
        strcpy(&reponse[strlen(reponse)],"image/png");
    } else if( strcmp(buffer2,"pdf") == 0){
        strcpy(&reponse[strlen(reponse)],"application/pdf");
    } else if( strcmp(buffer2,"rar") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-rar-compressed");
    } else if( strcmp(buffer2,"mid") == 0 || strcmp(buffer2,"midi") == 0){
        strcpy(&reponse[strlen(reponse)],"audio/midi");
    } else if( strcmp(buffer2,"rtf") == 0){
        strcpy(&reponse[strlen(reponse)],"application/rtf");
    } else if( strcmp(buffer2,"sh") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-sh");
    } else if( strcmp(buffer2,"svg") == 0){
        strcpy(&reponse[strlen(reponse)],"image/svg+xml");
    } else if( strcmp(buffer2,"swf") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-shockwave-flash");
    } else if( strcmp(buffer2,"tar") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-tar");
    } else if( strcmp(buffer2,"ts") == 0){
        strcpy(&reponse[strlen(reponse)],"application/typescript");
    } else if( strcmp(buffer2,"ttf") == 0){
        strcpy(&reponse[strlen(reponse)],"font/ttf");
    } else if( strcmp(buffer2,"wav") == 0){
        strcpy(&reponse[strlen(reponse)],"audio/x-wav");
    } else if( strcmp(buffer2,"tif") == 0 || strcmp(buffer2,"tiff") == 0){
        strcpy(&reponse[strlen(reponse)],"image/tiff");
    } else if( strcmp(buffer2,"xls") == 0){
        strcpy(&reponse[strlen(reponse)],"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    } else if( strcmp(buffer2,"webp") == 0){
        strcpy(&reponse[strlen(reponse)],"image/webp");
    } else if( strcmp(buffer2,"woff") == 0){
        strcpy(&reponse[strlen(reponse)],"font/woff");
    } else if( strcmp(buffer2,"woff2") == 0){
        strcpy(&reponse[strlen(reponse)],"font/woff2");
    } else if( strcmp(buffer2,"xhtml") == 0){
        strcpy(&reponse[strlen(reponse)],"application/xhtml+xml");
    } else if( strcmp(buffer2,"xml") == 0){
        strcpy(&reponse[strlen(reponse)],"application/xml");
    } else if( strcmp(buffer2,"zip") == 0){
        strcpy(&reponse[strlen(reponse)],"application/zip");
    } else if( strcmp(buffer2,"xul") == 0){
        strcpy(&reponse[strlen(reponse)],"application/vnd.mozilla.xul+xml");
    } else if( strcmp(buffer2,"3gp") == 0){
        strcpy(&reponse[strlen(reponse)],"video/3gpp");
    } else if( strcmp(buffer2,"3g2") == 0){
        strcpy(&reponse[strlen(reponse)],"video/3gpp2");
    } else if( strcmp(buffer2,"7z") == 0){
        strcpy(&reponse[strlen(reponse)],"application/x-7z-compressed");
    }
    strcpy(&reponse[strlen(reponse)],"\r\n");
    writeDirectClient(clientId,reponse,strlen(reponse));
    //printf("%s\n",reponse);
    free(buffer1);
    free(buffer2);
    free(reponse);
    return toReturn;
}
