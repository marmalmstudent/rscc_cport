#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>

#include <rsc/io/intops.h>
#include "crypto.h"

#define BUFF_SIZE 500

#define RSA_PRIVATE_KEY "df2d68618fb54940641b05121fdd9b4ca8cd3c25e8df94962ee9837a566121174d421657bc7ce7acd321321cb93e69faa8e0761fa53ae6cd2660b41da4f5b77"
#define RSA_PUBLIC_KEY  "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457"
#define RSA_MOD_HEX     "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877"

static mpz_t privkey;

Crypto crypto_ctor()
{
    Crypto c = (Crypto)malloc(sizeof(struct crypto_struct));
    c->buff = buffer_ctor(BUFF_SIZE);

    // make encryption keys
    mpz_init(c->encrptkey);
    mpz_set_str(c->encrptkey, RSA_PUBLIC_KEY, 16);
    mpz_init(c->encrptmod);
    mpz_set_str(c->encrptmod, RSA_MOD_HEX, 16);
    mpz_init(privkey);
    mpz_set_str(privkey, RSA_PRIVATE_KEY, 16);
    return c;
}

void crypto_dtor(Crypto obj)
{
    mpz_clear(obj->encrptkey);
    mpz_clear(obj->encrptmod);
    mpz_clear(privkey);
    buffer_dtor(obj->buff);
    free(obj);
    obj = NULL;
}

#include <stdio.h>

void decryptPackgetWithKeys(Crypto self)
{
  mpz_t bigint_raw;
  mpz_t bigint_enc;
  mpz_init(bigint_raw);
  mpz_init(bigint_enc);

  printf("Size: %d\n", self->buff->bfr[0]);
  mpz_import(bigint_enc, self->buff->bfr[0], 1, sizeof(char), 0, 0, self->buff->bfr+1);
  printf("Encrypted: [ ");
  for (int i = 0; i < self->buff->bfr[0]; ++i) {
    printf("%02x ", (uint8_t)self->buff->bfr[i+1]);
  }
  printf("]\n");
  
  mpz_powm(bigint_raw, bigint_enc, privkey, self->encrptmod);
  mpz_clear(bigint_enc);
  
  size_t len_dec = 0;
  char *bfr_dec = (char *)mpz_export(NULL, &len_dec, 1, sizeof(char), 0, 0, bigint_raw);
  char *msg_dec = (char *) calloc(len_dec, sizeof(char));
  memcpy(msg_dec, bfr_dec, len_dec);
  mpz_clear(bigint_raw);

  printf("Decrypted: [ ");
  for (size_t i = 0; i < len_dec; ++i) {
    printf("%02x ", (uint8_t)msg_dec[i]);
  }
  printf("]\n");
  free(msg_dec);
}

void encryptPacketWithKeys(Crypto self)
{
  /* copy data to package of smallest required length */
  size_t len = get_used(self->buff);
  char *pkt = (char *) calloc(len, sizeof(char));
  get_data(self->buff, pkt);
  
  printf("Original: [ ");
  for (size_t i = 0; i < len; ++i) {
    printf("%02x ", (uint8_t)pkt[i]);
  }
  printf("]\n");

  /* make the encrypted data */
  mpz_t bigint_raw;
  mpz_t bigint_enc;
  mpz_init(bigint_raw);
  mpz_init(bigint_enc);

  mpz_import(bigint_raw, len, 1, sizeof(char), 0, 0, pkt);
  free(pkt);
  mpz_powm(bigint_enc, bigint_raw, self->encrptkey, self->encrptmod);
  mpz_clear(bigint_raw);

  /* convert enctrypted data to char* */
  size_t len_enc = 0;
  char *bfr = (char *)mpz_export(NULL, &len_enc, 1, sizeof(char), 0, 0, bigint_enc);
  char *pkt_enc = (char *) calloc(len_enc, sizeof(char));
  memcpy(pkt_enc, bfr, len_enc);
  mpz_clear(bigint_enc);

  /* write to packet array */
  reset(self->buff);
  printf("Size: %zd\n", len_enc);
  put_1_byte(self->buff, (uint8_t)len_enc);
  put_data(self->buff, pkt_enc, len_enc);
  printf("Encrypted: [ ");
  for (size_t i = 0; i < len_enc; ++i) {
    printf("%02x ", (uint8_t)pkt_enc[i]);
  }
  printf("]\n");
  free(pkt_enc);
}
