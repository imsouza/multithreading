#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define reset "\e[0m"

typedef enum {
	false,
	true
} bool;

int intervalo  = 0;
int valorFinal = 0;

bool 
ehPrimo (int valor) {
	if (valor < 2) {
		return false;
	}

	for (int i = 2; i <= valor / 2; i++) {
		if (valor % i == false) {
			return false;
		}
	}

	return true;
}


void *
runner (void* teste) {
	int *inicio = (int*)(teste);
	int fim = 0;

	printf(BYEL"Início da Função %i\n"reset, *inicio);

	if (*inicio + intervalo < valorFinal) {
		fim = *inicio + intervalo;
	} else {
		fim = valorFinal;
	}

	for (int i = *inicio + 1; i <= fim; i++) {
		if (ehPrimo(i)) {
			printf("%i\n", i);
		}
	}

	free(inicio);
}


void 
inicializarThread (int valorFinal, int qtdeThreads) {
	int *inicioIntervalo = malloc(sizeof(int));
	intervalo = valorFinal / qtdeThreads;

	pthread_t tid[qtdeThreads];
	pthread_attr_t attr[qtdeThreads];

	for (int i = 0; i < qtdeThreads; i++) {
		pthread_attr_init(&attr[i]);

		*inicioIntervalo = (i * intervalo);
		printf(BGRN"Início do Intervalo %i\n"reset, *inicioIntervalo);

		pthread_create(&tid[i], &attr[i], runner, inicioIntervalo);

		inicioIntervalo = malloc(sizeof(int));
	}

	pthread_exit (0);
}


int 
main (int argc, char *argv []) {
	int qtdeThreads = 0;

	if (argc != 3) {
		return -1;
	}

	valorFinal = atoi(argv[1]);
	qtdeThreads = atoi(argv[2]);

	inicializarThread(valorFinal, qtdeThreads);

	return 0;
}

