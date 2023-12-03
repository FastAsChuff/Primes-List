# Primes-List
Builds an array of prime numbers, then saves them to a file.
This program builds an ascending array in memory of numprimes prime numbers, then saves them to file primes.txt.

There are fast and slow algorithms available. The fast one is the default, but it requires ~10GB RAM to find all 203280221 unsigned 32 bit primes. The slow algorithm can be invoked instead with second argument --slow, which only requires ~1GB RAM, but may take a couple of hours rather than a couple of minutes. 

In general, the slow algorithm requires little more than the space needed for the array, which is numprimes * 4bytes, and the fast algoritm requires around 12 times as much.

Usage:- ./primeslist.bin numprimes [--slow]

e.g. ./primeslist.bin 3

Output to primes.txt

0000000002

0000000003

0000000005
