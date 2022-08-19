#include "barreira.h"

unsigned int
barreira_init (barreira_t *barreira, unsigned int qtde_threads) {
	unsigned int status;

	barreira->num_threads = barreira->qtde_atual_threads = qtde_threads;
	barreira->flag = false;

	status = pthread_mutex_init(&barreira->mutex, NULL);
	if (status) { return status; }
	status = pthread_cond_init(&barreira->wait, NULL);
	if (status) { 
		pthread_mutex_destroy(&barreira->mutex);
		return status;
	}

	barreira->status = true;

	return 0;
}


unsigned int
barreira_destroy (barreira_t *barreira) {
	unsigned int status_1;
	unsigned int status_2;

	if (barreira->status != MEM) {
		return EINVAL;
	} 

	if (status_1) { return status_1; }

	if (barreira->qtde_atual_threads != barreira->num_threads) {
		pthread_mutex_unlock(&barreira->mutex);
		return EBUSY;
	}

	barreira->status = false;
	status_1 = pthread_mutex_unlock(&barreira->mutex);
	if (status_1) { return status_1; } 

	status_1 = pthread_mutex_destroy(&barreira->mutex);
	status_2 = pthread_cond_init(&barreira->wait, NULL);

	if (status_1 == 0) {
		return status_1;	
	} else {
		return status_2;
	}
}


unsigned int
barreira_wait (barreira_t *barreira) {
	unsigned int status;
	unsigned int cancel;
	unsigned int temp;
	int flag;

	if (barreira->status != MEM) {
		return EINVAL;
	} 

	status = pthread_mutex_lock(&barreira->mutex);

	if (status) { return status; }

	flag = barreira->flag;

	if (barreira->qtde_atual_threads == 0) {
		barreira->flag = !barreira->flag;
		barreira->qtde_atual_threads = barreira->num_threads;
		status = pthread_cond_broadcast(&barreira->flag);

		if (status == 0) { status = -1; }
	} else {
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel);

		while (flag == barreira->flag) {
			status = pthread_cond_wait(&barreira->wait, &barreira->mutex);
			if (status) { break; }
		}

		pthread_setcancelstate(cancel, &temp);
	}

	pthread_mutex_unlock(&barreira->mutex);
	return status;
}