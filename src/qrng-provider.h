#include <openssl/provider.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>
#include <openssl/crypto.h>
#include <openssl/params.h>
#define DBG(...) /*fprintf(stderr, __VA_ARGS__); fflush(stderr)*/

struct qrng_rand_ctx_st
{
    const OSSL_CORE_HANDLE *core;
    CRYPTO_RWLOCK *lock;
};

typedef struct qrng_rand_ctx_st QRNG_RAND_CTX;

struct qrng_provider_ctx_st
{
    const OSSL_CORE_HANDLE *core;
    OSSL_LIB_CTX *libctx;
};

typedef struct qrng_provider_ctx_st QRNG_PROVIDER_CTX;
