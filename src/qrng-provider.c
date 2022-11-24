#include <openssl/provider.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>
#include <openssl/crypto.h>
#include <openssl/params.h>

#define QRNG_PROV_NAME "RQRNG provider"

struct QRNG_PROVIDER_CTX {
    const OSSL_CORE_HANDLE *core;
    OSSL_LIB_CTX *libctx;
};



static void qrng_unquery_operation(void *provctx, int operation_id, const OSSL_ALGORITHM *alg)
{
    OPENSSL_free((void *)alg);
}

static const OSSL_ALGORITHM* qrng_query_operation(void* provctx, int operation_id, const int* no_store){
    return NULL;
}

static int qrng_get_params(void *provctx, OSSL_PARAM params[])
{
    OSSL_PARAM *p;

    p = OSSL_PARAM_locate(params, OSSL_PROV_PARAM_NAME);
    if (p != NULL && !OSSL_PARAM_set_utf8_ptr(p, QRNG_PROV_NAME))
        return 0;

    return 1;
}

static const OSSL_DISPATCH qrng_dispatch_table[] = {
    { OSSL_FUNC_PROVIDER_QUERY_OPERATION, (void (*)(void))qrng_query_operation },
    { OSSL_FUNC_PROVIDER_UNQUERY_OPERATION, (void (*)(void))qrng_unquery_operation},
    { OSSL_FUNC_PROVIDER_GET_PARAMS, (void (*)(void))qrng_get_params },
    { 0, NULL }
};


int OSSL_provider_init(const OSSL_CORE_HANDLE* handle,
                       const OSSL_DISPATCH* in, const OSSL_DISPATCH** out,
                       void** provctx)
{
    struct QRNG_PROVIDER_CTX* cprov;

    if ((cprov = OPENSSL_zalloc(sizeof(struct QRNG_PROVIDER_CTX))) == NULL)
        return 0;

    *out = qrng_dispatch_table;

    return 1;
}