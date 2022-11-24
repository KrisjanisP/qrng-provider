mkdir -p ./lib
gcc -shared ./src/qrng-provider.c ./src/qrng-provider-rand.c -o ./lib/libqrngprovider.so