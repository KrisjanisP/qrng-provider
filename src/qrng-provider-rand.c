#include "qrng-provider.h"
#include <string.h>

struct qrng_rand_ctx_st {
    const OSSL_CORE_HANDLE *core;
    CRYPTO_RWLOCK *lock;
};

typedef struct qrng_rand_ctx_st QRNG_RAND_CTX;

static OSSL_FUNC_rand_newctx_fn qrng_rand_newctx;
static OSSL_FUNC_rand_freectx_fn qrng_rand_freectx;
static OSSL_FUNC_rand_instantiate_fn qrng_rand_instantiate;
static OSSL_FUNC_rand_uninstantiate_fn qrng_rand_uninstantiate;
static OSSL_FUNC_rand_generate_fn qrng_rand_generate;
static OSSL_FUNC_rand_enable_locking_fn qrng_rand_enable_locking;
static OSSL_FUNC_rand_lock_fn qrng_rand_lock;
static OSSL_FUNC_rand_unlock_fn qrng_rand_unlock;
static OSSL_FUNC_rand_gettable_ctx_params_fn qrng_rand_gettable_ctx_params;
static OSSL_FUNC_rand_get_ctx_params_fn qrng_rand_get_ctx_params;

static void *
qrng_rand_newctx(void *provctx, void *parent,
                 const OSSL_DISPATCH *parent_calls)
{
    QRNG_PROVIDER_CTX *cprov = provctx;
    QRNG_RAND_CTX *rand = OPENSSL_zalloc(sizeof(QRNG_RAND_CTX));

    if (rand == NULL) return NULL;

    rand->core = cprov->core;
    return rand;
}

static void
qrng_rand_freectx(void *ctx)
{
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL) return;

    CRYPTO_THREAD_lock_free(rand->lock);
    OPENSSL_clear_free(rand, sizeof(QRNG_RAND_CTX));
}

static int
qrng_rand_instantiate(void *ctx, unsigned int strength,
                      int prediction_resistance,
                      const unsigned char *pstr, size_t pstr_len,
                      const OSSL_PARAM params[])
{
    return 1;
}

static int
qrng_rand_uninstantiate(void *ctx)
{
    return 1;
}

static int
qrng_rand_generate(void *ctx, unsigned char *out, size_t outlen,
                   unsigned int strength, int prediction_resistance,
                   const unsigned char *adin, size_t adinlen)
{
    QRNG_RAND_CTX *rand = ctx;

    while (outlen > 0) {
        char b = 'a'; // TODO: replace this lmao
        memcpy(out,&b,sizeof(b));
        outlen -= sizeof(b);
        out += sizeof(b);
    }

    return 1;
}

static int
qrng_rand_enable_locking(void *ctx)
{
    QRNG_RAND_CTX *rand = ctx;

    rand->lock = CRYPTO_THREAD_lock_new();
    return 1;
}

static int
qrng_rand_lock(void *ctx)
{
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL || rand->lock == NULL)
        return 1;
    return CRYPTO_THREAD_write_lock(rand->lock);
}

static void
qrng_rand_unlock(void *ctx)
{
    QRNG_RAND_CTX *rand = ctx;

    if (rand == NULL || rand->lock == NULL)
        return;
    CRYPTO_THREAD_unlock(rand->lock);
}

static const OSSL_PARAM *
qrng_rand_gettable_ctx_params(void *ctx, void *provctx)
{
    static const OSSL_PARAM known_gettable_ctx_params[] = {
        OSSL_PARAM_size_t(OSSL_RAND_PARAM_MAX_REQUEST, NULL),
        OSSL_PARAM_END
    };
    return known_gettable_ctx_params;
}

static int
qrng_rand_get_ctx_params(void *ctx, OSSL_PARAM params[])
{
    OSSL_PARAM *p;

    if (params == NULL)
        return 1;
    TRACE_PARAMS("RAND GET_CTX_PARAMS", params);

    p = OSSL_PARAM_locate(params, OSSL_RAND_PARAM_MAX_REQUEST);
    if (p != NULL && !OSSL_PARAM_set_size_t(p, sizeof(long long)))
        return 0;

    return 1;
}

const OSSL_DISPATCH qrng_rand_functions[] = {
    { OSSL_FUNC_RAND_NEWCTX, (void(*)(void))qrng_rand_newctx },
    { OSSL_FUNC_RAND_FREECTX, (void(*)(void))qrng_rand_freectx },
    { OSSL_FUNC_RAND_INSTANTIATE, (void(*)(void))qrng_rand_instantiate },
    { OSSL_FUNC_RAND_UNINSTANTIATE, (void(*)(void))qrng_rand_uninstantiate },
    { OSSL_FUNC_RAND_GENERATE, (void(*)(void))qrng_rand_generate },
    { OSSL_FUNC_RAND_ENABLE_LOCKING, (void(*)(void))qrng_rand_enable_locking },
    { OSSL_FUNC_RAND_LOCK, (void(*)(void))qrng_rand_lock },
    { OSSL_FUNC_RAND_UNLOCK, (void(*)(void))qrng_rand_unlock },
    { OSSL_FUNC_RAND_GETTABLE_CTX_PARAMS, (void(*)(void))qrng_rand_gettable_ctx_params },
    { OSSL_FUNC_RAND_GET_CTX_PARAMS, (void(*)(void))qrng_rand_get_ctx_params },
    { 0, NULL }
};

