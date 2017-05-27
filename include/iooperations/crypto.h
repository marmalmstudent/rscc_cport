#ifndef CRYPTO_H
#define CRYPTO_H

#include "buffer.h"

typedef struct crypto_struct *Crypto;

/** Constructor */
Crypto crypto_ctor();
/** Destructor */
void crypto_dtor(Crypto obj);

struct crypto_struct
{
    IOBuffer buff;
};
void encryptPacketWithKeys(Crypto self);

#endif // CRYPTO_H
