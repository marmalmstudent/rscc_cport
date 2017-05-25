#ifndef DATAENCTYPTION_H
#define DATAENCTYPTION_H

#include <stdlib.h>
#include <gmp.h>

#define PKT_SIZE 500

typedef struct dataencryption_struct *DataEnctyption;

/** Constructor */
DataEnctyption dataencryption_ctor();
/** Destructor */
void dataencryption_dtor(DataEnctyption obj);

/** The DataEnctyption declaration */
struct dataencryption_struct
{
    char *packet;
    int offset;
    mpz_t encrptkey, encrptmod;
};

#endif // DATAENCTYPTION_H
