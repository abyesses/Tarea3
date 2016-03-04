//
//  main.c
//  T3E3
//
//  Created by Abraham Esses on 2/29/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//Programe una aplicación en C que permita leer una serie de números enteros y almacenarlos en un array, para luego presentarlos todos en pantalla. El programa ignorará la pulsación CTRL + C.
//
//La cantidad de números a leer se dará como un parámetro en la línea de comandos y han de ser más de 4 y menos de 10.
//Si no se da un parámetro en la línea de comandos, o bien, si el valor numérico de la cadena dada no está entre 5 y 9, ambos inclusive, se dará el correspondiente mensaje de error y finalizará el programa.
//Para indicarle al usuario que introduzca un dato, debe aparecer en pantalla:
//Dato X: _
//Hay que tener en cuenta que este mensaje aparecerá si no han transcurrido 3 segundos desde que se dio el primer mensaje.
//Si han transcurridos 3 segundos desde que se le pidió el primer dato, los próximos mensajes de petición de entrada de datos serán de la forma:
//Dese prisa. Dato X:

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
void handler(int);
int indice = 0;
char * mensaje;
void handler(int fd){
    strcpy(mensaje, "Dese prisa. Dato %d ");
    //printf("Dato %d, Dese prisa...",indice);
}
int main(int argc, const char * argv[]) {
    int * array = (int *)malloc(sizeof(int)*15);
    
    signal(SIGALRM, handler);
    signal(SIGINT, SIG_IGN);
    int c,num = 0, n;
    while ((c = getopt(argc, argv, "n:")) != -1){
        switch (c) {
            case 'n':
                num = atoi(optarg);
                break;
            default:
                abort();
        }
    }
    
    if (num >= 4 && num <= 10)
    {
        //printf("Se solicitarán %d numeros a entrar\n",num);
        mensaje = (char *) malloc(sizeof(char) * 1000);
        strcpy(mensaje, "Dato %d ");
        for (int i = 0; i < num; i++) {
            printf(mensaje,i+1);
            alarm(3);
            scanf("%d",&n);
            if ( !(n >= 5 && n <= 9)) {
                printf("El valor introducido es una amenaza.. (n >=5 y n <=9) \n");
                printf("Me suicido :(\n");
                raise(SIGKILL);
            }
            *(array + i) = (int) malloc(sizeof(int));
            *(array + i) = n;
        }
        free(mensaje);
    }
    else{
        printf("El parámetro dado es menor a 4 o mayor a 10, Me suicido :( .\n");
        raise(SIGKILL);
    }
    
        free(array);
    return 0;
}
