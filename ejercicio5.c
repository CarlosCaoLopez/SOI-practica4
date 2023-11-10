/**
 * Entregable 1
 * Autores: Carlos Cao López, Yago Falgueras Casarejos
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


/** Número de hilos del ordenador: 16.
 *  NUMBER_OF_THREADS debe tomar un valor entre 1 y 16
 * */
#define NUMBER_OF_THREADS 16
#define N 1e9


double partial_sum[NUMBER_OF_THREADS];


/**
 * Estructura genérica donde guardar los parámetros que se le pasan a un hilo
 * @param tid Identificador del hilo (usado en impresión de información)
 */
typedef struct {

    pthread_t tid;
    
}args;    


/**
 * Función ejecutada por los hilos creados
 * @param arguments Argumentos iniciales, será una estructura args
 */
void * thread_function(void *arguments);



int main(int argc, char *argv[]){
    args arguments[NUMBER_OF_THREADS];
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i;
    double pi_main=0.0, pi_threads=0.0;

    
    for(i=0; i < NUMBER_OF_THREADS; i++){
        printf("Hilo principal. Creando hilo %d\n", i);
        
        arguments[i].tid = i; /* Guardamos el tid*/

        //Creación del hilo
        status = pthread_create(&threads[i], NULL, thread_function, (void *)&arguments[i]);

        //Comprobación de que se crea correctamente
        if(status != 0){
            printf("Error en la creación del hilo. Status: %d\n", status);
        }
        //sleep(1);
    }
    
    //El hijo principal espera a la terminación de todos los hilos
    for(i=0; i < NUMBER_OF_THREADS; i++){
        //printf("Esperando por la finalización del hilo %d", i);
        pthread_join(threads[i], NULL);
        printf("Suma parcial hilo %2d: %.30f\n", i, partial_sum[i]);
        pi_threads+=partial_sum[i];
    }
    printf("Valor de pi calculado con hilos concurrentes: %.30f\n", pi_threads);

    for(i=0; i<N; i++)
        pi_main += ( 4./((double)(8*i+1)) - 2./((double)(8*i+4)) - 1./((double)(8*i+5)) - 1./((double)(8*i+6)) )/pow(16.,(double)i);
    printf("\nValor de pi calculado con el hilo principal: %.30f\n", pi_main);

    exit(EXIT_SUCCESS);
}



void * thread_function(void *arguments){
    args *aux;
    long int j,k;
    double sum = 0.0;

    aux = (args *) arguments;

    printf("Aquí el hilo %d\n", (int)aux->tid);

    for(k=10*(long int)aux->tid; k<N; k+=(10*NUMBER_OF_THREADS)){
        for(j=0;j<10 && (j+k)<N;j++){
            //sum++;
            sum += ( 4./((double)(8*(j+k)+1)) - 2./((double)(8*(j+k)+4)) - 1./((double)(8*(j+k)+5)) - 1./((double)(8*(j+k)+6)) )/pow(16.,(double)(j+k));
        }
    }

    //Se guarda el valor calculado
    partial_sum[(int)aux->tid]=sum;
   
    pthread_exit(NULL);

}
