# Check prime numbers in an interval

## Run serial-prime
Compile: `gcc -pthread serial-prime.c -o SerialPrime -lm`
Run: `./SerialPrime <intervalLength>`

## Run parallel-prime
Compile: `gcc -pthread parallel-prime.c -o ParallelPrime -lm`
Run: `./ParallelPrime <intervalLength> <n_threads>`

The algorithm can also be run serially by passing only 1 thread in the parallel-prime argument
