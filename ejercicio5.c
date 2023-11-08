#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h>

#define NUMBER_OF_THREADS 16
#define N 1e9

typedef struct {

    pthread_t tid;
    
}args;    

void * thread_function(void *arguments);


int main(int argc, char *argv[]){
    args arguments;
    pthread_t threads[NUMBER_OF_THREADS];
    int status, j;
    
    
    for(j=0; j < NUMBER_OF_THREADS; j++){
        printf("Hilo principal. Creando hilo %d\n", j);
        
        arguments.tid = j; /* Guardamos el tid*/
        
        status = pthread_create(&threads[j], NULL, thread_function, (void *)&arguments);
        
        if(status != 0){
            printf("Error en la creación del hilo. Status: %d\n", status);
        }
        sleep(1);
    }
    
    /*El hijo principal espera a la terminación de todos lo hilos */
    for(j=0; j < NUMBER_OF_THREADS; j++){
        pthread_join(threads[i], NULL);
    }
    

    exit(EXIT_SUCCESS);
}

void * thread_function(void *arguments){
    args *aux;
    int j;
    float sum = 0.0;

     aux = (args *) arguments;

    for(){
    
    }
    
   
    pthread_exit(NULL);

}
