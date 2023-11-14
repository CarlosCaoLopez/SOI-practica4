/**
 * Entregable optativo 1
 * Autores: Carlos Cao López, Yago Falgueras Casarejos
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>


/** Número de procesos del ordenador: 8.
 *  NUMBER_OF_PROCESSES debe tomar un valor entre 1 y 8
 * */
#define NUMBER_OF_PROCESSES 8
#define N 1e9

// Variables estáticas para que los procesos compartan pids
static pid_t hijos[NUMBER_OF_PROCESSES];

/**
 * Función que busca la posición de un hijo dentro del vector de pids de hijos
 * @return La posición del hijo en el vector de hijos
 */
static int search_pid();

/**
 * Función ejecutada por los procesos creados
 */
void process_function(FILE *f);


int main(int argc, char *argv[]){
    //args arguments[NUMBER_OF_PROCESSES];
    //pthread_t threads[NUMBER_OF_PROCESSES];
    int i, hijo;
    pid_t padre=getpid();
    double pi_main=0.0, pi_processes=0.0, partial_sum;
    FILE *f;

    f= fopen("processes_data", "w+");
    if(f==NULL){
        printf("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }


    for(i=0; i < NUMBER_OF_PROCESSES; i++){
        printf("Proceso principal. Creando hijo %d\n", i);

        hijos[i]=fork();  //Creación del proceso hijo
        if(hijos[i]==-1){  //Comprobación de que se crea correctamente
            printf("Error en la ejecución del fork %d\n", i+1);
            exit(EXIT_FAILURE);
        }else if(hijos[i]==0){ //Proceso hijo
            process_function(f);
            break;
        }
    }

    if(getpid()==padre){ //El proceso padre espera a la terminación de todos los hijos
        for(i=0; i < NUMBER_OF_PROCESSES; i++){
            waitpid(hijos[i], NULL, 0);
        }

        rewind(f);
        for(i=0; i < NUMBER_OF_PROCESSES; i++){
            fscanf(f, "%d;%lf\n", &hijo, &partial_sum);
            printf("Suma parcial hijo %d: %.30f\n", hijo, partial_sum);
            pi_processes += partial_sum;
        }

        fclose(f);

        printf("Valor de pi calculado con hijos concurrentes: %.30f\n", pi_processes);

        for(i=0; i<N; i++)
            pi_main += ( 4./((double)(8*i+1)) - 2./((double)(8*i+4)) - 1./((double)(8*i+5)) - 1./((double)(8*i+6)) )/pow(16.,(double)i);
        printf("\nValor de pi calculado con el proceso padre: %.30f\n", pi_main);

        exit(EXIT_SUCCESS);
    }

}


static int search_pid(){
    for(int a=0;a<NUMBER_OF_PROCESSES;a++){
        if((int)hijos[a]==0)
            return a;
    }
    printf("Error en la búsqueda del pid\n");
    exit(EXIT_FAILURE);
}


void process_function(FILE *f){
    int i = search_pid();
    long int j,k;
    double sum = 0.0;

    printf("Aquí el hijo %d\n", i);

    for(k=10*i; k<N; k+=(10*NUMBER_OF_PROCESSES)){
        for(j=0;j<10 && (j+k)<N;j++){
            //sum++;
            sum += ( 4./((double)(8*(j+k)+1)) - 2./((double)(8*(j+k)+4)) - 1./((double)(8*(j+k)+5)) - 1./((double)(8*(j+k)+6)) )/pow(16.,(double)(j+k));
        }
    }

    //Se guarda el valor calculado
    fseek(f, 0, SEEK_END);
    fprintf(f, "%d;%.30f\n", i, sum);
    printf("Termina el hijo %d\n", i);
    fclose(f);
}

