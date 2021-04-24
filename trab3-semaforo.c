/*
	Código desenvolvido por Rodrigo Rech
	-A partir da thread principal criar 3 threads;

	-Cada thread executa, basicamente, a mesma tarefa que consiste em incrementar uma variável global inicializada com valor zero (0); 
	no entanto, a cada rodada envolvendo todas as threads, cada thread incrementa a variável global uma única vez. Além disso, 
	a alternância entre as threads dá-se sempre em ordem crescente de identificadores. Assumir identificadores das threads 
	incrementais iniciando-se a primeira thread com ID=0. Exemplo: assumindo-se que  as 3 threads tem ids 0, 1 e 2, ter-se-á
	a seguinte apresentação de incremento da variável global:

	-thread 0: global = 1;

	-thead 1: global = 2;

	-thread 2: global = 3;

	-thread 0: global = 4;

	-thread 1: global = 5;
	
	O incremento da variável global finaliza quando atingir um valor pré-estabelecido MAX (definido também pelo usuário).
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

void *contador();

#define VALOR_MAXIMO 13
#define NUM_THREADS 5
#define PROXIMA_THREAD(indice) (indice + 1) % NUM_THREADS

pthread_t tids[NUM_THREADS];
int id_anterior = NUM_THREADS-1;
int  count = 0;
pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
sem_t s;

void main() {
	sem_init(&s, 0, 1);
	for(int i = 0; i < NUM_THREADS; i++) {
		int *i_temp = malloc(sizeof(int));
		*i_temp = i;

		pthread_create(&tids[i], NULL, contador, (void *) i_temp);
	}

	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(tids[i], NULL);
	}	
}

void *contador(void* i_temp) {
	int id = *((int*) i_temp);
	while(1) {
		bool continuarLooping = true;
		while(continuarLooping) {
			if(PROXIMA_THREAD(id_anterior) == id && pthread_mutex_trylock(&count_mutex) == 0) {
				continuarLooping = false;
			}
		}
		if(count != VALOR_MAXIMO) {
			fflush(stdout); 
		      	count++;
		      	printf("thread %d: global = %d\n", id, count); 
		      	id_anterior = id;
		      	sleep(1);
			pthread_mutex_unlock(&count_mutex);
		}
		else {
			exit(EXIT_SUCCESS);
		}
	}
}

