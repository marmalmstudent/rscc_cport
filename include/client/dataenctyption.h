#ifndef DATAENCTYPTION_H
#define DATAENCTYPTION_H

#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#define PKT_SIZE 500

typedef struct dataencryption_struct *DataEnctyption;

/** Constructor */
DataEnctyption dataencryption_ctor();
/** Destructor */
void dataencryption_dtor(DataEnctyption obj);

/** The DataEnctyption declaration */
struct dataencryption_struct
{
    char packet[PKT_SIZE];
    int offset;
    mpz_t encrptkey, encrptmod;

    void (* encryptPacketWithKeys)(DataEnctyption self);
};

#endif // DATAENCTYPTION_H
