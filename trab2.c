/*
	Implementar/resolver o problema do "Jantar dos Filósofos" em linguagem C, no Linux.
*/
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <math.h>

#define N 5
#define LEFT(indice_filosofo) (indice_filosofo + N - 1) % N
#define RIGHT(indice_filosofo) (indice_filosofo + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

void *philosopher(void* i_temp);
void take_forks(int i);
void put_forks(int i);
void test(int i);
void think();
void eat();
int gera_numero_aleatorio();
void inicializa_semaforos();

int state[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t s[N];
pthread_t tids[N];

int main(void) {
	inicializa_semaforos();
	
	for(int i = 0; i < N; i++) {
		int *i_temp = malloc(sizeof(int));
		*i_temp = i;

		pthread_create(&tids[i], NULL, philosopher, (void *) i_temp);
	}

	for(int i = 0; i < N; i++) {
		pthread_join(tids[i], NULL);
	}
}

void *philosopher(void* i_temp) {

	int i = *((int*) i_temp);
	while(1) {
		think(i);
		take_forks(i);
		eat(i);
		put_forks(i);
	}
}

void take_forks(int i) {
	pthread_mutex_lock(&mutex);
	state[i] = HUNGRY;
	printf("Thread %d faminto\n", i);
	test(i);
	pthread_mutex_unlock(&mutex);
	sem_wait(&s[i]);
}

void put_forks(int i) {
	pthread_mutex_lock(&mutex);
	state[i] = THINKING;
	test(LEFT(i));
	test(RIGHT(i));
	pthread_mutex_unlock(&mutex);
}

void test(int i) {
	if(state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING) {
		state[i] = EATING;
		sem_post(&s[i]);
	}
}


void think(int i) {
	printf("Thread %d pensando\n", i);
	fflush(stdout);
	sleep(gera_numero_aleatorio());
}


void eat(int i) {
	printf("Thread %d comendo\n", i);
	fflush(stdout);
	sleep(gera_numero_aleatorio());
}

/*
	Gera número aleatório de 1 a 10.
*/
int gera_numero_aleatorio() {
	srand(time(NULL));
	return rand() % 10 + 1;
}

void inicializa_semaforos() {
	for(int i = 0; i < N; i++) {
		sem_init(&s[i], 0, 0);
		state[i] = THINKING;
	}
}
