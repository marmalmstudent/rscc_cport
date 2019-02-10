#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdbool.h>

#include <gmp.h>

#include "buffer.h"

struct rsa_crypto {
  struct data_buffer buff;
  mpz_t encrptkey, encrptmod;
};

/** Constructor */
bool crypto_ctor(struct rsa_crypto *crypto);
/** Destructor */
void crypto_dtor(struct rsa_crypto *crypto);

void decryptPackgetWithKeys(struct rsa_crypto *crypto);
void encryptPacketWithKeys(struct rsa_crypto *crypto);

#endif // CRYPTO_H
