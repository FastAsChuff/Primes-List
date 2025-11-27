#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "/home/simon/Mairsonsprimesieve.c"
// gcc Mairsonsprimesievetest.c -o Mairsonsprimesievetest.bin -O3 -Wall -mssse3
 
int main(int argc, char* argv[]) {
  _Bool validinput = true;
  uint32_t n;
  char *printprimesliteral = "--printprimes";
  _Bool printprimes = false;
  if (argc < 2) {
    validinput = false;
  } else {
    n = atol(argv[1]);
    if (n < 2) validinput = false;
    if (argc > 2) printprimes = (0 == memcmp(printprimesliteral, argv[2], strlen(printprimesliteral)));
  }
  if (!validinput) {
    printf("This program tests a naive implementation of Mairson's algorithm for creating a list of all primes <= n.\nAuthor: Simon Goater Nov 2025\nUsage: %s n [%s]\n2 <= n < 2^32\n~n bytes of RAM required.\n", argv[0], printprimesliteral);
    exit(0);
  }
  uint32_t numprimes;
  uint32_t *primes = Mairsonsprimesieve(n, &numprimes);
  assert(primes);
  if (printprimes) {
    for (uint32_t i=0; i<numprimes; i++) printf("%u ", primes[i]);
    printf("\n");
  }
  uint64_t primesum = 0;
  for(uint32_t i=0; i<numprimes; i++) primesum += primes[i];
  printf("Found %u primes <= %u with sum %lu.\n", numprimes, n, primesum);
}
