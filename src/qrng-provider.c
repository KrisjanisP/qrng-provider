#include "qrng-provider.h"

#define QRNG_PROV_NAME "RQRNG"

extern const OSSL_DISPATCH qrng_rand_functions[];

static const OSSL_ALGORITHM qrng_rands[] = {
    { "CTR-DRBG", "provider=RQRNG", qrng_rand_functions, "remote qunatum random number generation" },
    { NULL, NULL, NULL }
};

static const OSSL_ALGORITHM *qrng_query_operation(void *provctx, int operation_id, int *no_store)
{
    DBG("querying operation started\n");

    switch(operation_id) {
        case OSSL_OP_RAND:
            DBG("querying operation returning\n");
            return qrng_rands;
    }

    DBG("querying operation returning\n");
    return NULL;
}

static void qrng_unquery_operation(void *provctx, int operation_id, const OSSL_ALGORITHM *alg)
{
    DBG("unquerying operation %d started\n", operation_id);

    if(operation_id!=OSSL_OP_RAND)
        OPENSSL_free((void *)alg);

    DBG("unquerying operation returning\n");
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
    {OSSL_FUNC_PROVIDER_QUERY_OPERATION, (void (*)(void))qrng_query_operation},
    {OSSL_FUNC_PROVIDER_UNQUERY_OPERATION, (void (*)(void))qrng_unquery_operation},
    {OSSL_FUNC_PROVIDER_GET_PARAMS, (void (*)(void))qrng_get_params},
    {0, NULL}};

int OSSL_provider_init(const OSSL_CORE_HANDLE *handle,
                       const OSSL_DISPATCH *in, const OSSL_DISPATCH **out,
                       void **provctx)
{
    QRNG_PROVIDER_CTX *cprov;

    if ((cprov = OPENSSL_zalloc(sizeof(QRNG_PROVIDER_CTX))) == NULL)
        return 0;

    *out = qrng_dispatch_table;

    return 1;
}