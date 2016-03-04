//
//  main.c
//  T3E1
//
//  Created by Abraham Esses on 2/29/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.

//Programe una aplicación en C que permita crear un hijo, el cual quedará durmiendo durante 10 segundos y al despertar mandará la señal SIGUSR1 a su padre. El padre, al recibir esta señal, mostrará el mensaje:
//
//“Se ha pulsado X veces CTRL+C y se ha pulsado Y veces CTRL+Z. Acabando...”
//
//y a continuación mandará la señal de terminación a todos los procesos de su grupo, incluido el mismo.
//
//Mientras el hijo duerme, el padre mostrará un mensaje cada x segundos. Inicialmente x vale 3. El mensaje será de la forma: “Aparezco cada x segundos” Para controlar los intervalos de tiempo que determinan la frecuencia de aparición de este último mensaje hay que establecer un temporizador que genere al expirar una señal de alarma.
//
//Si se pulsa CTRL+C , se incrementará en 1 el valor de x (el número de segundos al que se inicia el temporizador).
//Si se pulsa CTRL+Z se disminuye en 1 el valor de x , siempre que éste sea mayor que 1. Usar una sola función manejadora (handler) para controlar las señales provocadas por  CTRL + C y  CTRL + Z.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
int x = 3;
int ctrlz = 0;
int ctrlc = 0;
pid_t pid;
void handler (int);
void handler1 (int);
void handlerAlrm (int);
void handler (int fd){
    printf("Se ha pulsado %d veces CTRL+C y se ha pulsado %d veces CTRL+Z. Acabando...\n",ctrlc,ctrlz);
    kill(pid, SIGKILL);
    exit(0);
}
void handlerAlrm (int fd){
    printf("Aparezco cada %d segundos\n",x);
}
void handler1 (int fd){
    
    switch (fd) {
        case SIGINT:
            x++;
            ctrlc++;
            printf("Aumentando 1 ---> x=%d\n",x);
            break;
        case SIGTSTP:
            if(x > 1){
                x--;
                ctrlz++;
                printf("Disminuyendo 1---> x=%d\n",x);
            }
            break;
    }
}
int main(int argc, const char * argv[]) {
    signal(SIGUSR1, handler);
    signal(SIGTSTP,handler1); //Ctrl-z
    signal(SIGINT, handler1);//Ctrl-c
    signal(SIGALRM, handlerAlrm);
    printf("PID padre %d\n",getpid());
    pid = fork();
    if (pid < 0) perror("Fork");
    else if (pid == 0){
        printf("PID hijo %d\n",getpid());
        sleep(10);
        printf("Mandando señal SIGUSR1\n");
        kill(getppid(), SIGUSR1);
    }
    else{
        while (1) {
            alarm(x);
            sleep(2);
        }
        wait(NULL);
    }

    return 0;
}
