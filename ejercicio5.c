#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h>

/** Número de hilos del ordenador: 16
 *  NUMBER_OF_THREADS debe tomar un valor entre 1 y 16
 * */
#define NUMBER_OF_THREADS 16
#define N 1e9

typedef struct {

    pthread_t tid;
    
}args;    


void * thread_function(void *arguments){
    args *aux;
    int i;
    float sum = 0.0;

     aux = (args *) arguments;

    if((aux->tid) == 0){/*Si estamos en el primer hilo, este cederá la CPU antes de cada iteración*/
        printf("Soy el hilo generoso y mi tid es: %ld\n", aux->tid);
        
        for(i=0; i< N; i++){
        
            sched_yield();
             
            sum += sqrt(i);
        }
        printf("Soy el hilo generoso y he terminado. Mi tid es: %ld\n", aux->tid);
        
    }
    else{ /* Hilos no generosos */
        printf("Soy un hilo NO generoso y mi tid es: %ld\n", aux->tid);
        for(i=0; i< N; i++){
             
            sum += sqrt(i);
        }
        
        printf("Soy un hilo NO generoso y he terminado. Mi tid es: %ld\n", aux->tid);
        
    }
    
   
    pthread_exit(NULL);

}

int main(int argc, char *argv[]){
    args arguments;
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i;
    
    
    for(i=0; i < NUMBER_OF_THREADS; i++){
        printf("Hilo principal. Creando hilo %d\n", i);
        
        arguments.tid = i; /* Guardamos el tid*/
        
        status = pthread_create(&threads[i], NULL, thread_function, (void *)&arguments);
        
        if(status != 0){
            printf("Error en la creación del hilo. Status: %d\n", status);
        }
        sleep(1);
    }
    
    for(i=0; i < NUMBER_OF_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    

    exit(EXIT_SUCCESS);
}
