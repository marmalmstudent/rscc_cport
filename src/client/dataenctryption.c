#include "../../include/client/dataenctyption.h"

static void addByte(DataEnctyption self, int i);
static void add4ByteInt(DataEnctyption self, int i);
static void addString(DataEnctyption self, char *s);
static void addBytes(DataEnctyption self, char *data, int len);
static void getBytes(DataEnctyption self, char *data, int len);
static void encryptPacketWithKeys(DataEnctyption self);

DataEnctyption dataencryption_ctor()
{
    DataEnctyption d = (DataEnctyption)malloc(sizeof(struct dataencryption_struct));
    memset(d->packet, 0, PKT_SIZE);

    mpz_init(d->encrptkey);
    mpz_set_str(d->encrptkey, "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457", 16);
    mpz_init(d->encrptmod);
    mpz_set_str(d->encrptmod, "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877", 16);

    d->encryptPacketWithKeys = &encryptPacketWithKeys;

    return d;
}

void dataencryption_dtor(DataEnctyption obj)
{
    free(obj);
}

static void addByte(DataEnctyption self, int i)
{
    self->packet[self->offset++] = (char) i;
}

static void add4ByteInt(DataEnctyption self, int i)
{
    self->packet[self->offset++] = (char) (i >> 24);
    self->packet[self->offset++] = (char) (i >> 16);
    self->packet[self->offset++] = (char) (i >> 8);
    self->packet[self->offset++] = (char) i;
}

static void addString(DataEnctyption self, char *s)
{
    addBytes(self, s, strlen(s));
    self->packet[self->offset++] = 10;
}

static void addBytes(DataEnctyption self, char *data, int len)
{
    for (int k = 0; k < len; ++k)
        self->packet[self->offset++] = *data++;
}

static void getBytes(DataEnctyption self, char *data, int len)
{
    for (int k = 0; k < len; k++)
        *data++ = self->packet[self->offset++];
}


static void encryptPacketWithKeys(DataEnctyption self)
{
    int i = self->offset;
    self->offset = 0;
    char dummyPackage[i];

    getBytes(self, dummyPackage, i);
    mpz_t bigint, bigpowmod;
    mpz_init(bigint);
    mpz_init(bigpowmod);
    /* create big integer */
    mpz_import(bigint, sizeof(dummyPackage), 1, sizeof(char), 0, 0, dummyPackage);
    /* enctrypt using <bigpowmod = pow(bigint, self->encrptkey) % self->encrptmod> */
    mpz_powm(bigpowmod, bigint, self->encrptkey, self->encrptmod);

    /* convert enctrypted data to char* */
    unsigned long wordcount = 0;
    char *bfr = (char *)mpz_export(NULL, &wordcount, 1, sizeof(char), 0, 0, bigint);
    /* copy to array with correct size */
    char encryptedPacket[wordcount];
    for(unsigned long i = 0; i < wordcount; ++i)
        encryptedPacket[i] = bfr[i];

    /* write to packet array */
    self->offset = 0;
    addByte(self, wordcount);
    addBytes(self, encryptedPacket, wordcount);
}
