#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t sem;

void *runner (void *param)
{
	sem_post(&sem);
	int id = *(int *) param;
	printf ("Meu id é %d.\n", id);
	pthread_exit (0);
}


int main (int argc, char *argv [])
{
	int i, n;
	pthread_attr_t attr;
	sem_init(&sem, 0, 0);

	if (argc != 2)
	{
		fprintf (stderr, "usage: ./main <integer value>\n");
		return -1;
	}

	n = atoi (argv [1]);
	if (n < 0)
	{
		fprintf (stderr, "%d must be >= 0\n", n);
		return -1;
	}

	pthread_attr_init (&attr);
	pthread_t tid [n];

	for (i = 0; i < n; i++)
	{
		pthread_create (&tid [i], &attr, runner, &i);
		sem_wait(&sem);
	}

	for (i = 0; i < n; i++)
		pthread_join (tid [i], NULL);

	sem_destroy(&sem);

	return 0;
}
