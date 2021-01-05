/**
 * @file generic.c
 * @brief A program to demonstrate the processing of a value using
 * threads in C with the pthread library 
 * @author Mateus Almeida
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <threads.h>
#include <time.h>
#include <stdatomic.h>

//! Condicional compilation directives
#ifndef LEN
#define LEN 10
#endif
#if LEN <= 0
#undef LEN
#define LEN 10
#endif

/**
 * This structure will be responsible for 
 * storing the processing values
 */
typedef struct {
  atomic_int index;
  atomic_int length;
} thread_arg, *ptr_thread_arg;

//! Initializes threads
pthread_t threads[LEN];

/**
 * This function puts the thread to sleep for a 
 * specified time in milliseconds
 */
void thread_sleep () {
  struct timespec time;
  time.tv_sec = 1;
  thrd_sleep(&time, NULL);
}


/**
 * This function saves the processing of the 
 * thread in a separate file
 * @param i: increment unit
 */
void save_thread_processing (int i) {
  FILE *fp_unity, *fp_processing;

  fp_unity = fopen("unity.txt", "w");
  fp_processing = fopen("processing.txt", "a");

  if (fp_unity == NULL) {
    perror("An error occurred");
    exit(1);
  } else {
    fprintf(fp_unity, "Thread: %li | Value: %i\n", 
    pthread_self(), i);
  }

  if (fp_processing == NULL) {
    perror("An error occurred");
    exit(1);
  } else {
    fprintf(fp_processing, "Thread: %li | Value: %i\n", 
    pthread_self(), i);
  }

  fclose(fp_unity);
}


/**
 * This function deletes the file saved on the next run so 
 * that it can be rewritten
 */
void delete_thread_processing () {
  FILE *fp_processing;
  fp_processing = fopen("processing.txt", "w");
  remove("processing.txt");
  fclose(fp_processing);
}


/**
 * This function exports the program's compilation data
 * @param timespent: stores the time spent in milliseconds
 */
void export_build_data (double timespent) {
  FILE *fp_build;
  fp_build = fopen("build.txt", "w");
  fprintf(fp_build, "FILE: %s\nDATE: %s\nTIME: %s\nPROCESSING TIME: %gms"
  ,__FILE__, __DATE__, __TIME__, timespent);
  fclose(fp_build);
}


/**
 * This function is a pointer that will be 
 * executed on the thread
 * @param *arg: function to start the thread
 */
void *thread_init (void *arg) {
  ptr_thread_arg t_arg = (ptr_thread_arg) arg;

  for (int i = t_arg->index; i <= (t_arg->index + t_arg -> length); i++) {
    if (i >= 500) {
      thread_sleep();
    }

    save_thread_processing(i);
  }
}


/**
 * This function starts processing the thread
 * @param *arg: structure to process the thread
 */
void process_thread (thread_arg *arg) {
  for (int i = 0; i < LEN; i++) {
    arg[i].index = LEN; arg[i].length = LEN;
    pthread_create(&(threads[i]), NULL, thread_init, &(arg[i]));
  }

  for (int i = 0; i < LEN; i++) {
    pthread_join(threads[i], NULL);
  }
}


int main () {
  #ifdef __STDC_NO_THREADS__
  #error Threads are not supported 
  #ifdef __STDC_NO_ATOMICS__
  #error Atomic objects are not supported 
  #endif
  #else 
    delete_thread_processing();

    clock_t Ticks[2];
    Ticks[0] = clock();

    thread_arg arg[2];

    process_thread(arg);

    Ticks[1] = clock();
    double time = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

    printf("Time spent: %g ms. [Enter]", time);
    getchar();
    export_build_data(time);
    exit(1);
  #endif

  return 0;
}
