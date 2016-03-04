//
//  main.c
//  T3E2
//
//  Created by Abraham Esses on 2/29/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//Escribir un programa en C que cumpla los siguientes requisitos:
//
//--OK Mediante una función se debe comprobar si en el directorio actual existe un subdirectorio llamado “datos”. Si éste no existe, se deberá crear; en caso contrario, se borrarán todos los posibles ficheros que contenga.
//--OK El programa principal se encargará de instalar un gestor para la señal SIGALRM.
//--OK A continuación, mediante un ciclo realizará 5 iteraciones, en cada una de las cuales:
//generará un nombre de fichero distinto (el primero de ellos “a0” , el siguiente “a1” y, así sucesivamente, terminando por “a4”),
//abrirá (para crear) dicho fichero dentro del subdirectorio “datos” (es obligatorio utilizar descriptores de archivos),
//establecerá un temporizador a 3 segundos,
//y, finalmente, comenzará a escribir en el fichero recién creado el carácter “x” mientras una variable global (de nombre “grabar”) valga 1.
//--OK Cuando el temporizador expire, se debe establecer la variable “grabar” a 0 y, luego, cerrar el fichero.
//En resumen, se trata de generar distintos ficheros, y en cada uno de ellos se estará grabando un carácter entre dos señales SIGALRM.
//Por último, listar en pantalla los nombres de los ficheros creados y su tamaño. No usar la función system.
//--OK El número total de ficheros a crear, así como el número de segundos que establece el temporizador, se definirán como macros.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>

#define TEMP 3
#define FICH 5

int grabar = 1;
FILE *fp;
DIR * dir;
char buf[100];
void createDir(); //Crea la carpeta datos
void handleAlrm(int);
void handleAlrm2(int);
struct stat st ={0};
struct stat st2;
struct dirent * file;
void createDir(){
    if (stat("datos", &st) == -1) {
        mkdir("datos", 0755);
    }
    else{
        char nom2[10];
        for (int i=0; i<FICH; i++) {
            sprintf(nom2, "datos/a%d",i);
            remove(nom2);
        }
        
        rmdir("datos");
        
        printf("datos borrado vuelve a ejecutarme!\n");
        exit(-1);
    }
}
void printLS(){
    dir = opendir("./datos");
    printf("Nombre de fichero   Tamaño\n");
    while ((file = readdir(dir)) != NULL) {
        stat(file->d_name,&st2);
        
        printf("    %s              ",file->d_name);
        printf("%lld bytes\n",st2.st_size);
        
    }
    closedir(dir);
}
void handleAlrm(int fd){
    printf("Dejando de escribir...\n");
    grabar = 0;
    fclose(fp);
}
void handleAlrm2(int fd){
    printf("Permitiendo escribir...\n");
    grabar = 1;
}
void writeToFile(int i){
    //void (* sig)(int);
    //sig = signal(SIGALRM,handleAlrm2);
    char nom[15];
    sprintf(nom,"./datos/a%d",i);
    fp = fopen(nom, "w");
    while (grabar) {
        
        fprintf(fp, "x\n");
        sleep(1);
    }
}
int main(int argc, const char * argv[]) {
    createDir();
    //Crear directorios
    for( int i = 0; i < FICH; i++){
        signal(SIGALRM, handleAlrm);
        alarm(TEMP);
        writeToFile(i);
        signal(SIGALRM, handleAlrm2);
        alarm(TEMP);
        sleep(TEMP);
    }
    printLS();
    return 0;
}
