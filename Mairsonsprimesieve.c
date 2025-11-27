
uint32_t *Mairsonsprimesieve(uint32_t n, uint32_t *numprimes) {
  // Returns array of all numprimes primes <= n.
  // Naive implementation by Simon Goater Nov 2025
  // based on algorithm described here https://dl.acm.org/doi/pdf/10.1145/358527.358540
  // 'A Sublinear Additive Sieve For Finding Prime Numbers' by Paul Pritchard Jan 1981.
  // Note: Mairson's isn't the claimed 'Sublinear' algorithm in the paper.
  // ~n bytes of RAM required.
  // E.g. 
  // uint32_t numprimes;
  // uint32_t *primes = Mairsonsprimesieve(n, &numprimes);
  // assert(primes);
  if (n < 2) return NULL;
  uint8_t *composites = calloc(n,sizeof(uint8_t));
  if (composites == NULL) return NULL;
  uint32_t p = 2;
  uint32_t psqrd = p*p;
  while (psqrd <= n) {
    uint64_t i = psqrd-1;
    while (i < n) {
      composites[i] = 1;
      i += p;
    }
    p++;
    while (composites[p-1]) p++;
    psqrd = p*p;
    if (psqrd < 2*p + 1) break;
  }
  uint32_t primecount = n-1;
  for (uint32_t i=1; i<n; i++) primecount -= composites[i];
  uint32_t *primes = malloc(primecount*sizeof(uint32_t));
  if (primes == NULL) {
    free(composites);
    return NULL;
  }
  uint32_t primeix = 0;
  for (uint32_t i=1; i<n; i++) if (0 == composites[i]) primes[primeix++] = 1+i;
  *numprimes = primecount;
  free(composites);
  return primes;
}
