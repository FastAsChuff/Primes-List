#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

// gcc primeslist.c -o primeslist.bin -O3 -march=native -lm --fast-math -Wall



_Bool save_prime_buffer(char*filename, uint32_t *primes, uint32_t prime_buffer_size) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) return false;
  for (uint32_t i=0; i<prime_buffer_size; i++) {
    fprintf(fp, "%.10u\n", primes[i]);
  }
  fclose(fp);
  return true;
}

uint32_t *build_prime_buffer_slow(uint32_t prime_buffer_size) {
  // Requires ~1GB RAM to find all 203,280,221 unsigned 32 bit primes.
  if (prime_buffer_size > 203280221) return NULL; // Max uint32_t prime is 203280221st prime 4294967291
  // NOTE: Free returned pointer after use.
  uint32_t pixhead=1;
  uint32_t i, j=0, p=3;
  uint32_t *primes = malloc(prime_buffer_size*sizeof(uint32_t));
  if (primes == NULL) return NULL;
  primes[0] = 2;
  while (pixhead < prime_buffer_size) {
    if ((uint64_t)primes[j]*primes[j] <= p) j++;
    for (i=1; i<j; i++) {
      if ((p % primes[i]) == 0) break;
    }
    if (i >= j) {
      primes[pixhead] = p;
      pixhead++;
    }
    p+=2;
  }  
  return primes;
}

uint32_t *build_prime_buffer_fast(uint32_t prime_buffer_size) {
  // Requires ~10GB RAM to find all 203,280,221 unsigned 32 bit primes. (~50s on Xeon E3-1225v2 32GB)
  if (prime_buffer_size > 203280221) return NULL; // Max uint32_t prime is 203280221st prime 4294967291
  if (prime_buffer_size < 180000) return build_prime_buffer_slow(prime_buffer_size);
  uint64_t max, maxbuffersize;
  if (prime_buffer_size >= 6) {
    long double lnn = logl(prime_buffer_size);
    max = ceil(prime_buffer_size*(lnn + logl(lnn)));
  } else {
    max = 11;
  }
  max |= 1;
  if (max > 0xffffffff) max = 0xffffffff;
  maxbuffersize = max/2;
  //printf("max = %lu\n", max);
  //printf("Fast algo. requires approx. = %lu MB RAM\n", (maxbuffersize+prime_buffer_size)*sizeof(uint32_t)/1000000);
  uint32_t num = 3;
  uint32_t *nums = malloc(maxbuffersize*sizeof(uint32_t));
  if (nums == NULL) return build_prime_buffer_slow(prime_buffer_size);
  for (uint64_t i=0; i<maxbuffersize; i++) {
    nums[i] = num;
    num += 2;
  }
  // Estimate of maximum number of sieve primes needed such that sieve_primes[sieve_primes_size-1]^2 > max
  uint32_t sieve_primes_size = 250; 
  uint32_t sieve_primeix = 0;
  uint32_t sieve_prime;
  uint32_t *sieve_primes = build_prime_buffer_slow(sieve_primes_size);
  if (sieve_primes == NULL) return NULL;
  while (sieve_primes[sieve_primes_size-1] <= max/sieve_primes[sieve_primes_size-1]) {
    free(sieve_primes);
    sieve_primes_size *= 2;
    sieve_primes = build_prime_buffer_slow(sieve_primes_size);
    //printf("Extended sieve primes.\n");
  }
  while (sieve_primes[sieve_primeix] <= max/sieve_primes[sieve_primeix]) sieve_primeix++;
  sieve_primes_size = sieve_primeix;
  // Remove sieve primes and all muliples thereof from nums array
  for (uint64_t j=1; j<sieve_primes_size; j++) {
    sieve_prime = sieve_primes[j];
    for (uint64_t i=(sieve_prime - 3)/2; i<maxbuffersize; i+=sieve_prime) nums[i] = 0;
  }  
  uint32_t *primes = malloc(prime_buffer_size*sizeof(uint32_t));
  uint32_t primesix = sieve_primes_size;
  // Copy sieve primes
  memcpy(primes, sieve_primes, sieve_primes_size*sizeof(uint32_t));
  // Copy rest of primes from non-zero elements of nums array
  for (uint64_t i=0; primesix<prime_buffer_size; i++) {
    if (nums[i] != 0) {
      primes[primesix] = nums[i];
      primesix++;
    }
  }
  free(nums);
  free(sieve_primes);
  return primes;
}


int main(int argc, char**argv) {
  char*filename = "primes.txt";
  uint32_t input, prime_buffer_size = 1000;
  uint32_t *primes;
  _Bool slow_algo = false;
  if (argc > 1) {
    input = atoi(argv[1]);
    if (input > 1) prime_buffer_size = input;
  } else {
    printf("This program builds an ascending array in memory of numprimes prime numbers, then saves them to file %s.\n", filename);
    printf("There are fast and slow algorithms available. The fast one is the default, but it requires ~10GB RAM to find all 203280221 unsigned 32 bit primes. The slow algorithm can be invoked instead with second argument --slow, which only requires ~1GB RAM, but may take a couple of hours rather than a couple of minutes. \nIn general, the slow algorithm requires little more than the space needed for the array, which is numprimes * 4bytes, and the fast algoritm requires around 12 times as much.\n");
    printf("Usage:- %s numprimes [--slow]\n", argv[0]);
    printf("e.g. %s 3\n", argv[0]);
    printf("Output to %s\n", filename);
    printf("0000000002\n0000000003\n0000000005\n");
    exit(0);
  }
  if (argc > 2) {
    if (strcmp("--slow", argv[2]) == 0) slow_algo = true;
  }
  if (slow_algo) {
    primes = build_prime_buffer_slow(prime_buffer_size);
  } else {
    primes = build_prime_buffer_fast(prime_buffer_size);
  }
  if (primes == NULL) exit(1);
  uint64_t primes_sum = 0;
  for (uint32_t i=0; i<prime_buffer_size; i++) {
    primes_sum += primes[i];
  }
  printf("Sum of first %u primes is %lu\n", prime_buffer_size, primes_sum);
  printf("Prime number %u is %u\n", prime_buffer_size, primes[prime_buffer_size-1]);
  if(!save_prime_buffer(filename, primes, prime_buffer_size)) {
    fprintf(stderr, "Failed to save prime array!\n");
    free(primes);
    exit(1);
  }
  free(primes);
}
