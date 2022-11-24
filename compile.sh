mkdir -p ./lib
gcc -shared ./src/qrng-provider.c -o ./lib/libqrngprovider.so