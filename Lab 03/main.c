#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "bankers.h"

#define N_HASHIS 4
#define N_FILOSOFOS 4

sem_t ler_id;
sem_t hashis [N_HASHIS];

void pensar (void)
{
}

void comer (void)
{
}

void *filosofo (void *param)
{
	int id = * (int *) param;
	int aux = 0;

	sem_post (&ler_id);

	while (1)
	{
		pensar ();

		sem_wait (&hashis [id]);
		printf ("Filosofo %d: Peguei hashi %d.\n", id, id);
		if ((id + 1) % N_HASHIS == aux) {
			sem_wait (&hashis [aux]);
		}

		aux++;

		printf ("Filosofo %d: Peguei hashi %d.\n", id, (id + 1) % N_HASHIS);

		comer ();

		sem_post (&hashis [id]);
		sem_post (&hashis [(id + 1) % N_HASHIS]);
		
		aux = 0;
	}

	pthread_exit (0);
}

int main (int argc, char *argv [])
{
	pthread_t tid [N_FILOSOFOS];
	pthread_attr_t attr;
	int i;

	/* Inicializa semaforos */
	sem_init (&ler_id, 0, 0);
	for (i = 0; i < N_HASHIS; i++)
		sem_init (&hashis [i], 0, 1);

	/* Cria filosofos */
	pthread_attr_init (&attr);
	for (i = 0; i < N_FILOSOFOS; i++)
	{
		pthread_create (&tid [i], &attr, filosofo, &i);
		sem_wait (&ler_id);
	}

	/* Deixa filósofos comerem e pensarem */
	
	while (1)
	{
	}

	return 0;
}

