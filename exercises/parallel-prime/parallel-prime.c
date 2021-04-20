#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include "primechecker.h"

int intervalForEachThread;

int isPrime(int n)
{
  if (n <= 1)
  {
    return 0;
  }

  int nSquareRoot = (int)sqrt(n);

  for (int i = 2; i <= nSquareRoot; i++)
  {
    if (n % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

void *printPrimeNumbers(void *arguments)
{
  int thread_id = *((int *)arguments);

  for (int i = intervalForEachThread * thread_id; i < intervalForEachThread * (thread_id + 1); i++)
  {
    if (isPrime(i))
    {
      printf("%d ", i);
    }
  }
}

int main(int argc, char *argv[])
{
  int intervalLength = atoi(argv[1]);
  int n_threads = atoi(argv[2]);
  int ids[n_threads];
  intervalForEachThread = intervalLength / n_threads;
  pthread_t threads[n_threads];

  struct timeval t1, t2;

  gettimeofday(&t1, NULL);

  for (int i = 0; i < n_threads; i++)
  {
    ids[i] = i;
    pthread_create(&threads[i], NULL, printPrimeNumbers, &ids[i]);
  }

  for (int i = 0; i < n_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&t2, NULL);

  double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
  printf("\n time = %f\n", t_total);

  return 0;
}