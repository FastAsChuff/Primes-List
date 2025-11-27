uint32_t *Mairsonsprimesieve(uint32_t n, uint32_t *numprimes) {
  // Returns array of all numprimes primes <= n.
  // Naive implementation by Simon Goater Nov 2025
  // based on algorithm described here https://dl.acm.org/doi/pdf/10.1145/358527.358540
  // 'A Sublinear Additive Sieve For Finding Prime Numbers' by Paul Pritchard Jan 1981.
  // Note: Mairson's isn't the claimed 'Sublinear' algorithm in the paper.
  // ~n(1+8/ln(n))/2 bytes of RAM required. Max 3GB.
  // E.g. 
  // uint32_t numprimes;
  // uint32_t *primes = Mairsonsprimesieve(n, &numprimes);
  // assert(primes);
  if (n < 2) return NULL;
  uint32_t *primes;
  if (n == 2) {
    primes = malloc(sizeof(uint32_t));
    primes[0] = 2;
    *numprimes = 1;
    return primes;
  }
  uint8_t *composites = calloc((n-1)/2,sizeof(uint8_t)); // 3,5,7,9,...
  if (composites == NULL) return NULL;
  uint32_t p = 3;
  uint32_t psqrd = p*p;
  while (psqrd <= n) {
    uint64_t i = psqrd;
    while (i <= n) {
      composites[(i-3)/2] = 1;
      i += 2*p;
    }
    p+=2;
    while (composites[(p-3)/2]) p+=2;
    if (p >> 16) break;
    psqrd = p*p;
  }
  uint32_t primecount = 1 + (n-1)/2;
  for (uint32_t i=3; i<(n-1)/2; i++) primecount -= composites[i];
  primes = malloc(primecount*sizeof(uint32_t));
  if (primes == NULL) {
    free(composites);
    return NULL;
  }
  uint32_t primeix = 1;
  primes[0] = 2;
  for (uint64_t i=3; i<=n; i+=2) if (0 == composites[(i-3)/2]) primes[primeix++] = i;
  *numprimes = primecount;
  free(composites);
  return primes;
}
