#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef enum {
	false,
	true
} bool;


typedef struct __barreira__ {
	pthread_mutex_t mutex;
	pthread_cond_t wait;
	unsigned int num_threads;
	unsigned int qtde_atual_threads;
	int flag;
	bool status;
} barreira_t;

#define MEM 0xdbcafe

extern unsigned int barreira_init(barreira_t *, unsigned int);
extern unsigned int barreira_destroy(barreira_t *);
extern unsigned int barreira_wait(barreira_t *);
