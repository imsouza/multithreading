#include "bankers.h"
#include <stdio.h>
#include <stdlib.h>

int contador = 0;

Banqueiro * 
inicializarBanqueiro (Banqueiro *banqueiro_t) {
	int i, j;

	for (i = 0; i < NUM_PROCESSOS; i++) {
		banqueiro_t->processos_completados[i] = 0;
	}

	for (i = 0; i < NUM_PROCESSOS; i++) {
		for (j = 0; j < NUM_RECURSOS; j++) {
			banqueiro_t->matriz_maxima_para_cada_processo[i][j] = rand() % NUM_RECURSOS;
			banqueiro_t->alocacao_para_cada_processo[i][j] = rand() % NUM_RECURSOS;
		}
	}

	for (i = 0; i < NUM_RECURSOS; i++) {
		banqueiro_t->recursos_disponiveis[i] = rand() % NUM_RECURSOS;

		for (i = 0; i < NUM_PROCESSOS; i++) {
			for (j = 0; j < NUM_RECURSOS; j++) {
				banqueiro_t->recursos_requisitados[i][j] = \
				banqueiro_t->matriz_maxima_para_cada_processo[i][j] - \
				banqueiro_t->alocacao_para_cada_processo[i][j];
			}
		}
	}

	return banqueiro_t;
}


void 
processarBanqueiro (Banqueiro *banqueiro_t) {
	int i, j;

	do {
		printf("\n MAX MATRIZ | \tRECURSOS ALOCADOS\n");
		for (i = 0; i < NUM_PROCESSOS; i++) {
			for (j = 0; j < NUM_RECURSOS; j++) {
				printf("%i ", banqueiro_t->matriz_maxima_para_cada_processo[i][j]);
			}

			printf("\t\t");

			for (j = 0; j < NUM_RECURSOS; j++) {
				printf("%i  ", banqueiro_t->alocacao_para_cada_processo[i][j]);
			}

			putchar('\n');
		}

		banqueiro_t->status_processo = -1;

		for (i = 0; i < NUM_PROCESSOS; i++) {
			if (banqueiro_t->processos_completados[i] == 0) {
				banqueiro_t->status_processo  = i;
				for (j = 0; j < NUM_RECURSOS; j++) {
					if (banqueiro_t->recursos_disponiveis[j] < \
						banqueiro_t->recursos_requisitados[i][j]) {
						banqueiro_t->status_processo = -1;
						break;
					}
				}
			}

			if(banqueiro_t->status_processo != -1) {
				break;
			}
		}

		if (banqueiro_t->status_processo != -1) {
			banqueiro_t->sequencia_de_estados[contador++] = \
			banqueiro_t->status_processo + 1;
			for (j = 0; j < NUM_RECURSOS; j++) {
				banqueiro_t->recursos_disponiveis[j] += \
				banqueiro_t->alocacao_para_cada_processo[banqueiro_t->status_processo][j];
				banqueiro_t->alocacao_para_cada_processo[banqueiro_t->status_processo][j] = 0;
				banqueiro_t->matriz_maxima_para_cada_processo[banqueiro_t->status_processo][j] = 0;
				banqueiro_t->processos_completados[banqueiro_t->status_processo] = 1;
			}
		}
	} while (contador != NUM_PROCESSOS && banqueiro_t->status_processo != -1);

	if (contador == NUM_PROCESSOS) {
		printf("Sequência de Estados Segura: < ");
		for (i = 0; i < NUM_PROCESSOS; i++) {
			printf("%i ", banqueiro_t->sequencia_de_estados[i]);
		}
		printf(">\n");
	}
}
