/**
 * @file factor.c
 * @brief A program that calculates the number of factors of a number using threads
 * @author Mateus Almeida
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <threads.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>

//! Condicional compilation directives
#ifndef THREADS
#define THREADS 5
#endif
#if THREADS <= 0
#undef THREADS
#define THREADS 5
#endif

/**
 * Structure to store information about a single task
 */
typedef struct {
  uint64_t num;
  uint64_t from, to;
  uint64_t result;
} tasks;


/**
 * This function calculates the number of factors
 * @param *arg: pointer to the task structure
 */
void *process (void *arg) {
  tasks *const task_t = arg;
  task_t->result = 0;
  /**
   * Calculates the number of factors and 
   * fills in the results field
   */
  for (uint64_t i = task_t->from; i < task_t -> to; i++) {
    if (task_t->num % i == 0) {
      task_t->result++;
    }
  }

  return NULL;
}


/**
 * This function fills in the task fields
 * @param *task_t: task structure
 * @param num: number of factors
 * @param count: auxiliary variable for counting
 */
void fill_tasks (tasks *task_t, uint64_t num, size_t count) {
  uint64_t init = 1;
  size_t step = num / count;

  for (size_t i = 0; i < count; i++) {
    task_t[i].num  = num;
    task_t[i].from = init;
    task_t[i].to   = init + step;
    init += step;
  }

  task_t[count - 1].to = num + 1;
}


/**
 * This function deallocates instances
 * @param *task_t: task structure
 * @param threads: generated threads
 */
void deallocate_memory (tasks *task_t, pthread_t *threads) {
  free(task_t); 
  free(threads);
  task_t  = NULL;
  threads = NULL;
}


/**
 * This function create the threads
 * @param num: number of factors
 * @param count: auxiliary variable for counting
 */
uint64_t factors (uint64_t num, size_t count) {
  //! Allocate tasks
  tasks *task_t = malloc(count * sizeof(*task_t));
  //! Allocate threads
  pthread_t *threads = malloc(count * sizeof(*threads));

  fill_tasks(task_t, num, count);

  //! Generate threads
  for (size_t i = 0; i < count; i++) {
    pthread_create(threads + i, NULL, process, task_t + i);
  }

  uint64_t result = 0;

  /**
   * Waits for the end of each thread and 
   * accumulates the sum in the result variable
   */
  for (size_t i = 0; i < count; i++) {
    pthread_join(threads[i], NULL);
    result += task_t[i].result;
  }

  deallocate_memory(task_t, threads);

  return result;
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
 * This function exports the result of the program
 * @param input: number of inputs
 */
void export_result (uint64_t input) {
  FILE *fp_result;
  fp_result = fopen("result.txt", "w");
  fprintf(fp_result, "Factors of %"PRIu64": %"PRIu64"\n"
  ,input, factors(input, THREADS));
  fclose(fp_result);
}


int main () {
  #ifdef __STDC_NO_THREADS__
  #error Threads are not supported 
  #else 
    clock_t Ticks[2];
    Ticks[0] = clock();

    uint64_t input = 999;

    export_result(input);

    Ticks[1] = clock();
    double time = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

    printf("Time spent: %g ms. [Enter]", time);
    getchar();
    export_build_data(time);
    exit(1);
  #endif

  return 0;
}
