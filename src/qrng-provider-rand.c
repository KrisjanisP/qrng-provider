#include "qrng-provider.h"
#include <string.h>

static void *
qrng_rand_newctx(void *provctx, void *parent,
                 const OSSL_DISPATCH *parent_calls)
{
    DBG("qrng_rand_newctx started\n");
    QRNG_RAND_CTX *rand = OPENSSL_zalloc(sizeof(QRNG_RAND_CTX));

    if (rand == NULL)
    {
        DBG("qrng_rand_newctx returning\n");
        return NULL;
    }

    QRNG_PROVIDER_CTX *cprov = provctx;
    DBG("qrng_rand_newctx reached second state\n");

    DBG("qrng_rand_newctx returning\n");
    return rand;
}

static void
qrng_rand_freectx(void *ctx)
{
    DBG("qrng_rand_freectx started\n");
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL)
    {
        DBG("qrng_rand_newctx returning\n");
        return;
    }

    CRYPTO_THREAD_lock_free(rand->lock);
    OPENSSL_clear_free(rand, sizeof(QRNG_RAND_CTX));
    DBG("qrng_rand_newctx returning\n");
}

static int
qrng_rand_instantiate(void *ctx, unsigned int strength,
                      int prediction_resistance,
                      const unsigned char *pstr, size_t pstr_len,
                      const OSSL_PARAM params[])
{
    DBG("qrng_rand_instantiate started\n");
    DBG("qrng_rand_newctx returning\n");
    return 1;
}

static int
qrng_rand_uninstantiate(void *ctx)
{
    DBG("qrng_rand_uninstantiate started\n");
    DBG("qrng_rand_uninstantiate returning\n");
    return 1;
}

static int
qrng_rand_generate(void *ctx, unsigned char *out, size_t outlen,
                   unsigned int strength, int prediction_resistance,
                   const unsigned char *adin, size_t adinlen)
{
    DBG("qrng_rand_generate started\n");
    QRNG_RAND_CTX *rand = ctx;

    while (outlen > 0)
    {
        char b = 'a'; // TODO: replace this lmao
        memcpy(out, &b, sizeof(b));
        outlen -= sizeof(b);
        out += sizeof(b);
    }

    DBG("qrng_rand_generate returning\n");
    return 1;
}

static int
qrng_rand_enable_locking(void *ctx)
{
    DBG("qrng_rand_enable_locking started\n");
    QRNG_RAND_CTX *rand = ctx;

    rand->lock = CRYPTO_THREAD_lock_new();
    DBG("qrng_rand_enable_locking returning\n");
    return 1;
}

static int
qrng_rand_lock(void *ctx)
{
    DBG("qrng_rand_lock started\n");
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL || rand->lock == NULL)
    {
        DBG("qrng_rand_lock returning\n");
        return 1;
    }

    DBG("qrng_rand_lock returning\n");
    return CRYPTO_THREAD_write_lock(rand->lock);
}

static void qrng_rand_unlock(void *ctx)
{
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL || rand->lock == NULL)
        return;
    CRYPTO_THREAD_unlock(rand->lock);
}

static const OSSL_PARAM *qrng_rand_gettable_ctx_params(void *ctx, void *provctx)
{
    static const OSSL_PARAM known_gettable_ctx_params[] = {
        OSSL_PARAM_size_t(OSSL_RAND_PARAM_MAX_REQUEST, NULL),
        OSSL_PARAM_END};
    return known_gettable_ctx_params;
}

static int qrng_rand_get_ctx_params(void *ctx, OSSL_PARAM params[])
{
    OSSL_PARAM *p;

    if (params == NULL)
        return 1;

    p = OSSL_PARAM_locate(params, OSSL_RAND_PARAM_MAX_REQUEST);
    if (p != NULL && !OSSL_PARAM_set_size_t(p, sizeof(long long)))
        return 0;

    return 1;
}

const OSSL_DISPATCH qrng_rand_functions[] = {
    {OSSL_FUNC_RAND_NEWCTX, (void (*)(void))qrng_rand_newctx},
    {OSSL_FUNC_RAND_FREECTX, (void (*)(void))qrng_rand_freectx},
    {OSSL_FUNC_RAND_INSTANTIATE, (void (*)(void))qrng_rand_instantiate},
    {OSSL_FUNC_RAND_UNINSTANTIATE, (void (*)(void))qrng_rand_uninstantiate},
    {OSSL_FUNC_RAND_GENERATE, (void (*)(void))qrng_rand_generate},
    {OSSL_FUNC_RAND_ENABLE_LOCKING, (void (*)(void))qrng_rand_enable_locking},
    {OSSL_FUNC_RAND_LOCK, (void (*)(void))qrng_rand_lock},
    {OSSL_FUNC_RAND_UNLOCK, (void (*)(void))qrng_rand_unlock},
    {OSSL_FUNC_RAND_GETTABLE_CTX_PARAMS, (void (*)(void))qrng_rand_gettable_ctx_params},
    {OSSL_FUNC_RAND_GET_CTX_PARAMS, (void (*)(void))qrng_rand_get_ctx_params},
    {0, NULL}};
