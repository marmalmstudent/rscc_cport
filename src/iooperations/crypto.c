#include "../../include/iooperations/crypto.h"

#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#include "../../include/dataoperations/intops.h"

static mpz_t encrptkey, encrptmod;
static void initCryptoVals()
{
    mpz_init(encrptkey);
    mpz_set_str(encrptkey, "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457", 16);
    mpz_init(encrptmod);
    mpz_set_str(encrptmod, "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877", 16);
}

Crypto crypto_ctor()
{
    Crypto c = (Crypto)malloc(sizeof(struct crypto_struct));
    c->buff = buffer_ctor(500);
    initCryptoVals(); // make encryption keys
    return c;
}

void crypto_dtor(Crypto obj)
{
    buffer_dtor(obj->buff);
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
}

void encryptPacketWithKeys(Crypto self)
{
    /* copy data to packade of smallest required length */
    int i = self->buff->offset;
    self->buff->offset = 0;
    char dummyPackage[i];
    memcpy(dummyPackage, self->buff->bfr + self->buff->offset, i);

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

    /* write to packet array */
    self->buff->offset = 0;
    wu1b(self->buff->bfr + self->buff->offset++, (unsigned char)wordcount);
    memcpy(self->buff->bfr + self->buff->offset, encryptedPacket, wordcount);
    self->buff->offset += wordcount;
}
