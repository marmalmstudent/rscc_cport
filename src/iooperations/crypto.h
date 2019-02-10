#ifndef CRYPTO_H
#define CRYPTO_H

#include <gmp.h>

#include "buffer.h"

struct crypto_struct
{
  IOBuffer buff;
  mpz_t encrptkey, encrptmod;
};
typedef struct crypto_struct *Crypto;

/** Constructor */
Crypto crypto_ctor();
/** Destructor */
void crypto_dtor(Crypto obj);

void decryptPackgetWithKeys(Crypto self);
void encryptPacketWithKeys(Crypto self);

#endif // CRYPTO_H
