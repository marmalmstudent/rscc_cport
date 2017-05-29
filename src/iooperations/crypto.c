#include "../../include/iooperations/crypto.h"

#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#include "../../include/dataoperations/intops.h"

#define BUFF_SIZE 500

struct crypto_struct
{
    IOBuffer buff;
};

static mpz_t encrptkey, encrptmod;
/** Creates an enctrypted value from arr */
static void getCryptoVal(mpz_t cryptoval, const char *arr);
/** Initialize the encryption key and mod */
static void initCryptoVals();

Crypto crypto_ctor()
{
    Crypto c = (Crypto)malloc(sizeof(struct crypto_struct));
    c->buff = buffer_ctor(BUFF_SIZE);
    initCryptoVals(); // make encryption keys
    return c;
}

void crypto_dtor(Crypto obj)
{
    mpz_clear(encrptkey);
    mpz_clear(encrptmod);
    buffer_dtor(obj->buff);
    obj->buff = NULL;
    free(obj);
}

static void getCryptoVal(mpz_t cryptoval, const char *arr)
{
    mpz_t bigint;
    mpz_init(bigint);
    /* create big integer */
    mpz_import(bigint, sizeof(arr), 1, sizeof(char), 0, 0, arr);
    /* enctrypt using <cryptoval = pow(bigint, encrptkey) % encrptmod> */
    mpz_powm(cryptoval, bigint, encrptkey, encrptmod);
    mpz_clear(bigint);
}

static void initCryptoVals()
{
    mpz_init(encrptkey);
    mpz_set_str(encrptkey, "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457", 16);
    mpz_init(encrptmod);
    mpz_set_str(encrptmod, "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877", 16);
}

void encryptPacketWithKeys(Crypto self)
{
    /* copy data to package of smallest required length */
    char dummyPackage[get_used_size(self->buff)];
    copy_data(self->buff, dummyPackage);
    reset(self->buff);

    /* make the encrypted data */
    mpz_t bigpowmod;
    mpz_init(bigpowmod);
    getCryptoVal(bigpowmod, dummyPackage);

    /* convert enctrypted data to char* */
    long unsigned wordcount;
    char *bfr = (char *)mpz_export(NULL, &wordcount, 1,
                                   sizeof(char), 0, 0, bigpowmod);
    char encryptedPacket[wordcount];
    memcpy(encryptedPacket, bfr, wordcount);
    mpz_clear(bigpowmod);

    /* write to packet array */
    reset(self->buff);
    putUnsigned1Byte(self->buff, (unsigned char)wordcount);
    add_data(self->buff, encryptedPacket, wordcount);
}
