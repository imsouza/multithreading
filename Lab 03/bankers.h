#pragma once

#define QTDE 10

#define NUM_PROCESSOS 4
#define NUM_RECURSOS 2

typedef enum {
	false,
	true
} bool;

typedef struct banqueiro_t {
  int matriz_maxima_para_cada_processo[QTDE][QTDE];
  int recursos_requisitados[QTDE][QTDE];
  int alocacao_para_cada_processo[QTDE][QTDE];
  int recursos_disponiveis[QTDE];
  int processos_completados[QTDE];
  int sequencia_de_estados[QTDE];
  int status_processo;
} Banqueiro;

Banqueiro *inicializarBanqueiro(Banqueiro *);
void processarBanqueiro(Banqueiro *);