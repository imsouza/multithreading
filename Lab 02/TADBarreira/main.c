#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "barreira.h"

barreira_t bar;

void *runner (void *param)
{
	int id = *(int *) param;
	printf ("Meu id é %d.\n", id);
	pthread_exit (0);
}


int main (int argc, char *argv [])
{
	int i, n;
	pthread_attr_t attr;
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

	barreira_init(&bar, n);

	pthread_attr_init (&attr);
	pthread_t tid [n];

	for (i = 0; i < n; i++)
	{
		pthread_create (&tid [i], &attr, runner, &i);
		barreira_wait(&bar);
	}

	for (i = 0; i < n; i++)
		pthread_join (tid [i], NULL);

	barreira_destroy(&bar);

	return 0;
}
