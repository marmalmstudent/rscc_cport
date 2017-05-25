#include "../../include/client/dataenctyption.h"

DataEnctyption dataencryption_ctor()
{
    DataEnctyption d = (DataEnctyption)malloc(sizeof(struct dataencryption_struct));
    d->packet[PKT_SIZE];
    memset(b->buffer, 0, PKT_SIZE);

    mpz_init(c->encrptkey);
    mpz_set_str(c->encrptkey, "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457", 16);
    mpz_init(c->encrptmod);
    mpz_set_str(c->encrptmod, "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877", 16);
}

void dataencryption_dtor(DataEnctyption obj)
{
    free(obj);
}
