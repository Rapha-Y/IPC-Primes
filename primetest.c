#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define MAXNUM 2000
#define MAXSTR 50
#define MAXRESULTS 4000000
#define THREAD_NUM 8

char firstHalf[MAXNUM][MAXSTR];
char secondHalf[MAXNUM][MAXSTR];
char strToTest[MAXSTR];
long int result[MAXRESULTS];
int numPrimes = 0;

int isprime(long int value) { 
  long int root;
  long int factor = 2;
  int prime = 1;
  root = sqrtl(value);
  #pragma omp parallel for private(factor) num_threads(THREAD_NUM)
  for(; (factor <= root) && (prime); factor++) {
    prime = fmod((double)value, (double)factor) > 0.0;
  }
  return prime;
}

void quicksort(long int *primes, int first, int last) { 
  int i, j, pivot;
  long int temp;

  if(first < last) { 
    pivot = first;
    i = first;
    j = last;
    while(i < j) { 
      #pragma omp parallel for private(i) num_threads(THREAD_NUM)
      for(; primes[i] <= primes[pivot] && i < last; i++);
      #pragma omp parallel for private(j) num_threads(THREAD_NUM)
      for(; primes[j] > primes[pivot]; j--);
      if(i < j) { 
        temp = primes[i];
        primes[i] = primes[j];
        primes[j] = temp;
      }
    }
    temp = primes[pivot];
    primes[pivot] = primes[j];
    primes[j] = temp;
    quicksort(primes, first, j - 1);
    quicksort(primes, j + 1, last);
  }
}


int main(int argc, char** argv) {
  double start_time = omp_get_wtime();
  FILE *primesFile;
  int i = 0, j = 0, numResults = 0;
  long int primeToTest;
  primesFile = stdin;
  fscanf(primesFile, "%d\n", &numPrimes);
  #pragma omp parallel for private(i) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++)
    fscanf(primesFile, "%s\n", firstHalf[i]);
  #pragma omp parallel for private(i) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++)
    fscanf(primesFile, "%s\n", secondHalf[i]);
  fclose(primesFile);
  #pragma omp parallel for private(i, j) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++)
	  for(j = 0; j < numPrimes; j++) { 
      strcpy(strToTest, firstHalf[i]);
	    strcat(strToTest, secondHalf[j]);
			primeToTest = atol(strToTest);
			if(isprime(primeToTest)) { 
        result[numResults++] = primeToTest;
			}
	  }
  quicksort(result, 0, numResults - 1);
  #pragma omp parallel for private(i) num_threads(THREAD_NUM)
  for(i = 0; i < numResults; i++)
    printf("%ld\n", result[i]);
  double end_time = omp_get_wtime();
  printf("%f\n", end_time - start_time);
  return 0;
}