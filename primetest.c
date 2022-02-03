#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define MAXNUM 2000
#define MAXSTR 50
#define MAXRESULTS 4000000
#define THREAD_NUM 4

char firstHalf[MAXNUM][MAXSTR];
char secondHalf[MAXNUM][MAXSTR];
char strToTest[MAXSTR];
long int testList[MAXRESULTS];
int numPrimes = 0;

int isprime(long int value) { 
  int i;
  if(value == 2 || value == 3){return 1;}
  else if(value % 2 == 0 || value == 1){return 0;}
  else{
    for(i = 3; i * i <= value; i+=2){
      if(value % i == 0){return 0;}
    }
    return 1;
  }
}

void quicksort(long int *primes, int first, int last) { 
  int i, j, pivot;
  long int temp;

  if(first < last) { 
    pivot = first;
    i = first;
    j = last;
    //#pragma omp parallel for private(i, j) num_threads(THREAD_NUM)
    for(;i < j;) { 
      //#pragma omp parallel for private(i) num_threads(THREAD_NUM)
      for(; primes[i] <= primes[pivot] && i < last; i++);
      //#pragma omp parallel for private(j) num_threads(THREAD_NUM)
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
  float start;
  start = omp_get_wtime();
  FILE *primesFile;
  int i = 0, j = 0, i_testList = 0;
  long int primeToTest;
  primesFile = stdin;
  fscanf(primesFile, "%d\n", &numPrimes);
  //#pragma omp parallel for private(i) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++)
    fscanf(primesFile, "%s\n", firstHalf[i]);
  //#pragma omp parallel for private(i) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++)
    fscanf(primesFile, "%s\n", secondHalf[i]);
  fclose(primesFile);
  //#pragma omp parallel for private(i, j) num_threads(THREAD_NUM)
  for(i = 0; i < numPrimes; i++){
	  for(j = 0; j < numPrimes; j++) { 
      sprintf(strToTest, "%s%s", firstHalf[i], secondHalf[j]);
      testList[i_testList++] = atol(strToTest);
	  }
  }
  quicksort(testList, 0, i_testList - 1);
  int result[i_testList];
  for(i = 0; i < i_testList; i++){
    result[i] = 0;
  }
  #pragma omp parallel for private(i) num_threads(THREAD_NUM)
     for(i = 0; i < i_testList; i++){
       result[i] = isprime(testList[i]);
     }


  for(i = 0; i < i_testList; i++){
    if(result[i] == 1){
      printf("%ld\n", testList[i]);
    }
  }

  printf("Tempo %f\n", omp_get_wtime() - start);
  return 0;
}
