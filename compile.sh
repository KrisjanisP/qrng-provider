mkdir -p ./lib
gcc -shared -fPIC ./src/qrng-provider-rand.c ./src/qrng-provider.c  -o ./lib/librqrng.so