mkdir -p ./lib
gcc -shared ./src/qrng-provider-rand.c ./src/qrng-provider.c  -o ./lib/librqrng.so