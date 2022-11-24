#include <openssl/provider.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>
#include <openssl/crypto.h>
#include <openssl/params.h>

struct qrng_provider_ctx_st
{
    const OSSL_CORE_HANDLE *core;
    OSSL_LIB_CTX *libctx;
};

typedef struct qrng_provider_ctx_st QRNG_PROVIDER_CTX;
