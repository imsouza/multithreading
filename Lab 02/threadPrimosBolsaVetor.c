#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BCYN "\e[1;36m"
#define reset "\e[0m"
#define TAM 1000

typedef enum {
	false,
	true
} bool;


int index_ = 0;
int vetorPrimos[TAM];
int intervalo  = 0;
int valorFinal = 0;
int proximoNumero = 0;


double 
MyClock () {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000000) + tv.tv_usec; 
}


int *
insertionSort (int *V, int n) {
  int atual, j;

  for (int i = 1; i < n; i++) {
    atual = V[i];
    for (j = i; (j > 0) && (atual < V[j - 1]); j--) {
      V[j] = V[j - 1];
    }

    V[j] = atual;
  }

  return V;
}


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

	//printf(BYEL"Início da Função %i\n"reset, *inicio);

	if (*inicio + intervalo < valorFinal) {
		fim = *inicio + intervalo;
	} else {
		fim = valorFinal;
	}
	
	while (proximoNumero < valorFinal) {
		if (ehPrimo(proximoNumero)) {
			vetorPrimos[++index_] = proximoNumero;
			insertionSort(vetorPrimos, index_);
		}
		proximoNumero++;
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
		//printf(BGRN"Início do Intervalo %i\n"reset, *inicioIntervalo);

		pthread_create(&tid[i], &attr[i], runner, inicioIntervalo);
		inicioIntervalo = malloc(sizeof(int));
		pthread_join(tid[i], NULL);
	}
}


int 
main (int argc, char *argv []) {
	system("clear");
	int qtdeThreads = 0;

	if (argc != 3) {
		return -1;
	}

	valorFinal = atoi(argv[1]);
	qtdeThreads = atoi(argv[2]);

	double time = MyClock();

	inicializarThread(valorFinal, qtdeThreads);

	for (int i = 0; i < index_; i++) {
		printf(BCYN"%i "reset, vetorPrimos[i]);
	}

	printf(BGRN"\n\nQuantidade de Primos: %i\n"reset, index_);

	printf(BGRN"Tempo de Execução Geral: %.10lf ms\n\n\n"reset, 
	(MyClock() - time) / CLOCKS_PER_SEC);

	pthread_exit(0);

	return 0;
}

